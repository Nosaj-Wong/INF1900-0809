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

#include "singMario5seconds.h"

// Constructeur modifiant la duree et les repetitions de la chanson
SingMario5seconds::SingMario5seconds() {}

// Chante une note en modifiant les attribut note et duration
void SingMario5seconds::note(Note note, uint8_t factorOctave, double duration) {
    //   int frequency = note * factor;
    int frequency = note * factorOctave;
    // int frequency = 110 * factor;
    PWM_software(100, duration, frequency);
    PWM_software(100, 0.1, 0);
}
// Liste des notes a jouer pour notre melodie
void SingMario5seconds::UneFois() {
        note(C, 2, 0.12);
        note(E, 2, 0.12);
        note(G, 2, 0.12);
        note(C, 4, 0.12);
        note(E, 4, 0.12);

        note(G, 4, 0.36);
        note(E, 4, 0.24);

        note(C, 2, 0.12);
        note(DE, 2, 0.12);
        note(GA, 2, 0.12);
        note(C, 4, 0.12);
        note(DE, 4, 0.12);

        note(GA, 4, 0.36);
        note(DE, 4, 0.24);

        note(D, 2, 0.12);
        note(F, 2, 0.12);
        note(AB, 4, 0.12);
        note(D, 4, 0.12);
        note(F, 4, 0.12);

        note(AB, 8, 0.36);

        note(AB, 8, 0.12);
        note(AB, 8, 0.12);
        note(AB, 8, 0.12);

        note(C, 8, 1.44);
}
