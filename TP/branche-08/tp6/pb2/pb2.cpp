/*
* Programme qui change la couleur du LED quand on appuie et/ou relache le bouton. Bourne positive du LED est branchee sur PIN2 du PORTC
* \file pb2.cpp
* \authors Facundo GIANNOTTI 1882989 et Xi Chen SHEN 1930285
* \date 4 septembre 2018

Entree: DDRD, verifiee par la fonction buttonPressed
Sortie: DDRC
 
 Table 1: Tableau des états

state = 0 : fait rien
state = 1 : compteur ++ jusqu'a 120
state = 2 : vert 1/2 seconde, attend 2 secondes, clignote x fois, vert une seconde, state to 0
*/

#define F_CPU 8000000
#include <avr/io.h>
#include <can.h>
#include <util/delay.h>

int main() {
    DDRA = 0x00;  //PORTD-> In
    DDRB = 0xFF;  //PORTA -> Out

    uint16_t Green = 0x02;
    uint16_t Red = 0x01;
    uint8_t compteur = 0;

    can can;
    uint8_t resultat;

    for (;;) {
        compteur++;
        // lecture dans PIN0 pour avoir 10 bits, ensuite declaler pour avoir 8 bits
        resultat = uint8_t(can.lecture(2) >> 2);
        if (resultat > 240) {
            PORTB = Red;
            _delay_ms(5);
        } else if (resultat < 240 && resultat > 200) {
            PORTB = compteur;
        } else {
            PORTB = Green;
            _delay_ms(5);
        }
    }
}
