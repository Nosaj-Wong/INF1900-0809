
/*
** file: FinalTimer.cpp
** Implementation de la classe FinalTimer qui permet de produire des
** interruptions a une frequence souhaitee
**
** date: 16 novembre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
*/
#include "FinalTimer.h"

// Constructeur qui initialise les registres de minuterie et d'interruption
FinalTimer::FinalTimer(uint16_t duration_10ms) {
  // Activation du compare match pour OCR1B, sortie a 1 lorsque match
  TCCR1A |= (1 << COM1B1) | (1 << COM1B0);

  // Diminution de l'horloge a 1/1024, mode CTC
  TCCR1B |= (1 << WGM12 | 1 << CS10) | (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1C = 0;

  // CTC depend de OCR1A donc on le modifie egalement
  // 78 represente environ 10ms puisque 8 000 000/1024 = 7812.5
  // donc le compteur atteint ce nombre en 1 seconde et 78 en 1/100 de secondes
  OCR1A = 78 * duration_10ms;
  OCR1B = 78 * duration_10ms;
}

// Active les interruptions pour le timer
void FinalTimer::start() {
  // Recommence le compte a 0 pour conserver le rythme des interruptions
  TCNT1 = 0;
  TIMSK1 |= (1 << OCIE1B);
  sei();
}

// Stop: Desactive les interruptions
void FinalTimer::stop() {
  TIMSK1 &= ~(1 << OCIE1B);

  cli();
}

// Permet de modifier la duree du timer
void FinalTimer::setDuration(uint16_t duration_10ms) {
  OCR1A = 78 * duration_10ms;
  OCR1B = 78 * duration_10ms;
}

// Permet de savoir a combien est rendu le timer
uint16_t FinalTimer::getCurrentTimerCount() { return TCNT1/78; }

// Permet de commencer a compter mais sans activer le mode interrupt
void FinalTimer::startWithoutInterrupt(){TCNT1=0;}