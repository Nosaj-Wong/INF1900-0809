
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

#include "FinalFunctions.h"

// Definition des notes (C=Do)
enum Note {
    A = 110,
    AB = 117,
    B = 123,
    C = 131,
    CD = 139,
    D = 147,
    DE = 156,
    E = 165,
    F = 175,
    FG = 185,
    G = 196,
    GA = 208
};

class SingMario5seconds {
   public:
    SingMario5seconds();

    // Note de music
    void note(Note note, uint8_t factorOctave, double duration);

    // Chansons
    void UneFois();

   private:
};

#endif /* SING_H */
