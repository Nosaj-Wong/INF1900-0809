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

int main() {
  for (;;) {
    uint16_t resultatDroite = 0;
    uint16_t resultatGauche = 0;
    for (int i = 0; i < VALUES_PER_READ; i++) {
      resultatDroite += can.lecture(1);  // capteur droit
      _delay_ms(TIME_PER_READ);
      resultatGauche += can.lecture(0);  // capteur gauche
      _delay_ms(TIME_PER_READ);
    }
    // on calcule la moyenne
    resultatDroite = resultatDroite / VALUES_PER_READ;
    resultatGauche = resultatGauche / VALUES_PER_READ;

    // calcul pasant du voltage sur 10 bit a la longeur en (pouces/16)

if (resultatDroite>=185)
    resultatDroite =
        RIGHT_NUMERATOR1 / (resultatDroite - RIGHT_DENOMINATOR_CONSTANT1) -
        RIGHT_CORRECTION_CONSTANT1;

else
    resultatDroite =
        RIGHT_NUMERATOR2 / (resultatDroite - RIGHT_DENOMINATOR_CONSTANT2) +
        RIGHT_CORRECTION_CONSTANT2;


    if (resultatGauche >= 213)
      resultatGauche =
          LEFT_NUMERATOR1 / (resultatGauche - LEFT_DENOMINATOR_CONSTANT1) -
          LEFT_CORRECTION_CONSTANT1;
    else
      resultatGauche =
          LEFT_NUMERATOR2 / (resultatGauche + LEFT_DENOMINATOR_CONSTANT2) +
          LEFT_CORRECTION_CONSTANT2;

    char tableau[10];
    intToString(resultatDroite, tableau);
    DEBUG_PRINT((tableau));
  }
  return 0;
}
