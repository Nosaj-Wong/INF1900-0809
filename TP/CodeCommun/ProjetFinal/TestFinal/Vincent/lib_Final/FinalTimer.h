/*
** file: FinalTimer.h
** Declaration de la classe FinalTimer qui permet de produire des interruptions
** a une frequence souhaitee
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

#include <avr/interrupt.h>
#include <avr/io.h>

class FinalTimer {
 public:

  FinalTimer(uint16_t duration_10ms);

  void start();

  void stop();

  void setDuration(uint16_t duration_10ms);

  uint16_t getCurrentTimerCount();

  void startWithoutInterrupt();

 private:
};