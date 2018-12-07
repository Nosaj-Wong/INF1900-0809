/*
* Programme qui change la couleur du LED quand on appuie et/ou relache le bouton. Borne positive du LED est branchee sur PIN2 du PORTC
* \file pb2.cpp
* \authors Facundo GIANNOTTI 1882989 et Xi Chen SHEN 1930285
* \date 4 septembre 2018

Entree: DDRD, verifiee par la fonction buttonPressed
Sortie: DDRC
 
 Table 1: States
| Current State | Button pressed | Next State |            Output             |
|---------------|----------------|------------|-------------------------------|
| E0            | 0              | E0         | ---                           |
| E0            | 1              | E1         | ---                           |
|               |                |            | Green(0.5s) -> Off(2s)        |
| E1            | ---            | E0         | ->flash Red(Compteur/2 times) |
|               |                |            | -> Green(1s)                  |

state = 0 : fait rien
state = 1 : compteur ++ jusqu'a 120
state = 2 : vert 1/2 seconde, attend 2 secondes, clignote x fois, vert une seconde, state to 0
*/

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

bool buttonPressed() {
    if (PIND & 0b00000100) {
        _delay_ms(10);
        if (PIND & 0b00000100) {
            return 1;
        }
    }
    return 0;
}

int main() {
    DDRD = 0x00;  //PORTD-> In
    DDRB = 0xFF;  //PORTB -> Out

    uint8_t Green = 0x01;
    uint8_t Red = 0x02;
    uint8_t Off = 0x00;

    unsigned long compteur = 0;

    enum States {
        E0,  //INIT, before pressed
        E1,  //after 1st pressed
    };
    States state = E0;
    for (;;) {
        switch (state) {
            case E0:
                PORTB = Off;
                if (buttonPressed()) {
                    while (buttonPressed() && compteur <= 120) {
                        compteur++;
                        _delay_ms(100);
                    }
                    state = E1;
                }
                break;
            case E1:
                PORTB = Green;
                _delay_ms(500);
                PORTB = Off;
                _delay_ms(2000);
                for (unsigned int i = 0; i < (compteur / 2); i++) {
                    PORTB = Red;
                    _delay_ms(250);
                    PORTB = Off;
                    _delay_ms(250);
                }
                PORTB = Green;
                _delay_ms(1000);
                state = E0;
                break;
        }
    }
}
