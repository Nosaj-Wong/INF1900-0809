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

void ajustementPWM(uint16_t pwm) // 8 000 000/8= 1 000 000 tour d'horloge/s
{

	// mise à un des sorties OC1A et OC1B sur comparaison
	// réussie en 

	// page 177 de la description technique du ATmega324PA)

	// Set OCnA/OCnB on Compare Match when upcounting.
	// Clear OCnA/OCnB on Compare Match.
	OCR1A = pwm; //sortie A PWM 
	OCR1B = pwm; //sortie B PWM

	/
	
	//mode PWM 8 bits, phase correcte et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
	TCCR1A |= (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0) |(1 << WGM10);

	// division d'horloge par 8 - implique une frequence de PWM fixe
	TCCR1B |= (1 << CS11) ; // p.130

	TCCR1C = 0;
}

int main()
{
	DDRD = 0x30; // PORT D est en mode entree
				// fait en sorte que certains ports ne sont plus etre accessibles en sortie
	ajustementPWM(255);
	_delay_ms(2000);
	ajustementPWM(192);
	_delay_ms(2000);
	ajustementPWM(128);
	_delay_ms(2000);
	ajustementPWM(64);
	_delay_ms(2000);
	ajustementPWM(0);

	return 0;
}
