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


int main()
{
	DDRA = 0xff;
	DDRD = 0x00;

	PORTA = 0x01;
	Memoire24CXXX accesMemoire;
	unsigned char messageEcrit[45] = {'*', 'P', '*', 'O', '*', 'L', '*', 'Y', '*', 'T', '*', 'E', '*', 'C', '*', 'H', '*', 'N', '*', 'I', '*', 'Q', '*', 'U', '*', 'E', '*',
									  '*', 'M', '*', 'O', '*', 'N', '*', 'T', '*', 'R', '*', 'E', '*', 'A', '*', 'L', '*', 0x00};
	unsigned char messageRecu[45] = {};
	bool estPareil = true;
	for (;;)
	{
		if (PIND & 0x04)
		{
			PORTA = 0x00;
			_delay_ms(30);
			if (PIND & 0x04)
			{
				accesMemoire.ecriture(0x00, messageEcrit, sizeof(messageEcrit));
				_delay_ms(5);
				accesMemoire.lecture(0x00, messageRecu, sizeof(messageRecu));
			}
			for (unsigned i = 0; i < 45 && estPareil; i++)
			{
				if (messageRecu[i] != messageEcrit[i])
					estPareil = false;
			}
			if (estPareil)
				PORTA = 0x02;
			else
				PORTA = 0x01;
		}
	}
	return 0;
}
