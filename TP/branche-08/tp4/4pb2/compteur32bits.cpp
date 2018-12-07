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

volatile uint8_t minuterieExpiree;
volatile uint8_t boutonPoussoir;

void lightGreen() {
    PORTC = 0x01;
}
void lightRed() {
    PORTC = 0x02;
}
void lightOff() {
    PORTC = 0x00;
}

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

void partirMinuterie(uint16_t duree) {
    minuterieExpiree = 0;
    // mode CTC du timer 1 avec horloge divisée par 1024
    // interruption après la durée spécifiée
    TCNT1 = 0;                                // Start with 0
    OCR1A = duree;                            // 8000000/256 = 31250
    TCCR1A |= (1 << COM1A1) | (1 << COM1A0);  //Set OCnA/OCnB on Compare Match (Set output to high level).
    TCCR1B |= (1 << CS12);                    // mode 100, clkI/O 1/256 (From prescaler)
    TCCR1C = 0;
    TIMSK1 |= (1 << OCIE1A);  // Bit 1 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
}

ISR(INT0_vect) {
    boutonPoussoir = 1;
    // anti-rebond
    _delay_ms(30);  // unnecessary

    // cas si un second signal d'interruption arrive durant l'exécution de ISR ... on empeche ca!
    EIFR |= (1 << INTF0);
}

ISR(TIMER1_COMPA_vect) {
    minuterieExpiree = 1;
}

int main() {
    initialisation();

    _delay_ms(2000);

    lightRed();

    _delay_ms(100);
    lightOff();
    boutonPoussoir=0;
    
    partirMinuterie(31250);

    do {
        // attendre qu'une des deux variables soit modifiée
        // par une ou l'autre des interruptions.
    } while (minuterieExpiree == 0 && boutonPoussoir == 0);
    // Une interruption s'est produite. Arrêter toute
    // forme d'interruption. Une seule réponse suffit.
    cli();

    // Verifier la réponse
    if (boutonPoussoir == 1) {
        while (boutonPoussoir == 1) {
            lightGreen();
            _delay_ms(10);
        }
    } else if (minuterieExpiree == 1) {
        while (minuterieExpiree == 1) {
            lightRed();
            _delay_ms(10);
        }
    }
}
