/*
** file: main.cpp
** Test de la completion du parcours en mouvement
**
** date: 11 novembre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
**Configuration:  A0: Capteur distance gauche
                  A1: Capteur distance droit
                  A3-A7: Suiveur de ligne (A7=Cal)
                  AVCC-AGRND: Capteur distance droit

                  B0-B1: VCC-GRN suiveur de ligne (respectivement)
                  B2 et B4: Moteur droit (B2=Dir/B4=PWM)
                  B3 et B5: Moteur gauche (B5=Dir/B3=PWM)
                  B6-B7: Piezo-electrique
                  BVCC-BGRND: Capteur distance gauche

                  C en entier: Ecran LCD

                  D0-D1: Libre pour transmit UART
                  D2: Libre pour interrupteur
                  D3: Libre
                  D4: Libre pour timer 2 (interruptions)
                  D5 et D7: LED
                  D6: Libre
                  DVCC-DGRND: Libres

*/
#include "FinalDriver.h"
#include "FinalFunctions.h"
#include "FinalLED.h"
#include "FinalTimer.h"
#include "can.h"
#include "debug.h"
#include "lenghtSensor.h"
#include "singMario5seconds.h"

const uint16_t N_MAX_DATA = 200;

int8_t RIGHT_CAPTOR_ADJUST;
int8_t LEFT_CAPTOR_ADJUST;

uint16_t rightData[N_MAX_DATA] = {};
uint16_t leftData[N_MAX_DATA] = {};
uint16_t nData = 0;

can can;

// Division du parcours en parties
enum PathPart {
  beginning,
  straightForwardScanning,
  firstOrThirdTurn,
  straightForward,
  secondOrFourthTurn,
  lastLine,
  transmit
};

enum TimerMode { Scanning, TurnDelay };
TimerMode timerMode = Scanning;
volatile bool blindForwardIsFinish;
volatile uint8_t interruptCounter = 0;

ISR(TIMER1_COMPB_vect) {
  switch (timerMode) {
    case Scanning:
      DEBUG_PRINT(("Interruption!!"));
      read(can, leftData, rightData, nData, LEFT_CAPTOR_ADJUST,
           RIGHT_CAPTOR_ADJUST);
      break;

    case TurnDelay:
      DEBUG_PRINT(("2e Interrpupt"));
      if (interruptCounter != 0) {
        DEBUG_PRINT(("2e fois dans le interrupt"));
        blindForwardIsFinish = true;
        interruptCounter = 0;
      } else
        interruptCounter++;
      break;
  }
};

