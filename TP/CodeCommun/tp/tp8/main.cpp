#define F_CPU 8000000
#include <util/delay.h>
#include <string.h>

#include "debug.h"
#include "driver.h"

int main() {
  DDRB = 0xff;

  _delay_ms(1000);
  PWM moteurGauche(B4, 2, 0, 0);
  DEBUG_PRINT(("Moteur Gauche initialise\n"));

  PWM moteurDroit(B3, 5, 0, 0);
  DEBUG_PRINT(("Moteur Droit initialise\n"));

  Driver pilote(moteurGauche, moteurDroit);
  DEBUG_PRINT(("Vers l'avant 50\n"));
  pilote.forward(50);

  _delay_ms(1000);
  DEBUG_PRINT(("Vers l'arriere 75\n"));
  pilote.backward(75);

  _delay_ms(500);
  DEBUG_PRINT(("Arret\n"));
  pilote.stop();

  pilote.forward(50);
  _delay_ms(1000);
  DEBUG_PRINT(("Vers l'avant 50\n"));
  pilote.resume();

  _delay_ms(1000);
  DEBUG_PRINT(("Tourne a droite\n"));
  pilote.turnRight90();

  DEBUG_PRINT(("Tourne a gauche\n"));
  pilote.turnLeft90();
  _delay_ms(1000);

  DEBUG_PRINT(("Tourne 180\n"));
  pilote.turn180();

  _delay_ms(1000);
  pilote.stop();

  return 0;
}