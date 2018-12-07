/*
 * Nom: pb1vincent.cpp
 * Description: Resolution du probleme 1 du tp4
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu (1934201)
 * Creation: 18/09/2018
 * Derniere modification : 18/09/2018
 * Configuration : LED libre relie a A0 et A1, fleche (positif) sur connecteur vis-a-vis A0 et LED+

 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

enum EtatsLed
{
	Rouge1,
	Ambre,
	Vert1,
	Rouge2,
	Eteinte,
	Vert2
};

volatile EtatsLed etatLED = Rouge1;

// placer le bon type de signal d'interruption

// à prendre en charge en argument

ISR(INT0_vect) //Toujours mettre nomDuVecteur_vect
{

	// laisser un delai avant de confirmer la réponse du
	// bouton-poussoir: environ 30 ms (anti-rebond) (juste pour les pressions)

	switch (etatLED)
	{
	case Rouge1:
		_delay_ms(30); 

		if (PIND & 0x04)
			etatLED = Ambre;
		break;

	case Ambre:
		etatLED = Vert1;
		break;

	case Vert1:
		_delay_ms(30);

		if (PIND & 0x04)
			etatLED = Rouge2;
		break;

	case Rouge2:
		etatLED = Eteinte;
		break;

	case Eteinte:
		_delay_ms(30);

		if (PIND & 0x04)
			etatLED = Vert2;
		break;

	case Vert2:
		etatLED = Rouge1;
		break;
	}

	// Voir la note plus bas pour comprendre cette instruction et son rôle (clear le flag lie au bouton)

	EIFR |= (1 << INTF0); 
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

	EICRA |= (0 << ISC01) | (1 << ISC00); //Activation de l'interrupt lors du edge montrant ET descendant (p.67)

	// sei permet de recevoir à nouveau des interruptions.

	sei();
}

int main()
{
	initialisation();
	uint8_t alternateurAmbre = 0x01; // Utilise pour creer la couleur ambre en alternant entre 0x01 et 0xfe

	for (;;)
	{

		switch (etatLED)
		{
		case Rouge1:
			PORTA = 0x01; // Rouge
			break;

		case Ambre:
			alternateurAmbre = ~alternateurAmbre; // Production de la couleur ambre en alternant tant qu'on reste en Ambre
			PORTA = alternateurAmbre;
			break;

		case Vert1:
			PORTA = 0x02; // Vert
			break;

		case Rouge2:
			PORTA = 0x01; // Rouge
			break;

		case Eteinte:
			PORTA = 0x00; // Eteinte
			break;

		case Vert2:
			PORTA = 0x02; // Vert
			break;
		}
	}
}
