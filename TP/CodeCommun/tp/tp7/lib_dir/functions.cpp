/*
** file: functions.cpp
** Implementation de diverses fonctions utiles
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

#include "functions.h"

// Fonction qui cree un delais de 10us
void _my_delay_10us(unsigned long us) {
    for (unsigned long i = 0; i < us; i++) {
        _delay_us(8);
    }
}

// Fonction qui produit un signal PWM logiciel sur le port C0
void PWM_software(int percentage, double duration, double frequency) {
    const double periode = 1 / frequency;
    const double a = percentage / 100.0 * periode;
    const int nbRepetition = duration / periode;
    const long int nb10usDans1s = 1000000 / 10;

    for (int i = 0; i < nbRepetition; i++) {
        PORTC |= 0b11;
        _my_delay_10us(a * nb10usDans1s);
        PORTC &= ~0b1;
        _my_delay_10us((periode - a) * nb10usDans1s);
    }
}

// Fonction qui met le bouton-pressoir usager en mode interruption
// pour en declencher lors d'une variation du edge choisie
void initializeButtonInterrupt(Edge edge) {
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'être interrompu alors que
    // le microcontroleur n'est pas prêt...
    cli ();

    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes
    EIMSK |= (1 << INT0);

    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir choisi
    // en ajustant le registre EICRA
    EICRA &= ~0x03;  // remise a zero des deux premier bits
    EICRA |= edge;

    // sei permet de recevoir à nouveau des interruptions.
    sei ();
}

// Fonction qui verifie si le bouton-pressoir usager est actuellement presse
bool buttonPressed() {
    return (PIND & 0x04);
}

// Fonction qui verifie si le bouton-pressoir usager est actuellement presse en plus d'un anti-rebond
bool debounceButtonPress() {
    if (buttonPressed()) {
        _delay_ms(30);
        if (buttonPressed()) {
            return true;
        }
    }
    return false;
}