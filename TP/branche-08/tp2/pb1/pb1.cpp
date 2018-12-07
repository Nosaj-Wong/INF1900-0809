/**
* Afficher rouge apres avoir appuyer et relacher 5 fois sur le bouton. Bourne positive du LED est branchee sur PIN2 du PORTC
* \file pb1.cpp
* \authors Facundo GIANNOTTI 1882989 et Xi Chen SHEN 1930285
* \date 4 septembre 2018

Entree: DDRD, verifiee par la fonction buttonPressed
Sortie: DDRC

Table 1: Tableau des états
---------------------------------------------------------------
 Etat present    Entree DDRD     Etat suivant    Sortie DDRC       
    INIT             0              INIT            0x00
    INIT             1               E1             0x00
    E1               0               E1             0x00
    E1               1               E2             0x00
    E2               0               E2             0x00
    E2               1               E3             0x00
    E3               0               E3             0x00
    E3               1               E4             0x00
    E4               0               E4             0x00
    E4               1               E5             0x00
    E5               0               E5             0x00
    E5               1               E6             0x00
    E6               0               E6             0x00
    E6               1               E7             0x00
    E7               0               E7             0x00
    E7               1               E8             0x00
    E8               0               E8             0x00
    E8               1               E9             0x00
 E9_lightRed         0               E9             0x00
 E9_lightRed         1              INIT            0x02
---------------------------------------------------------------

**/

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

bool buttonPressed() {
    if (PIND & 0x04) {
        _delay_ms(10);
        if (PIND & 0x04) {
            return 1;
        }
    }
    return 0;
}

void lightRed() {
    PORTC = 0x02;
}

void lightOff() {
    PORTC = 0x00;
}

void lightRed1s() {
    lightRed();
    _delay_ms(1000);
    lightOff();
}

int main() {
    DDRC = 0xff;  //PORTC -> Out
    DDRD = 0x00;  //PORTD -> In

    enum States {
        INIT,  // light is off
        E1,
        E2,
        E3,
        E4,
        E5,
        E6,
        E7,
        E8,
        E9_lightRed,  //after relased, lightRed -> 1second -> lightOff
    };
    States state = INIT;
    for (;;) {
        switch (state) {
            case INIT:
                if (buttonPressed()) {
                    state = E1;
                }
                break;
            case E1:
                if (!buttonPressed()) {
                    state = E2;
                }
                break;
            case E2:
                if (buttonPressed()) {
                    state = E3;
                }
                break;
            case E3:
                if (!buttonPressed()) {
                    state = E4;
                }
                break;
            case E4:
                if (buttonPressed()) {
                    state = E5;
                }
                break;
            case E5:
                if (!buttonPressed()) {
                    state = E6;
                }
                break;
            case E6:
                if (buttonPressed()) {
                    state = E7;
                }
                break;
            case E7:
                if (!buttonPressed()) {
                    state = E8;
                }
                break;
            case E8:
                if (buttonPressed()) {
                    state = E9_lightRed;
                }
                break;
            case E9_lightRed:
                if (!buttonPressed()) {
                    state = INIT;
                    lightRed1s();
                }
                break;
        }
    }
}