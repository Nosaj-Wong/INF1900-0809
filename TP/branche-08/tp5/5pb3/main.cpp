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

void initialisationUART(void) {
    // 2400 bauds. Nous vous donnons la valeur des deux
    // premier registres pour vous éviter des complications
    UBRR0H = 0;
    UBRR0L = 0xCF;

    // permettre la reception et la transmission par le UART0
    UCSR0A |= (1 << UDRE0);
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

    // Format des trames: 8 bits, 1 stop bits, none parity
    UCSR0C |= (0 << UCSZ02) | (1 << UCSZ01) | (1 << UCSZ00);
}

void transmissionUART(uint8_t donnee) {
    while (!(UCSR0A & (1 << UDRE0))) {
    }
    UDR0 = donnee;
}

int main() {
    DDRB = 0xff;

     uint8_t adresse;

    initialisationUART();
    Memoire24CXXX memoire = Memoire24CXXX();


    lightRed();
    _delay_ms(1000);
    lightOff();

    // char mots[21] = "Le robot en INF1900\n";
    uint8_t valeurLecture;
    //uint8_t position;
    for(adresse = 0x00; valeurLecture!= 0xFF; adresse++){
        memoire.lecture(adresse, &valeurLecture);
        if(valeurLecture != 0xFF)
            transmissionUART(valeurLecture);
    }

    // uint8_t* resultat = new uint8_t[position];
    // memoire.lecture(adreesse, resultat, uint8_t(position));

    // uint8_t i, j;
    // for (i = 0; i < 100; i++) {
    //     for (j = 0; j < position; j++) {
    //         transmissionUART(resultat[j]);
    //     }
        // lightRed();
        // _delay_ms(200);
        // lightOff();
        // _delay_ms(200);
    //}
};