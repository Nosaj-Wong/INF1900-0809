/**
* Programme qui change la couleur du LED quand on appuie et/ou relache le bouton. Bourne positive du LED est branchee sur PIN2 du PORTC
* \file exo#3.cpp
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
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void initialisation(void) {
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'être interrompu alors que
    // le microcontroleur n'est pas prêt...
    cli();
    // configurer et choisir les ports pour les entrées
    // et les sorties. DDRx... Initialisez bien vos variables
    DDRC = 0xff;  //PORTC -> Out
    DDRD = 0x00;  //PORTD -> In
    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes
    EIMSK |= (1 << INT0);
    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA |= (1 << ISC00);
    // sei permet de recevoir à nouveau des interruptions.
    sei();
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

volatile uint8_t state = 0;
// placer le bon type de signal d'interruption
// à prendre en charge en argument
ISR(INT0_vect) {
    // laisser un delai avant de confirmer la réponse du
    // bouton-poussoir: environ 30 ms (anti-rebond)
    _delay_ms(30);
    // se souvenir ici si le bouton est pressé ou relâché
    // changements d'états tels que ceux de la
    // semaine précédente
    if (state == 5) {
        state = 0;
    } else {
        state++;
    }

    // Voir la note plus bas pour comprendre cette instruction et son rôle
    EIFR |= (1 << INTF0);
}

int main() {
    initialisation();

    unsigned long compteur = 0;

    for (;;) {
        compteur++;
        switch (state) {
            case 0:
                lightRed();

                break;
            case 1:
                lightAmbre(compteur);

                break;
            case 2:
                lightGreen();

                break;
            case 3:
                lightRed();

                break;
            case 4:
                lightOff();

                break;
            case 5:
                lightGreen();

                break;
        }
    }
}