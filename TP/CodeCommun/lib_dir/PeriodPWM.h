/*
** file: PeriodPWM.h
** Declaration de la classe LED permettant le controle d'une LED
**
** date: 30 octobre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
*/

#ifndef F_CPU
#define F_CPU 8000000
#endif

#ifndef PERIODPWM_H
#define PERIODPWM_H

#include <avr/io.h>
#include <avr/interrupt.h>

class PeriodPWM {
   public:
    PeriodPWM(uint16_t periode = 0);
    uint8_t getPeriod();
    void setPeriod(uint16_t periode);
    void start();
    void stop();

   private:
    uint16_t periode_;
};

#endif