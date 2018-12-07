/*
 * Nom: pb2vincent.cpp
 * Description: Ceci est un programme mobilisant une roue avec avec un PWN plus versatile que le proble initial
 * Version: 1.0
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu
 * Date de creation: 14/09/2018
 * Date de modification: 14/09/2018
 * Configuration : Moteur Enable relie a A0 et moteur dir relie a A1. 9V fourni au moteur et 5V a carte mere
 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

void delais_10us(int nbRepetition)
{
    for (int i = 0; i < nbRepetition; i++)
        _delay_us(10);
}

bool interrupteurEstAppuye()
{
    return (PIND & 0x04);
}
void faireRoulerRobot(int frequence = 100, int dureeParNiveau = 1, bool reculons = true, int nbNiveau = 10,
                      uint8_t bitDuEnable = 0x08, uint8_t bitDeDirectetion = 0x20)
{
    int nbDeCycleParNiveau = dureeParNiveau * frequence;
    int incrementation = 100 / nbNiveau;
    bool cycleEstTermine = false;
    int proportionDeA = 0;
    int unCentiemeDeCyleUS = 10000 / frequence;

    while (!cycleEstTermine)
    {
        for (int i = 0; i < nbDeCycleParNiveau; i++)
        {
            PORTD = bitDuEnable + (reculons * bitDeDirectetion);
            delais_10us(proportionDeA * unCentiemeDeCyleUS / 10);
            PORTD = 0x00;
            delais_10us(((100 - proportionDeA) * unCentiemeDeCyleUS) / 10);
        }
        proportionDeA += incrementation;
        if (proportionDeA > 100)
            cycleEstTermine = true;
    }
}
int main()
{
    DDRD = 0xfb; // 2 bits de poids faible en sortie pour les roues, D2 en entree pour l'interrupteur
    PORTD = 0x00;

    for (;;)
    {
        if (interrupteurEstAppuye())
        {
            faireRoulerRobot();
        }
    }
    return 0;
}