int main() {
  // Ports tel que convenu
  DDRA = 0x80;
  DDRB = 0xff;
  DDRC = 0xff;  // Verifier les entrees/sorties pour le LCD
  DDRD = 0xA0;

  // Mise en place du Cal et du Vcc Grnd pour le suiveur de ligne
  PORTA |= 0x80;
  PORTB |= 0x02;

  // Constante d'optimisation
  const uint8_t SCANNING_INTERVAL = 40;

  // Instancion des objets utilises
  PWM leftMotor(B4, 2);
  PWM rightMotor(B3, 5);
  Driver driver(leftMotor, rightMotor);
  FinalLED led;
  SingMario5seconds singMario5seconds;
  LCM disp(&DDRC, &PORTC);
  FinalTimer timer(SCANNING_INTERVAL);

  // Variables

  // Nom du robot
  char *robotName = "  ATmegaReussi";

  // Longueur des murs
  uint32_t lengthLeft = 0;
  uint32_t lengthRight = 0;

  // Compteur et timer de virages
  uint8_t turnCounter = 0;
  uint16_t turnTimer = 0;

  // Variable utilise dans le switch case
  PathPart PathPart = beginning;
  for (;;) {
    switch (PathPart) {
      //------------------------------------------------------//
      case beginning:
        // Couvre du debut jusqu'a la ligne pleine
        DEBUG_PRINT(("Beginning\n"));
        LEFT_CAPTOR_ADJUST = 53;
        RIGHT_CAPTOR_ADJUST = -14;

        // Afficher nom du robot
        disp.write(robotName);
        _delay_ms(3000);

        // On avance tant qu'on croise pas la ligne pleine
        driver.boostBothWheel(Forward);
        while (driver.lineCaptor() != fullLine) {
          driver.forwardStraight();
        }
        timer.start();
        led.red();
        // On attend d'avoir termine la ligne pleine
        driver.goTroughFork();

        // On passe a la partie de ligne droite avec scan actif
        PathPart = straightForwardScanning;
        break;

      //------------------------------------------------------//
      case straightForwardScanning:
        DEBUG_PRINT(("straightForwardScanning\n"));

        // On avance tant qu'on croise pas une fourche
        while (!(driver.lineCaptor() & forkOrFullLine)) {
          driver.forwardStraight();
        }

        // On continue de scanner jusqu'a ce qu'on traverse la fourche
        driver.goTroughFork();

        // On arrive a un virage donc on incremente
        turnCounter++;

        // On passe au virage approprie
        PathPart = firstOrThirdTurn;
        break;

      //------------------------------------------------------//
      case firstOrThirdTurn:
        DEBUG_PRINT(("FirstOrThirdTurn\n"));

        // Tant qu'on voit la ligne on avance
        while (driver.lineCaptor() != noLine) {
          driver.forwardStraight();
        }
        timer.stop();

        // On continu un peu encore pour eviter de scanner cette ligne
        // au debut de la manoeuvre de virage
        _delay_ms(100);

        // On tourne du bon cote selon notre avancement dans le
        // parcours
        if (turnCounter == 1) {
          driver.turnBackwardRight();
        }

        else if (turnCounter == 3) {
          driver.turnBackwardLeft();
        }

        // On continue a tourner tant qu'on se recentre pas sur la
        // ligne
        while (driver.lineCaptor() != center) {
          ;
        }

        // On arrete pour eviter d'avantager le moteur deja en marche
        driver.stop();
        _delay_ms(500);

        // On change de partie de parcours
        PathPart = straightForward;
        break;

      //------------------------------------------------------//
      case straightForward:
        DEBUG_PRINT(("straightForward\n"));

        // On met le timer en mode turn delay et on part le timer

        // On avance tant que la ligne de disparait pas
        driver.boostBothWheel(Forward);
        while (driver.lineCaptor() != noLine) {
          driver.forwardStraight();
        }

        // On continue ensuite pour une periode fixe a une vitesse
        // fixe suffisante pour mettre notre centre de rotation au
        // dessus du point C
        driver.forwardBlind();

        // Ensuite on attend un fraction du temps pris pour le dernier troncon
        // Ce temps varie selon le virage
        if (turnCounter == 1) {
          _delay_ms(3500);
        } else if (turnCounter == 3) {
          _delay_ms(3800);
        }

        // On arrete pour stabiliser la manoeuvre
        driver.stop();
        _delay_ms(500);

        // On s'apprete a faire un autre virage donc on incremente
        turnCounter++;
        // On passe au virage approprie
        PathPart = secondOrFourthTurn;
        break;

      //------------------------------------------------------//
      case secondOrFourthTurn:
        DEBUG_PRINT(("SecondTurn\n"));

        // On tourne du bon cote selon l'avancement du parcours
        if (turnCounter == 2) {
          driver.turnPivotLeft();
          LEFT_CAPTOR_ADJUST = -6;
          RIGHT_CAPTOR_ADJUST = 50;
        }

        else if (turnCounter == 4) {
          driver.turnPivotRight();
          LEFT_CAPTOR_ADJUST = 51;
          RIGHT_CAPTOR_ADJUST = -12;
        }

        // On tourne tant qu'on ne se recentre pas
        while (driver.lineCaptor() != center) {
          ;
        }

        // On pause pour ne pas donner avantage a une des deux roues
        driver.stop();
        _delay_ms(250);

        driver.boostBothWheel(Forward);
        for (uint16_t i = 0; i < 12000; i++) {
          driver.forwardStraight();
        }

        /* // On va vers l'arriere pour se mettre vis a vis le debut du
            // scan tant qu'on ne retrouve pas la fourche
            driver.boostBothWheel(Backward);
            while (!(driver.lineCaptor() & forkOrFullLine)) {
              driver.backwardStraight();
            }

            // Ensuite on repart vers l'avant et on s'assure d'avoir
            // traverse la fourche avant de rescanner (pour etre vraiment
            // vis-a-vis ou on a arrete)
            driver.boostBothWheel(Forward);
            driver.goTroughFork(); */
        timer.start();

        // On passe ensuite a l'etape appropriee selon l'avancement du
        // parcours
        if (turnCounter == 2) {
          PathPart = straightForwardScanning;
        }

        else if (turnCounter == 4) {
          PathPart = lastLine;
        }
        break;

      //------------------------------------------------------//
      case lastLine:
        DEBUG_PRINT(("LastLine\n"));

        // On avance tant qu'on ne rencontre pas la ligne pleine
        while (driver.lineCaptor() != fullLine) {
          driver.forwardStraight();
        }

        // Puis on cesse tout (moteurs/scanneurs)
        driver.stop();
        led.off();
        timer.stop();
        PathPart = transmit;
        break;

      //------------------------------------------------------//
      case transmit:
        DEBUG_PRINT(("Transmit\n"));

        // On calcule la langueur du mur gauche
        smoothingTunnel(nData, leftData);
        smoothingTunnel(nData, rightData);
        lengthLeft = calculateLenghtTest3(leftData, nData);

        // On calcule la langueur du mur droite
        lengthRight = calculateLenghtTest3(rightData, nData);

        // On affiche les deux resultats calcules
        displayFinalResults(disp, lengthLeft, lengthRight);

        // On entre ensuite toujours dans un etat pres a transmettre
        // les donnees sur pression du bouton pressoir
        for (;;) {
          if (buttonPressed()) {
            led.flashingGreen();
            printTunnel(nData, leftData, rightData);
            char tableau[10];
            //! ATTENTION AFFICHE LE NOMBRE DE VALEUR POUR LINSTANT COMME 2E
            //! DONNEE
            intToString(nData, tableau);
            DEBUG_PRINT((tableau));
            led.ambre();
            singMario5seconds.UneFois();
            led.off();
          }
        }
        break;
    }
  }
  return 0;
}
