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
                  Potentiometre a 2.8V

*/
#include "FinalDriver.h"
#include "FinalFunctions.h"
#include "FinalLED.h"
#include "FinalTimer.h"
#include "can.h"
#include "debug.h"
#include "lenghtSensor.h"
#include "singMario5seconds.h"

// Creation des variables globales utiles a la fois aux interruptions et au main

// Taille des tableaux de donnees de distance
const uint16_t N_MAX_DATA = 200;

// Variable d'ajustement des valeurs de distances lors des changements de cote
int8_t RIGHT_CAPTOR_ADJUST;
int8_t LEFT_CAPTOR_ADJUST;

// Les 2 tableaux contenant les distances
uint16_t rightData[N_MAX_DATA] = {};
uint16_t leftData[N_MAX_DATA] = {};

// La variable de suivi du nombre de donnees prises
uint16_t nData = 0;

// Le convertisseur analogique-numerique
can can;

// Division du parcours en parties
enum PathPart {
  // Du debut a la ligne pleine
  beginning,

  // Les sections de ligne droite et de scannage
  straightForwardScanning,

  // Le premier et 3e virage
  firstOrThirdTurn,

  // Les secions de ligne droite sans scannage
  straightForward,

  // Le deuxieme et quatrieme virage
  secondOrFourthTurn,

  // La derniere ligne droite
  lastLine,

  // Le mode de transmission des infos a l'ordinateur
  transmit
};

// Interuuption lie au comparateur B de la mituterie 1
ISR(TIMER1_COMPB_vect) {
  DEBUG_PRINT(("Interruption!!"));

  // On lit les donnes actuelle a laide du can et on les modifie selon
  // l'ajustement pour le cote actuel du deplacement, on les ajoute au tableau,
  // on increment nData
  read(can, leftData, rightData, nData, LEFT_CAPTOR_ADJUST,
       RIGHT_CAPTOR_ADJUST);
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

  // Constante d'optimisation de la frequence de prise de mesure (40=40
  // centiSec)
  const uint8_t SCANNING_INTERVAL = 40;

  // Instancion des objets utilises
  PWM leftMotor(B4, 2);
  PWM rightMotor(B3, 5);
  Driver driver(leftMotor, rightMotor);
  FinalLED led;
  SingMario5seconds singMario5seconds;
  LCM disp(&DDRC, &PORTC);
  FinalTimer timer(SCANNING_INTERVAL);

  // Nom du robot
  char *robotName = "  ATmegaReussi";

  // Longueur des murs
  uint32_t lengthLeft = 0;
  uint32_t lengthRight = 0;

  // Compteur et timer de virages
  uint8_t turnCounter = 0;

  // Variable utilise dans le switch case
  PathPart PathPart = beginning;
  for (;;) {
    switch (PathPart) {
      //------------------------------------------------------//
      case beginning:

        // Couvre du debut jusqu'a la ligne pleine

        // On initilaise l'ajustement des deistances pour le parcours du tunnel
        // du cote gauche
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

        // On debute les mesures de distance et on allume la LED
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

        // On passe a travers la fourche
        driver.goTroughFork();

        // On arrive a un virage donc on incremente
        turnCounter++;

        // On passe au virage approprie
        PathPart = firstOrThirdTurn;
        break;

      //------------------------------------------------------//
      case firstOrThirdTurn:
        DEBUG_PRINT(("FirstOrThirdTurn\n"));

        // Tant qu'on voit la ligne on avance et on prend des mesures
        while (driver.lineCaptor() != noLine) {
          driver.forwardStraight();
        }

        // Une fois qu'on perd la ligne de vue, on cesse la prise de mesures
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
        // ligne suivante
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

        // On avance tant que la ligne de disparait pas
        driver.boostBothWheel(Forward);
        while (driver.lineCaptor() != noLine) {
          driver.forwardStraight();
        }

        // On continue ensuite pour une periode fixe a une vitesse
        // fixe suffisante pour mettre notre centre de rotation au
        // dessus du point C
        driver.forwardBlind();

        // Cette duree fixe varie selon le virage dans lequel on se trouve
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
        DEBUG_PRINT(("SecondOrFourthTurn\n"));

        // On tourne du bon cote selon l'avancement du parcours. On actualise
        // les constantes d'ajustement maintenant que nous changeons de cote du
        // tunnel
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

        // On avant pour une duree fixe pour se mettre au niveau de la derniere
        // mesure prise de l'autre cote
        driver.boostBothWheel(Forward);
        for (uint16_t i = 0; i < 12000; i++) {
          driver.forwardStraight();
        }

        // On recommence a prendre des donnee
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

        // On passe a la derniere etape du parcours
        PathPart = transmit;
        break;

      //------------------------------------------------------//
      case transmit:
        DEBUG_PRINT(("Transmit\n"));

        // On applique un algorithme d'adoucissement de l'apparence des parois
        smoothingTunnel(nData, leftData);
        smoothingTunnel(nData, rightData);

        // ON calcule la longueur du mur gauche
        lengthLeft = calculateLenghtSqrt(leftData, nData);

        // On calcule la langueur du mur droite
        lengthRight = calculateLenghtSqrt(rightData, nData);

        // On affiche les deux resultats calcules
        displayFinalResults(disp, lengthLeft, lengthRight);

        // On entre ensuite toujours dans un etat pres a transmettre
        // les donnees sur pression du bouton pressoir
        for (;;) {
          if (buttonPressed()) {
            // On fait flasher la DEL verte 2x par seconde de facon materiel
            led.flashingGreen();

            // On imprime le tunnel a l'ecran
            printTunnel(nData, leftData, rightData);

            // On met la LED en mode ambre de facon materiel
            led.ambre();

            // On joue une melodie de 5 secondes
            singMario5seconds.UneFois();

            // On eteint la DEL
            led.off();
          }
        }
        break;
    }
  }
  return 0;
}
