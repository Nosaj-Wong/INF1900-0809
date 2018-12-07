/*
** file: PeriodPWM.cpp
** Implementation de la classe PeriodPWM qui permet de generer un PWM avec le timer de 16 bits

** date: 30 octobre 2018
** 
** authors: 
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
*/

#include "PeriodPWM.h"

PeriodPWM::PeriodPWM(uint16_t periode)
    : periode_(periode) {
    // mode CTC du timer 1
    TCNT1 = 0;  // Start with 0
    OCR1A = periode_;
    TCCR1A &= ~(1 << COM1A0);               // Set OCnA/OCnB on mode Toggle, et mode CTC, on ne part pas le PWM
    TCCR1B |= (1 << CS10) | (1 << WGM12);  // mode 001, clkI/O 1 (From prescaler)
    TCCR1C = 0;
}

uint8_t PeriodPWM::getPeriod() {
    return periode_;
}
void PeriodPWM::setPeriod(uint16_t periode) {
    periode_ = periode;
    OCR1A = periode_;
}

void PeriodPWM::start() {
    TCCR1A |= (1 << COM1A0);  // Set OCnA/OCnB on mode Toggle, et mode CTC, on part le PWM
}

void PeriodPWM::stop() {
    TCCR1A &= ~(1 << COM1A0);  // Set OCnA/OCnB on mode Toggle, et mode CTC, on arrete le PWM
}
