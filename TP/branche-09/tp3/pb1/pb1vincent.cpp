/*
 * Nom: pb1vincent.cpp
 * Description: Ceci est un programme permettant d'eteindre un LED graduellement sur 3 secondes
 * Version: 1.0
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu
 * Date de creation: 11/09/2018
 * Date de modification: 11/09/2018
 * Configuration:LED libre relie a D0 et D1, fleche (positif) sur connecteur vis-a-vis D0 et LED+
 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

double DUREE_CYCLE_MS = 1;
double DUREE_RELACHEMENT_MS = 3000;
double DUREE_DES_DELAIS_MS = 0.01;

void mettreLedRouge()
{
    PORTD = 0x01;
}
void mettreLedVert()
{
    PORTD = 0x02;
}
void eteindreLed()
{
    PORTD = 0x00;
}
bool interrupteurEstPresse()
{
    return (PIND & 0x04);
}

int main()
{
    DDRD = 0x03; // D0 et D1 en mdoe sortie (LED) et D(2-7) en mode entree
    enum EtatBouton
    {
        relache,
        pousse,
        enRelachement
    };
    EtatBouton etatBouton = relache;

    bool relachementEstTermine = false;
    double tempsEnA = DUREE_CYCLE_MS;
    double tempsEnB;
    int nbDeCycle = DUREE_RELACHEMENT_MS / DUREE_CYCLE_MS;
    double decrementation=DUREE_CYCLE_MS/nbDeCycle;

    for (;;) // boucle sans fin
    {
        switch (etatBouton)
        {
        case relache:
            eteindreLed();
            if (interrupteurEstPresse())
            {
                _delay_ms(10);
                if (interrupteurEstPresse())
                {
                    etatBouton = pousse;
                }
            }
            break;
        case pousse:
            mettreLedRouge();
            if (!interrupteurEstPresse())
            {
                etatBouton = enRelachement;
            }
            break;
        case enRelachement:
            while (!relachementEstTermine)
            {
                for (int i = 0; i < tempsEnA / DUREE_DES_DELAIS_MS; i++)
                {
                    mettreLedRouge();
                    _delay_ms(0.01);
                }
                tempsEnB = DUREE_CYCLE_MS - tempsEnA;
                for (int i = 0; i < tempsEnB / DUREE_DES_DELAIS_MS; i++)
                {
                    eteindreLed();
                    _delay_ms(0.01);
                }
                if (tempsEnA <= 0)
                    relachementEstTermine = true;
                tempsEnA-=decrementation;
            }
            etatBouton = relache;
            tempsEnA=DUREE_CYCLE_MS;
            relachementEstTermine=false;
        }
    }
    return 0;
}
