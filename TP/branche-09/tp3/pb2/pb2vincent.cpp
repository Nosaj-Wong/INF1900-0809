/*
 * Nom: pb2vincent.cpp
 * Description: Ceci est un programme mobilisant une roue avec deux frequences de PWN et 5 niveau d'intensite de facon cyclique
 * Version: 1.0
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu
 * Date de creation: 11/09/2018
 * Date de modification: 12/09/2018
 * Configuration : Moteur Enable relie a D0 et moteur dir relie a D1. 9V fourni au moteur et 5V a carte mere
 *    (optionnel): LED + relie a D8
 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

void mon_delais(int nQuartDeCycle, int frequence)
{
    if (frequence == 60)
    {
        for (int i = 0; i < nQuartDeCycle; i++)
            _delay_us(4167);
    }
    else if (frequence == 400)
    {
        for (int i = 0; i < nQuartDeCycle; i++)
            _delay_us(625);
    }
}
bool interrupteurEstAppuye()
{
    return (PIND & 0x04);
}

int main()
{
    DDRD = 0xc3; // 2 bits de poids faible en sortie pour les roues, 2 bits de poids fort en sortie pour la LED, D2 en entree pour l'interrupteur
    PORTD = 0x00;

    for (;;)
    {
        if (interrupteurEstAppuye())
        {
            int quartDeCycleA = 0;
            int frequenceActuelle = 60;
            int nbDeCyclePour2Secondes = 120;
            bool EstComplete40Hz = false;
            bool EstComplete400Hz = false;
            uint8_t couleurLed = 0x80;
            uint8_t sensRoue = 0x02;

            while (!EstComplete40Hz || !EstComplete400Hz)
            {
                for (int i = 0; i < nbDeCyclePour2Secondes; i++)
                {
                    PORTD = 0x01 | couleurLed | sensRoue;
                    mon_delais(quartDeCycleA, frequenceActuelle);
                    PORTD = 0x00| sensRoue;
                    mon_delais(4 - quartDeCycleA, frequenceActuelle);
                }
                quartDeCycleA++;
                if (quartDeCycleA > 4)
                {
                    quartDeCycleA = 0;
                    if (!EstComplete40Hz)
                    {
                        EstComplete40Hz = true;
                        frequenceActuelle = 400;
                        nbDeCyclePour2Secondes = 800;
                        couleurLed = 0x40;
                        sensRoue = 0x00;
                    }
                    else if (EstComplete40Hz)
                    {
                        EstComplete400Hz = true;
                    }
                }
            }
        }
    }
    return 0;
}
