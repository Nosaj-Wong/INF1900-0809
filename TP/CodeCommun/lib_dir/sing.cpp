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
void Sing::note(Note note, uint8_t factorOctave, double duration) {
    //   int frequency = note * factor;
    int frequency = note * factorOctave;
    // int frequency = 110 * factor;
    PWM_software(100, duration, frequency);
    PWM_software(100, 0.1, 0);
}

// Chante starWars et le repete
void Sing::starWars(uint8_t repetition, uint8_t factorOctave, double speed) {
    for (int i = 0; i < repetition; i++) {
        note(G, 1 * factorOctave, 0.1 * speed);
        note(G, 1 * factorOctave, 0.1 * speed);
        note(G, 1 * factorOctave, 0.1 * speed);

        note(C, 2 * factorOctave, 0.3 * speed);
        note(G, 2 * factorOctave, 0.3 * speed);

        note(F, 2 * factorOctave, 0.1 * speed);
        note(E, 2 * factorOctave, 0.1 * speed);
        note(D, 2 * factorOctave, 0.1 * speed);
        note(C, 4 * factorOctave, 0.3 * speed);
        note(G, 2 * factorOctave, 0.2 * speed);

        note(F, 2 * factorOctave, 0.1 * speed);
        note(E, 2 * factorOctave, 0.1 * speed);
        note(D, 2 * factorOctave, 0.1 * speed);
        note(C, 4 * factorOctave, 0.3 * speed);
        note(G, 2 * factorOctave, 0.2 * speed);

        note(F, 2 * factorOctave, 0.1 * speed);
        note(E, 2 * factorOctave, 0.1 * speed);
        note(F, 2 * factorOctave, 0.1 * speed);
        note(D, 2 * factorOctave, 0.3 * speed);
    }
}

void Sing::Mario(uint8_t repetition, int8_t factorOctave, double speed) {
    for (int i = 0; i < repetition; i++) {
        note(C, 1 * factorOctave, 0.1 * speed);
        note(E, 1 * factorOctave, 0.1 * speed);
        note(G, 1 * factorOctave, 0.1 * speed);
        note(C, 2 * factorOctave, 0.1 * speed);
        note(E, 2 * factorOctave, 0.1 * speed);

        note(G, 2 * factorOctave, 0.3 * speed);
        note(E, 2 * factorOctave, 0.2 * speed);

        note(C, 1 * factorOctave, 0.1 * speed);
        note(DE, 1 * factorOctave, 0.1 * speed);
        note(GA, 1 * factorOctave, 0.1 * speed);
        note(C, 2 * factorOctave, 0.1 * speed);
        note(DE, 2 * factorOctave, 0.1 * speed);

        note(GA, 2 * factorOctave, 0.3 * speed);
        note(DE, 2 * factorOctave, 0.2 * speed);

        note(D, 1 * factorOctave, 0.1 * speed);
        note(F, 1 * factorOctave, 0.1 * speed);
        note(AB, 2 * factorOctave, 0.1 * speed);
        note(D, 2 * factorOctave, 0.1 * speed);
        note(F, 2 * factorOctave, 0.1 * speed);

        note(AB, 4 * factorOctave, 0.3 * speed);

        note(AB, 4 * factorOctave, 0.1 * speed);
        note(AB, 4 * factorOctave, 0.1 * speed);
        note(AB, 4 * factorOctave, 0.1 * speed);

        note(C, 4 * factorOctave, 1.2 * speed);

    }
}
