/*
 * Nom: Probleme 2
 * Description: Ceci est un programme ayant des compteurs qui controle une DEL.
 * Version: 1.1
 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

int main()
{

	DDRA = 0xff; // PORT A est en mode sortie
	DDRD = 0x00; // PORT D est en mode entree

	uint8_t alternateur = 0x01;
	enum EtatsA
	{
		INIT,
		etat1
	};
	enum EtatsB
	{
		ROUGE1,
		AMBRE,
		VERT1,
		ROUGE2,
		OFF,
		VERT2
	};

	EtatsA etatsA = INIT;
	EtatsB etatsB = ROUGE1;

	// PIND: Lit "Entree D" (interrupteur)
	// PORTA: Ecrit sur "Sortie A"
	bool pressed = false;
	// 1. Machine de Moore sortant A, l'entree de la prochaine machine.

	for (;;)
	{

		switch (etatsA)
		{
		case INIT:
			pressed = false;
			if (PIND & 0x04)   // D2
				_delay_ms(10); // antirebond
			if (PIND & 0x04)
				etatsA = etat1;
			else
				etatsA = INIT;
			break; // sort de la boucle "if" et passe a la deuxieme machine
		case etat1:
			pressed = true;
			if (PIND & 0x04)
			{
				etatsA = etat1;
			}
			else
			{
				etatsA = INIT;
			}
			break; // sort de la boucle "if" et passe a la deuxieme machine
		}
		// 2. Machine de Moore avec entree PIND et sortie Z.
		switch (etatsB)
		{
		case ROUGE1:
			PORTA = 0x01;
			if (pressed)
				etatsB = AMBRE;
			else
				etatsB = ROUGE1;
			break; // sort de la boucle "if" et passe a la deuxieme machine

		case AMBRE:
			if (pressed)
			{
				alternateur = ~alternateur;
				PORTA = alternateur;
				etatsB = AMBRE;
			}
			else
			{
				etatsB = VERT1;
			}
			break; // sort de la boucle "if" et passe a la deuxieme machine

		case VERT1:
			PORTA = 0x02;
			if (pressed)
				etatsB = ROUGE2;
			else
				etatsB = VERT1;
			break; // sort de la boucle "if" et passe a la deuxieme machine

		case ROUGE2:
			PORTA = 0x01;
			if (pressed)
				etatsB = ROUGE2;
			else
				etatsB = OFF;
			break; // sort de la boucle "if" et passe a la deuxieme machine

		case OFF:
			PORTA = 0x00;
			if (pressed)
				etatsB = VERT2;
			else
				etatsB = OFF;
			break; // sort de la boucle "if" et passe a la deuxieme machine

		case VERT2:
			PORTA = 0x02;
			if (pressed)
				etatsB = VERT2;
			else
				etatsB = ROUGE1;
			break; // sort de la boucle "if" et passe a la deuxieme machine
		}
	}
	return 0;
}
