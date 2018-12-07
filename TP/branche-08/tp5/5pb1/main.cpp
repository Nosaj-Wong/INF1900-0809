/**
* Programme qui change la couleur du LED quand on appuie et/ou relache le bouton. Bourne positive du LED est branchee sur PIN2 du PORTC
* \file exo#3.cpp
* \authors Facundo GIANNOTTI 1882989 et Xi Chen SHEN 1930285
* \date 4 septembre 2018

Entree: DDRD, verifiee par la fonction buttonPressed
Sortie: DDRC

**/

#define F_CPU 8000000
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "memoire_24.h"

void lightGreen() {
    PORTB = 0x01;
}
void lightRed() {
    PORTB = 0x02;
}
void lightOff() {
    PORTB = 0x00;
}

int main() {
    DDRB = 0xff;

    lightRed();
    _delay_ms(1000);  //change

    uint8_t adresse = 0x00;

    Memoire24CXXX memoire = Memoire24CXXX();
    uint8_t donne[22]     = {'P', 'O', 'L', 'Y', 'T', 'E', 'C', 'H', 'N', 'I', 'Q', 'U', 'E', ' ', 'M', 'O', 'N', 'T', 'R', 'E', 'A', 'L'};

    memoire.ecriture(adresse, donne, uint8_t(22));
    _delay_ms(5);

    uint8_t resultat[22] = {};
    memoire.lecture(adresse, resultat, uint8_t(22));

    bool isEqual = true;
    for (uint8_t i = 0; i < 22; i++) {
        if (donne[i] != resultat[i]) {
            isEqual = false;
        }
    }

    lightOff();
    _delay_ms(2000);
    if (isEqual) {
        lightGreen();
        _delay_ms(2000);
        lightOff();
        _delay_ms(2000);
    } else {
        lightRed();
        _delay_ms(2000);
        lightOff();
        _delay_ms(2000);
    }
};