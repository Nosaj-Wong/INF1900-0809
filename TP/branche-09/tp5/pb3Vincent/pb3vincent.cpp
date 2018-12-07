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

#include "memoire_24.h"

void initialisationUART(void)
{

	// 2400 bauds. Nous vous donnons la valeur des deux

	// premier registres pour vous éviter des complications

	UBRR0H = 0;

	UBRR0L = 0xCF;

	// permettre la reception et la transmission par le UART0

	UCSR0A = 0;

	UCSR0B = 1 << RXEN0 | 1 << TXEN0; //Enable la transmission et la reception (Rx,Tx) (p.186)

	// Format des trames: 8 bits, 1 stop bits, none parity

	UCSR0C = 1 << UCSZ01 | 1 << UCSZ00; // mode 8 bit, pas de parity, 1 stop bit (p.188) (deja comme ca de base)
}

// De l'USART vers le PC

void transmissionUART(uint8_t donnee)
{

	while (!(UCSR0A & (1 << UDRE0))) // Attendre que le buffer d'envoi soit vide
		;
	UDR0 = donnee; //Envoyer la donnee
}

int main()
{
	initialisationUART();

	Memoire24CXXX accesMemoire;
	unsigned char messageEcrit[45] = {'*', 'P', '*', 'O', '*', 'L', '*', 'Y', '*', 'T', '*', 'E', '*', 'C', '*', 'H', '*', 'N', '*', 'I', '*', 'Q', '*', 'U', '*', 'E', '*',
									  '*', 'M', '*', 'O', '*', 'N', '*', 'T', '*', 'R', '*', 'E', '*','A', '*', 'L', '*',0xff};

	accesMemoire.ecriture(0x00, messageEcrit, sizeof(messageEcrit));
	_delay_ms(5);

	uint8_t adresseActuelle = 0x00;
	uint8_t valeurEnCours = 0;

	bool aRencontreCaractereDeFin = false;

	while (!aRencontreCaractereDeFin)
	{
		accesMemoire.lecture(adresseActuelle, &valeurEnCours);
		_delay_ms(5);
		if (valeurEnCours == 0xff)
			aRencontreCaractereDeFin = true;
		else
		{
			transmissionUART(valeurEnCours);
			adresseActuelle++;
		}
	}

	return 0;
}
