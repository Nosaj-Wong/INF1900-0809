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

#ifndef FINALLED_H
#define FINALLED_H

#include <avr/io.h>

class FinalLED {
 public:
  FinalLED();

  void red();
  void green();
  void flashingGreen();
  void ambre();
  void off();

 private:
  void stopCompare();
};

#endif