/*
 * Nom: Machine Etat 2.cpp
 * Description: Resolution du probleme 2 du Tp2
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu (1934201)
 * Creation: 04/09/2018
 * Derniere modification : 07/09/2018
 * Configuration : LED libre relie a A0 et A1, fleche (positif) sur connecteur vis-a-vis A0 et LED+

Machine de Morre etat pour filtrer les pressions (anti-rebonds)
	Etat present |	Interrupteur (1=Presse, 0=Relache)	|      Etat suivant	 	|	Sortie bool estPresse
	_____________|______________________________________|_______________________|______________________________________________________
	Relache		 |	0									|	Relache				|	0
	Relache		 |	1									|	Presse				|	
	Presse	  	 |	0									|	Relache				|	1 
	Presse		 |	1									|	Presse				|	

Machine de Moore pour controler allumage de la DEL
	Etats	|     bool estPresse		|     Etat suivant	|	Sortie LED
	________|___________________________|___________________|______________
	Rouge1	|	0						|	Rouge1			|	Rouge
	Rouge1	|	1						|	Ambre			|	
	Ambre	|	0						|	Vert1 			|	Ambre
	Ambre	|	1						|	Ambre			|	
	Vert1	|	0						|	Vert1			|	Vert
	Vert1	|	1						|	Rouge2			|	
	Rouge2	|	0						|	Eteinte			|	Rouge
	Rouge2	|	1						|	Rouge2			|	
	Eteinte	|	0						|	Eteinte			|	Eteinte
	Eteinte	|	1						|	Vert2			|	
	Vert2	|	0						|	Rouge1			|	Vert
	Vert2	|	1						|	Vert2			|	

 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

ISR(ADC_vect)
{
	// user code here
}

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

enum EtatsInterrupteur
{
	Relache,
	Presse
};

volatile EtatsInterrupteur etatInterrupteur = Relache;
volatile bool estPresse = false;			 // Variable de sortie de la premiere machine et d'entree de la deuxieme machine
volatile uint8_t alternateurAmbre = 0x01; // Utilise pour creer la couleur ambre en alternant entre 0x01 et 0xfe


int main()
{
	DDRA = 0xff;
	DDRD = 0x00;

	
	for (;;)
	{
		// 1re machine (Moore) qui determine la position de l'interrupteur (avec anti-rebond)
		switch (etatInterrupteur)
		{
		case Relache:
			estPresse = false;
			if (PIND & 0x04)
			{
				_delay_ms(10);
				if (PIND & 0x04)
				{
					etatInterrupteur = Presse;
				}
			}
			else
			{
				etatInterrupteur = Relache;
			}
			break;

		case Presse:
			estPresse = true;
			if (PIND & 0x04)
			{ // Pas besoin d'anti-rebond puisque deja presse avant d'arriver ici
				etatInterrupteur = Presse;
			}
			else
			{
				etatInterrupteur = Relache;
			}
		}
		// 2eme Machine (moore) qui gere la LED selon l'etat de l'interrupteur
		switch (etatLED)
		{
		case Rouge1:
			PORTA = 0x01; // Rouge
			if (estPresse)
			{
				etatLED = Ambre;
			}
			else
			{
				etatLED = Rouge1;
			}
			break;

		case Ambre:
			alternateurAmbre = ~alternateurAmbre; // Production de la couleur ambre en alternant tant qu'on reste en Ambre
			PORTA = alternateurAmbre;
			if (estPresse)
			{
				etatLED = Ambre;
			}
			else
			{
				etatLED = Vert1;
			}
			break;

		case Vert1:
			PORTA = 0x02; // Vert
			if (estPresse)
			{
				etatLED = Rouge2;
			}
			else
			{
				etatLED = Vert1;
			}
			break;

		case Rouge2:
			PORTA = 0x01; // Rouge
			if (estPresse)
			{
				etatLED = Rouge2;
			}
			else
			{
				etatLED = Eteinte;
			}
			break;

		case Eteinte:
			PORTA = 0x00; // Eteinte
			if (estPresse)
			{
				etatLED = Vert2;
			}
			else
			{
				etatLED = Eteinte;
			}
			break;

		case Vert2:
			PORTA = 0x02; // Vert
			if (estPresse)
			{
				etatLED = Vert2;
			}
			else
			{
				etatLED = Rouge1; // Retour a l'etat initial
			}
		}
	}
}
