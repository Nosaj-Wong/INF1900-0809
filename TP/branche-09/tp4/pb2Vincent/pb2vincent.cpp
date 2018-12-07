/*
 * Nom: pb2vincent.cpp
 * Description: Resolution du probleme 2 du tp4
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu (1934201)
 * Creation: 18/09/2018
 * Derniere modification : 18/09/2018
 * Configuration : LED libre relie a A0 et A1, fleche (positif) sur connecteur vis-a-vis A0 et LED+

 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint8_t minuterieExpiree;
volatile uint8_t boutonPoussoir;

// placer le bon type de signal d'interruption

// à prendre en charge en argument

ISR(INT0_vect) //Toujours mettre nomDuVecteur_vect
{
    _delay_ms(30);
    boutonPoussoir = 1;
    // laisser un delai avant de confirmer la réponse du
    // bouton-poussoir: environ 30 ms (anti-rebond) (juste pour les pressions)

    // Voir la note plus bas pour comprendre cette instruction et son rôle (clear le flag lie au bouton)
    EIFR |= (1 << INTF0);
}
ISR(TIMER1_COMPA_vect) //Toujours mettre nomDuVecteur_vect, ici comparateur A du compteur 1 (16 bits)
{
    minuterieExpiree = 1;
    //Pas de clear du flag parcequ'on veut que ca sonne une seule fois
}

void initialisation(void)
{

    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'être interrompu alors que
    // le microcontroleur n'est pas prêt...

    cli();

    // configurer et choisir les ports pour les entrées
    // et les sorties. DDRx... Initialisez bien vos variables

    DDRA = 0xff;
    DDRD = 0x00;

    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes

    EIMSK |= (1 << INT0);

    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir
    // en ajustant le registre EICRA

    EICRA |= (1 << ISC01) | (1 << ISC00); //Activation de l'interrupt lors du edge montrant seulement (p.67)

    // sei permet de recevoir à nouveau des interruptions.

    sei();
}

void partirMinuterie_centiemeSeconde(uint16_t duree)
{
    //Programmer la minuterie pour calculer 1 secondes avant d'interrompre
    TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10); // Diminution de la clock a 1/1024 (p.131)
    TCCR1A |= (1 << COM1A1) | (1 << COM1A0);           // OC3A mis sur "compare match" (p.128)
    TCNT1 = 0;                                         //Commencer le compteur a 0
    OCR1A = 78 * duree;                                //Equivalent de 1 centieme de sec a un freqence de 7800 Hz (8 000 000 / 1024)
    TCCR1C = 0;                                        //Pas de force output compare (p.131)
    TIMSK1 = (1 << OCIE1A);                            // Enable l'interrupt pour compare A (p.134)
}

int main()
{
    initialisation();

    //Empechons les interruptions pendant les 10 premieres secondes (inutiles)
    cli();
    _delay_ms(10000);
    PORTA = 0x01;
    _delay_ms(100);
    PORTA = 0x00;
    sei();
    //Permettons les interruptions apres l'indication du debut du defi
    partirMinuterie_centiemeSeconde(5);

    do
    {
        //Attente inutile
    } while (minuterieExpiree == 0 && boutonPoussoir == 0);

    // On veut pu d'interruption, le premier des deux est suffisant
    if (minuterieExpiree)
        PORTA = 0x01;
    else if (boutonPoussoir)
        PORTA = 0x02;
}
