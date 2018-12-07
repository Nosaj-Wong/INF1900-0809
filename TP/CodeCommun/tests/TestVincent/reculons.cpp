/*
** file: main.cpp
** Test de la completion du parcours en mouvement
**
** date: 00 novembre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
*/
#define F_CPU 8000000
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include "FinalDriver.h"
#include "FinalLED.h"
#include "can.h"
#include "debug.h"
#include "functions.h"
#include "sing.h"

int main() {
  DDRA = 0x80;
  DDRB = 0x3f;
  DDRD = 0xA0;

  PORTA |= 0x80;
  PORTB |= 0x02;
  PWM leftMotor(B4, 2);
  PWM rightMotor(B3, 5);

  Driver driver(leftMotor, rightMotor);

  for (;;) {
    if (PIND & 0x04) {
      driver.boostBothWheel(Backward);
      for (;;) {
        driver.backwardStraight();
      }
    }
  }
}