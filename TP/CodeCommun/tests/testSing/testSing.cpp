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


#include "singMario5seconds.h"

int main()
{
	DDRB = 0b11000000;

	SingMario5seconds sing;

	sing.UneFois();
}
