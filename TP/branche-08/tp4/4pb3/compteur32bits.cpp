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

void ajustementPWM(double pourcentage) {
    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
    // page 177 de la description technique du ATmega324PA)

    // division d'horloge par 8 - implique une frequence de PWM fixe
    OCR1A = 255 - (255 * pourcentage / 100);

    OCR1B = 255 - (255 * pourcentage / 100);

    TCCR1A |= (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0) | (1 << WGM10);  //Set OCnA/OCnB on Compare Match (Set output to high level). PWM, Phase Correct, 8-bit

    TCCR1B |= (1 << CS11);  // clkI/O 1/8 (From prescaler)

    TCCR1C = 0;
}

int main() {
    DDRD           = 0xff;

    for (int i = 0; i < 5; i++) {
        ajustementPWM(25 * i);
        _delay_ms(2000);
    }
    ajustementPWM(0);

    return 0;
}
