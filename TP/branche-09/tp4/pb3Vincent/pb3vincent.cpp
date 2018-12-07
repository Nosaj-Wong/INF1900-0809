/*
 * Nom: pb3vincent.cpp
 * Description: Ceci est un programme mobilisant une roue avec deux frequences de PWN et 5 niveau d'intensite de facon cyclique avec la minuterie
 * Version: 1.0
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu
 * Date de creation: 21/09/2018
 * Date de modification: 21/09/2018
 * Configuration : Moteur Enable relie a D0 et moteur dir relie a D1. 9V fourni au moteur et 5V a carte mere
 *    (optionnel): LED + relie a D8
 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>


// placer le bon type de signal d'interruption

// à prendre en charge en argument

void ajustementPWM(uint8_t quartDePuissance)
{

    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
    // page 177 de la description technique du ATmega324PA)

    //Mode 8 bit donc max=255

    TCNT1=0; //Initisation de la minuterie a 0

    if (quartDePuissance == 0)
    {
        OCR1A = 255;
        OCR1B = 255;
    }
    
    else
    {
        OCR1A = 256 - 64 * quartDePuissance;

        OCR1B = 256 - 64 * quartDePuissance;
    }

    // division d'horloge par 8 - implique une frequence de PWM fixe

    TCCR1A = 1 << COM1A1 | 1 << COM1A0 | 1 << COM1B1 | 1 << COM1B0 | 1 << WGM10; // Mode 8 bits phase correcte (avec la partie sur le B) (p.130)
                                                                                 // Et mode set en montant et clear en descendant

    TCCR1B = 0 << CS12 | 1 << CS11 | 0 << CS10; // Definition de la clock a 1/8 (p.130)

    TCCR1C = 0;
}

void initialisation(void)
{

    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'être interrompu alors que
    // le microcontroleur n'est pas prêt...

    cli();

    // configurer et choisir les ports pour les entrées
    // et les sorties. DDRx... Initialisez bien vos variables

    DDRD = 0x30; //D2 est l'interrupteur donc entree, D4-5 font le PWM donc sortie

    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes

    //EIMSK |= (1 << INT0);

    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir
    // en ajustant le registre EICRA

    // sei permet de recevoir à nouveau des interruptions.

    sei();
}

int main()
{
    initialisation();
    for (;;)
    {
        uint8_t quartDePuissance = 0;
        if (PIND & 0x04)
        {
            for (uint8_t i = 0; i < 5; i++)
            {
                ajustementPWM(quartDePuissance);
                _delay_ms(2000);
                quartDePuissance += 1;
            }
            ajustementPWM(0);
        }
    }
}
