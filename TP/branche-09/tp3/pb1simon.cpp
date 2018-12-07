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
	PORTD = 0x00;

	bool pressed = false;
	bool released = false;

	enum EtatsA
	{
		INIT,
		etat1,
		etat2
	};

	enum EtatsB
	{
		ETEINTE,
		ROUGE,
		PWM
	};

	EtatsA etatsA = INIT;
	EtatsB etatsB = ETEINTE;

	for (;;) // boucle sans fin
	{
		switch (etatsA)
		{
		case INIT: // not pressed
			PORTD = 0x00;
			pressed = false;
			if (PIND & 0x04)   // D2
				_delay_ms(10); // antirebond
			if (PIND & 0x04)   // if button pressed
				etatsA = etat1;
			else
				etatsA = INIT;
			break;  // sort de la boucle "if" et passe a la deuxieme machine
		case etat1: // button pressed
			pressed = true;
			PORTD = 0x01;
			if (PIND & 0x04) // if button still pressed
			{
				etatsA = etat2;
			}
			else
			{
				etatsA = INIT;   // go back to INIT
				released = true; // button released
			}
			break;  // sort de la boucle "if" et passe a la deuxieme machine
		case etat2: // if button release, goes to INIT
			pressed = false;
			if (PIND & 0x04) // if button still pressed
			{
				etatsA = etat2;
			}
			else
			{
				etatsA = INIT;   // button released
				released = true; // go back to INIT
			}
			break; // sort de la boucle "if" et passe a la deuxieme machine
		}
		// Deuxieme machine: PWM
		// boucle de 100 x 30 (delay)
		switch (etatsB)
		{
		case ETEINTE:
			PORTA = 0x00;
			if (pressed)
				etatsB = ROUGE;
			else
				etatsB = ETEINTE;
			break; // sort de la boucle "if" et passe a la deuxieme machine

		case ROUGE:
			PORTA = 0x01;
			if (pressed)
				etatsB = ROUGE;
			if (released)
				etatsB = PWM;
			break; // sort de la boucle "if" et passe a la deuxieme machine
		case PWM:
			int a = 100;
			int b = 100;
			for (int i = 0; i < 100; i++) // 100 levels
			{
				for (int i = 0; i < 30; i++) // 100*30 = 3000
				{
					PORTD = 0x01; // ROUGE
					for (int i = 0; i < a; i++) // 3000us
					{
						_delay_us(10); //delay ROUGE
					}
					PORTD = 0x00; // OFF
					for (int i = 0; i < (b - a); i++)
					{
						_delay_us(10); // delay OFF
					}
				}
				a--;
			}
			etatsB = ETEINTE;
			break; // sort de la boucle "if" et passe a la deuxieme machine
		}
	}
	return 0;
}
