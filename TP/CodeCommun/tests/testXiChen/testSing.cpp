/*
** file: testSing.cpp
** Petit programme qui joue la melodie de Star Wars sur le haut-parleur du robot
**
** date: 22 octobre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
** Configuration materiel: Brancher le heut parleur sur les broches C0 et C1
*/


#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include "functions.h"
#define DEMO_DDR	DDRC // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT	PORTC // Port AVR occup� par l'afficheur


int main() {

	DDRB = 0xff;
	PORTB = 0x1;

	char* nom = "ATmegaReussi";
	char chiffre[10];
	intToString(12345,chiffre);

	for(int i = 0; i < 10; ++i){
		if(chiffre[i] == '\n' || chiffre[i] == '\0'){
			chiffre[i] = 0;
		}
	}

	LCM disp(&DEMO_DDR, &DEMO_PORT);
	disp.write(nom);
	disp.write(chiffre,16);

}


