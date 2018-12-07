/*
 * Nom: pb2vincent.cpp
 * Description: Ceci est un programme mobilisant une roue avec deux frequences de PWN et 5 niveau d'intensite de facon cyclique
 * Version: 1.0
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu
 * Date de creation: 11/09/2018
 * Date de modification: 12/09/2018
 * Configuration : Moteur Enable relie a A0 et moteur dir relie a A1. 9V fourni au moteur et 5V a carte mere
 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>


int main()
{
    DDRD = 0xff; // 2 bits de poids faible en sortie pour les roues, 2 bits de poids fort en sortie pour la LED, D2 en entree pour l'interrupteur
    PORTD = 0x07;

    for (;;)
    {
       
    }
    return 0;
}
