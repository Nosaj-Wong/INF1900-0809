/*
 * Nom: Probleme 1
 * Description: Ceci est un programme ayant des compteurs qui controle une DEL.
 * Version: 1.1
 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h> 

int main()
{
	
  DDRA = 0xff; // PORT A est en mode entree
  DDRD = 0x00; // PORT D est en mode entree
  
  PORTA = 0x00; // LED eteinte au depart

 enum EtatsA {INIT, etat1, etat2};
 enum EtatsB {etat3, etat4, etat5, etat6, etat7, etat8};
 
 EtatsA etatsA = INIT;
 EtatsB etatsB = etat3;
 
 // PIND: Lit "Entree D" (interrupteur)
 // PORTA: Ecrit sur "Sortie A"
 bool pressed = false;
 bool released = false;
 // 1. Machine de Moore sortant A, l'entree de la prochaine machine.
  
 for (;;)
 {
	switch (etatsA)
	{
		case INIT :
			pressed = false;
			if(PIND & 0x04) // D2
				_delay_ms(10); // antirebond
				if(PIND & 0x04)
				etatsA = etat1;
			else etatsA = INIT;
				break; // sort de la boucle "if" et passe a la deuxieme machine			
		case etat1 :
			pressed = true;
			if(PIND & 0x04){
				etatsA = etat2;
				released = false;
				}
			else {
				etatsA = INIT;
			}
				break; // sort de la boucle "if" et passe a la deuxieme machine
		case etat2 : 
			pressed = false;
			if(PIND & 0x04){
				etatsA = etat2;
				}
			else {
				etatsA = INIT;
				released = true;
				}
				break; // sort de la boucle "if" et passe a la deuxieme machine
		}
 // 2. Machine de Moore avec entree PIND et sortie Z.
	switch (etatsB){
		case etat3 :
			PORTA = 0x00;
			if(pressed)
				etatsB = etat4;
			else etatsB = etat3;
				break; // sort de la boucle "if" et passe a la deuxieme machine
				
		case etat4 :
			PORTA = 0x00;
			if(pressed)
				etatsB = etat5;
			else etatsB = etat4;
				break; // sort de la boucle "if" et passe a la deuxieme machine
				
		case etat5 : 
			PORTA = 0x00;
			if(pressed)
				etatsB = etat6;
			else etatsB = etat5;
				break; // sort de la boucle "if" et passe a la deuxieme machine
				
		case etat6 : 
			PORTA = 0x00;
			if(pressed)
				etatsB = etat7;
			else etatsB = etat6;
				break; // sort de la boucle "if" et passe a la deuxieme machine

		case etat7 : 
			PORTA = 0x00;
			if(pressed)
				etatsB = etat8;
			else etatsB = etat7;
				break; // sort de la boucle "if" et passe a la deuxieme machine
				
		case etat8 : 
			if(released){
				PORTA = 0x01;
				_delay_ms(1000);
				etatsB = etat3;
				}
				break; // sort de la boucle "if" et passe a la deuxieme machine
		}
 }
  return 0; 
}

