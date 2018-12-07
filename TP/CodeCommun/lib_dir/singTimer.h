/*
** file: singTimer.h
** Declaration de la classe singTimer
** Cette classe permet de produire des notes ou des melodies sur le haut-parleur du robot a l'aide du timer (PWM materiel)
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

#ifndef SINGTIMER_H
#define SINGTIMER_H

#include "PeriodPWM.h"

class SingTimer {
public:
  SingTimer();

  // Note de music
  void note(uint8_t note);
  void stop();

  // Chansons
  //void starWars(uint8_t repetition);

private:
  uint16_t notes_[37];
  PeriodPWM PWM_;
};

#endif /* SINGTIMER_H */
