/*
 * Nom: Probleme 2
 * Description: Ceci est un programme est jeu de réflexe avec l'utilisation d'une minuterie
 * Version: 1.1
 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t minuterieExpiree;
volatile uint8_t boutonPoussoir;

ISR(TIMER1_COMPA_vect) // comparator A of counter 1 (16 bits)
{
	minuterieExpiree = 1;
}

ISR(INT0_vect)
{
	// anti-rebond
	_delay_ms(30);
	boutonPoussoir = 1;

	EIFR |= (1 << INTF0); // clear le flag lu au bouton
}

//Compte jusqu'a 65 535
//8000000/1024 = 7812.5 tour d'horloge = 1 seconde
//65 535/7812.5 = 8.38848 secondes
//Donc, compte jusqu'a 8 secondes
void partirMinuterie(uint16_t duree) // 1000ms
{
	minuterieExpiree = 0;

	// mode CTC du timer 1 avec horloge divisée par 1024
	// interruption après la durée spécifiée

	TCNT1 = 0; // Timer/Counter1  initialement a zero

	OCR1A = duree; // comparaison avec TCNT1

	// Timer/Counter 1 Control Register A
	// Set OCnA/OCnB on Compare Match (Set output to high level).
	TCCR1A |= (1 << COM1A1) | (1 << COM1A0); // le met a 1

	// Timer/Counter 1 Control Register B
	// clk_{I/O}/1024 (From prescaler)
	TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
	; // Timer/Counter 1 Control Register B

	TCCR1C = 0;
	// Timer/Counter1 Interrupt Mask Register
	// Output Compare A Match Interrupt Enable
	TIMSK1 |= (1 << OCIE1A); // laisse passer l'interruption de A
}

void initialisation(void)
{
	// cli est une routine qui bloque toutes les interruptions.
	// Il serait bien mauvais d'être interrompu alors que le microcontroleur n'est pas prêt...
	cli();

	// configurer et choisir les ports pour les entrées
	// et les sorties. DDRx... Initialisez bien vos variables
	DDRA = 0xff; // PORT A est en mode sortie
	DDRD = 0x00; // PORT D est en mode entree

	// cette procédure ajuste le registre EIMSK
	// de l’ATmega324PA pour permettre les interruptions externes
	EIMSK |= (1 << INT0); // Qu'est ce que fait EIMSK????

	// il faut sensibiliser les interruptions externes aux
	// changements de niveau du bouton-poussoir
	// en ajustant le registre EICRA
	EICRA |= (1 << ISC01) | (1 < ISC00); // 0000 0011 // The rising edge of INTn generates asynchronously an interrupt request

	// sei permet de recevoir à nouveau des interruptions.
	sei();
}

void ledClignote()
{
	PORTA = 0x01;
	_delay_ms(100);
	PORTA = 0x00;
}

int main()
{
	initialisation();

	// cli est une routine qui bloque toutes les interruptions.
	cli();

	// attend 10 secondes
	PORTA = 0x00;
	_delay_ms(10000);

	// LED clignote pendant 0.1s
	ledClignote();

	// sei permet de recevoir à nouveau des interruptions.
	sei();

	partirMinuterie(7812);

	do
	{

		// attendre qu'une des deux variables soit modifiée par une ou l'autre des interruptions.

	} while (minuterieExpiree == 0 && boutonPoussoir == 0);

	// Une interruption s'est produite. Arrêter toute forme d'interruption. Une seule réponse suffit.

	cli();

	// Verifier la réponse

	if (boutonPoussoir)
		PORTA = 0x02;
	else if (minuterieExpiree == 1)
		PORTA = 0x01;
	return 0;
}
