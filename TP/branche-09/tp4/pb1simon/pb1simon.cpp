/*
 * Nom: Probleme 2
 * Description: Ceci est un programme ayant des compteurs qui controle une DEL.
 * Version: 1.1
 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

enum EtatsB
{
	ROUGE1,
	AMBRE,
	VERT1,
	ROUGE2,
	OFF,
	VERT2
};
volatile EtatsB etatsB = ROUGE1;

void initialisation(void)
{
	// cli est une routine qui bloque toutes les interruptions.
	// Il serait bien mauvais d'être interrompu alors que le microcontroleur n'est pas prêt...
	cli();

	// configurer et choisir les ports pour les entrées
	// et les sorties. DDRx... Initialisez bien vos variables
	DDRA = 0xff; // PORT A est en mode sortie (LED)
	DDRD = 0x00; // PORT D est en mode entree (interrupteur)

	// cette procédure ajuste le registre EIMSK
	// de l’ATmega324PA pour permettre les interruptions externes
	EIMSK |= (1 << INT0);

	// il faut sensibiliser les interruptions externes aux
	// changements de niveau du bouton-poussoir
	// en ajustant le registre EICRA
	EICRA |= (1 << ISC00); // 0000 0001 // Any edge of INT0 generates asynchronously an interrupt request

	// sei permet de recevoir à nouveau des interruptions.
	sei();
}

ISR(INT0_vect)
{
	// laisser un delai avant de confirmer la réponse du
	// bouton-poussoir: enviromakn 30 ms (anti-rebond)
	//_delay_loop_ms(30);

	// se souvenir ici si le bouton est pressé ou relâché

	// changements d'états tels que ceux de la
	// semaine précédente
	switch (etatsB)
	{
	case ROUGE1:
		_delay_ms(30);
		if (PIND & 0x04)
			etatsB = AMBRE;
		break; // sort de la boucle "if" et passe a la deuxieme machine

	case AMBRE:
		etatsB = VERT1;
		break; // sort de la boucle "if" et passe a la deuxieme machine

	case VERT1:
		_delay_ms(30);
		if (PIND & 0x04)
			etatsB = ROUGE2;
		break; // sort de la boucle "if" et passe a la deuxieme machine

	case ROUGE2:
		etatsB = OFF;
		break; // sort de la boucle "if" et passe a la deuxieme machine

	case OFF:
		_delay_ms(30);
		if (PIND & 0x04)
			etatsB = VERT2;
		break; // sort de la boucle "if" et passe a la deuxieme machine

	case VERT2:

		etatsB = ROUGE1;
		break; // sort de la boucle "if" et passe a la deuxieme machine
	}

	// Voir la note plus bas pour comprendre cette instruction et son rôle

	EIFR |= (1 << INTF0); // clear le flag lu au bouton
}

int main()
{
	uint8_t alternateur = 0x01;
	initialisation();

	for (;;)
	{
		switch (etatsB)
		{
		case ROUGE1: // LED demarre en s'allumant ROUGE
			PORTA = 0x01;
			break; // sort de la boucle "if" et passe a la deuxieme machine

		case AMBRE: // PRESSED
		{
			alternateur = ~alternateur;
			PORTA = alternateur;
		}
		break; // sort de la boucle "if" et passe a la deuxieme machine

		case VERT1:		  // RELEASED
			PORTA = 0x02; // LED
			break;		  // sort de la boucle "if" et passe a la deuxieme machine

		case ROUGE2: // PRESSED
			PORTA = 0x01;
			break; // sort de la boucle "if" et passe a la deuxieme machine

		case OFF: // RELEASED
			PORTA = 0x00;
			break; // sort de la boucle "if" et passe a la deuxieme machine

		case VERT2: // PRESSED

			PORTA = 0x02;
			break; // sort de la boucle "if" et passe a la deuxieme machine
		}
	}
	return 0;
}
