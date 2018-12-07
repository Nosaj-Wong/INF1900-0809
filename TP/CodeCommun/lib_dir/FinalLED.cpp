/*
** file: FinalLED.h
** Declaration de la classe LED du projet final
** date: 08 novembre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
*/

#include "FinalLED.h"

FinalLED::FinalLED() {}

void FinalLED::red() {
  stopCompare();
  PORTD &= ~(1 << 5);
  PORTD |= (1 << 7);
}

void FinalLED::green() {
  stopCompare();
  PORTD &= ~(1 << 7);
  PORTD |= (1 << 5);
}

void FinalLED::flashingGreen() {
  stopCompare();
  PORTD &= ~(1 << 7);
  TCCR1A = (1 << COM1A0);
  TCCR1B = (1 << WGM12 | 1 << CS11 | 1 << CS10);
  TCCR1C = 0;
  OCR1A = 31250;
  TCNT1 = 0;
}

void FinalLED::ambre() {
  stopCompare();
  PORTD &= ~(1 << 7 | 1 << 5);

  TCCR1A = (1 << COM1A1 | 1 << WGM10);
  TCCR2A = (1 << COM2A1 | 1 << COM2A0 | 1 << WGM20);

  TCCR1B = (1 << CS11);
  TCCR2B = (1 << CS21);

  TCCR1C = 0;

  OCR1A = 179;
  OCR2A = 179;

  TCNT1 = 0;
  TCNT2 = 0;
}

void FinalLED::off() {
  stopCompare();
  PORTD &= ~(1 << 7 | 1 << 5);
}

void FinalLED::stopCompare() {
  TCCR1A &= ~(1 << COM1A1 | 1 << COM1A0);
  TCCR2A &= ~(1 << COM2A1 | 1 << COM2A0);
}