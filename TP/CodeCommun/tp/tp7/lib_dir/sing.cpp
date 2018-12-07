/*
** file: sing.h
** Implementation de la classe sing
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

#include "sing.h"

// Constructeur modifiant la duree et les repetitions de la chanson
Sing::Sing() {}


// Chante une note en modifiant les attribut note et duration
void Sing::note(uint8_t note, double duration) {
  double hz = 0;
  switch (note) {
    case 1:
      hz = 440 / 2;
      break;
    case 2:
      hz = 493 / 2;
      break;
    case 3:
      hz = 554 / 2;
      break;
    case 4:
      hz = 587 / 2;
      break;
    case 5:
      hz = 659 / 2;
      break;
    case 6:
      hz = 739 / 2;
      break;
    case 7:
      hz = 830 / 2;
      break;
    case 11:
      hz = 440;
      break;
    case 22:
      hz = 493;
      break;
    case 33:
      hz = 554;
      break;
    case 44:
      hz = 587;
      break;
    case 55:
      hz = 659;
      break;
    case 66:
      hz = 739;
      break;
    case 77:
      hz = 830;
      break;
    case 111:
      hz = 880;
      break;
  }
  PWM_software(100, duration, hz);
  PWM_software(100, 0.1, 0);  // 0
}

// Chante starWars et le repete
void Sing::starWars(uint8_t repetition) {
  for (int i = 0; i < repetition; i++) {
    note(5, 0.1 * 3);
    note(5, 0.1 * 3);
    note(5, 0.1 * 3);

    note(11, 0.3 * 3);
    note(55, 0.3 * 3);

    note(44, 0.1 * 3);
    note(33, 0.1 * 3);
    note(22, 0.1 * 3);
    note(111, 0.3 * 3);
    note(55, 0.2 * 3);

    note(44, 0.1 * 3);
    note(33, 0.1 * 3);
    note(22, 0.1 * 3);
    note(111, 0.3 * 3);
    note(55, 0.2 * 3);

    note(44, 0.1 * 3);
    note(33, 0.1 * 3);
    note(44, 0.1 * 3);
    note(22, 0.3 * 3);
  }
}
