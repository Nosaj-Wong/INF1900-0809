/*
 * Nom: Interrupteur
 * Description: Ceci est un programme testant l'interupteur.
 * Version: 1.0
 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

int main()
{
	DDRD = 0x03; // PORT D est en mode sortie
	uint8_t alternateur = 0x01;
	for (;;) // boucle sans fin
	{
		if (PIND & 0x04)
		{
			_delay_ms(10);
			if (PIND & 0x04)
			{
				alternateur = ~alternateur;
				PORTD = alternateur;
			}
		}
		else
		{
			PORTD = 0x00;
		}
	}
	return 0;
}
