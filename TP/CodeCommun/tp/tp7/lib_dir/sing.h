/*
** file: sing.h
** Declaration de la classe sing
** Cette classe permet de produire des notes ou des melodies sur le haut-parleur du robot
** 
** date: 22 octobre 2018
** 
** authors: 
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
** 
*/

#ifndef SING_H
#define SING_H

#include "functions.h"

class Sing {
public:
  Sing();

  // Note de music
  void note(uint8_t note,double duration);

  // Chansons
  void starWars(uint8_t repetition);

private:
};

#endif /* SING_H */
