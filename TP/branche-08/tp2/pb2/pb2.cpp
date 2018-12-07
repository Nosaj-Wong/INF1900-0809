/**
* Programme qui change la couleur du LED quand on appuie et/ou relache le bouton. Bourne positive du LED est branchee sur PIN2 du PORTC
* \file pb2.cpp
* \authors Facundo GIANNOTTI 1882989 et Xi Chen SHEN 1930285
* \date 4 septembre 2018

Entree: DDRD, verifiee par la fonction buttonPressed
Sortie: DDRC
 
 Table 1: Tableau des états
 ------------------------------------------------------------------------------------------------------- 
 Etat present    Entree DDRD     Etat suivant    Sortie DDRC       
    E0_Red           0              E0_Red           0x02               
    E0_Red           1             E1_Ambre         Ambre(Sortie alternee entre 0x00 0x01 0x02 0x03)                   
   E1_Ambre          1             E1_Ambre         Ambre(Sortie alternee entre 0x00 0x01 0x02 0x03)                
   E1_Ambre          0             E2_Green          0x01               
   E2_Green          0             E2_Green          0x01    
   E2_Green          1              E3_Red           0x02               
    E3_Red           1              E3_Red           0x02           
    E3_Red           0              E4_Off           0x00                       
    E4_Off           0              E4_Off           0x00                                       
    E4_Off           1             E5_Green          0x01                               
   E5_Green          1             E5_Green          0x01                   
   E5_Green          0              E0_Red           0x02                                   
--------------------------------------------------------------------------------------------------------
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

void lightGreen() {
    PORTC = 0x01;
}

void lightRed() {
    PORTC = 0x02;
}

void lightAmbre(unsigned long compteur) {
    PORTC = compteur;
}

void lightOff() {
    PORTC = 0x00;
}

int main() {
    DDRC = 0xff;  //PORTC -> Out
    DDRD = 0x00;  //PORTD -> In

    unsigned long compteur = 0;

    enum States {
        E0_Red,    //INIT, before pressed
        E1_Ambre,  //after 1st pressed
        E2_Green,  //after 1st released
        E3_Red,    //after 2nd pressed
        E4_Off,    //after 2nd released
        E5_Green,  //after 3rd pressed, return to INIT
    };
    States state = E0_Red;
    for (;;) {
        compteur++;
        switch (state) {
            case E0_Red:
                lightRed();
                if (buttonPressed()) {
                    state = E1_Ambre;
                }
                break;
            case E1_Ambre:
                lightAmbre(compteur);
                if (!buttonPressed()) {
                    state = E2_Green;
                }
                break;
            case E2_Green:
                lightGreen();
                if (buttonPressed()) {
                    state = E3_Red;
                }
                break;
            case E3_Red:
                lightRed();
                if (!buttonPressed()) {
                    state = E4_Off;
                }
                break;
            case E4_Off:
                lightOff();
                if (buttonPressed()) {
                    state = E5_Green;
                }
                break;
            case E5_Green:
                lightGreen();
                if (!buttonPressed()) {
                    state = E0_Red;
                }
                break;
        }
    }
}