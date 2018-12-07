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
#include "FinalLED.h"
#include "can.h"
#include "debug.h"
#include "functions.h"

// TODO: Ajouter les include pertinents

// TODO : Declarer tableaux et compteur de donnees comme variable globales

// Division du parcours en parties
enum TrajectPart {
  beginning,
  straightForwardScanning,
  firstOrThirdTurn,
  straightForward,
  secondOrFourthTurn,
  lastLine,
  transmit
};

// TODO: Ecrire la routine d'interruption avec le stockage des distances

int main() {
  // Ports tel que convenu
  DDRA = 0x80;
  DDRB = 0xff;
  DDRC = 0xff;
  DDRD = 0xA0;

  // Mise en place du Cal et du Vcc Grnd pour le suiveur de ligne
  PORTA |= 0x80;
  PORTB |= 0x02;

  // Instancion des objets utilises
  // TODO: Intancier le sing optimise
  // TODO: Instancier le LCD et les capteurs de distance/calcul de longeurs

  can can;
  PWM leftMotor(B4, 2);
  PWM rightMotor(B3, 5);
  Driver driver(leftMotor, rightMotor);
  FinalLED led;

  // Variables

  // Compteur de virages
  uint8_t turnCounter = 2;

  // Variable utilise dans le switch case
  TrajectPart trajectPart = secondOrFourthTurn;

  for (;;) { // A retirer lors de l'examen
    if (PIND & 0x04) { // A retirer lors de l'examen
      for (;;) {
        switch (trajectPart) {
          //------------------------------------------------------//
          case beginning:
            // Couvre du debut jusqu'a la ligne pleine
            DEBUG_PRINT(("Beginning\n"));
            // TODO: Afficher nom du robot
            //_delay_ms(3000);

            // On avance tant qu'on croise pas la ligne pleine
            driver.boostBothWheel(Forward);
            while (driver.lineCaptor() != fullLine) {
              driver.forwardStraight();
            }

            // TODO Partir Scanner
            led.red();

            // On attend d'avoir termine la ligne pleine
            driver.goTroughFork();

            // On passe a la partie de ligne droite avec scan actif
            trajectPart = straightForwardScanning;
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

            // TODO: Arreter Scanner
            led.off();

            // On arrive a un virage donc on incremente
            turnCounter++;

            // On passe au virage approprie
            trajectPart = firstOrThirdTurn;
            break;

          //------------------------------------------------------//
          case firstOrThirdTurn:
            DEBUG_PRINT(("FirstOrThirdTurn\n"));

            // Tant qu'on voit la ligne on avance
            while (driver.lineCaptor() != noLine) {
              driver.forwardStraight();
            }

            // On continu un peu encore pour eviter de scanner cette ligne au
            // debut de la manoeuvre de virage
            _delay_ms(25);

            // On tourne du bon cote selon notre avancement dans le parcours
            if (turnCounter == 1) {
              driver.turnBackwardRight();
            }

            else if (turnCounter == 3) {
              driver.turnBackwardLeft();
            }

            // On continue a tourner tant qu'on se recentre pas sur la ligne
            while (driver.lineCaptor() != center) {
              ;
            }

            // On arrete pour eviter d'avantager le moteur deja en marche
            driver.stop();
            _delay_ms(500);

            // On change de partie de parcours
            trajectPart = straightForward;
            break;

          //------------------------------------------------------//
          case straightForward:
            DEBUG_PRINT(("straightForward\n"));

            // On avance tant que la ligne de disparait pas
            driver.boostBothWheel(Forward);
            while (driver.lineCaptor() != noLine) {
              driver.forwardStraight();
            }

            // On continue ensuite pour une periode fixe a une vitesse fixe
            // suffisante pour mettre notre centre de rotation au dessus du
            // point C
            driver.forwardPercent(BASICSPEED);
            _delay_ms(1950);

            // On arrete pour stabiliser la manoeuvre
            driver.stop();
            _delay_ms(500);

            // On s'apprete a faire un autre virage donc on incremente
            turnCounter++;

            // On passe au virage a approprie
            trajectPart = secondOrFourthTurn;
            break;

          //------------------------------------------------------//
          case secondOrFourthTurn:
            DEBUG_PRINT(("SecondTurn\n"));

            // On tourne du bon cote selon l'avancement du parcours
            if (turnCounter == 2) {
              driver.turnPivotLeft();
            }

            else if (turnCounter == 4) {
              driver.turnPivotRight();
            }

            // On tourne tant qu'on ne se recentre pas
            while (driver.lineCaptor() != center) {
              ;
            }

            // On pause pour ne pas donner avantage a une des deux roues
            driver.stop();
            _delay_ms(250);

            // On va vers l'arriere pour se mettre vis a vis le debut du scan
            // tant qu'on ne retrouve pas la fourche
            driver.boostBothWheel(Backward);
            while (!(driver.lineCaptor() & forkOrFullLine)) {
              driver.backwardStraight();
            }

            // Ensuite on repart vers l'avant et on s'assure d'avoir traverse la
            // fourche avant de rescanner (pour etre vraiment vis-a-vis ou on a
            // arrete)
            driver.boostBothWheel(Forward);
            driver.goTroughFork();

            // TODO: Repartir Scanner
            led.red();

            // On passe ensuite a l'etape appropriee selon l'avancement du
            // parcours
            if (turnCounter == 2) {
              trajectPart = straightForwardScanning;
            }

            else if (turnCounter == 4) {
              trajectPart = lastLine;
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
            // TODO: stop capteurs
            trajectPart = transmit;
            break;

          //------------------------------------------------------//
          case transmit:
            DEBUG_PRINT(("Transmit\n"));
            // TODO: Calculer la longueur des murs
            // TODO: Afficher la longueur des murs

            // On entre ensuite toujours dans un etat pres a transmettre les
            // donnees sur pression du bouton pressoir
            for (;;) {
              if (PIND & 0x04) {
                led.flashingGreen();
                // TODO: Affichier le tunnel
                led.ambre();
                // TODO: Jouer la chanson
                led.off();
              }
            }
            break;
        }
      }
    }
  }
  return 0;
}
