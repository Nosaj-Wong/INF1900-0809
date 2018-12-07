/*
 * Nom: pb1vincentv.2.cpp
 * Description: Ceci est un programme permettant d'eteindre un LED graduellement sur 3 secondes de facon plus simple
 * Version: 1.0
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu
 * Date de creation: 11/09/2018
 * Date de modification: 11/09/2018
 * Configuration : LED libre relie a D0 et D1, fleche (positif) sur connecteur vis-a-vis D0 et LED+
 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

int DUREE_CYCLE_MSx100 = 100;
long int DUREE_RELACHEMENT_MS = 300000;

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
void monDelais_ms(int delaisVoulu)
{
    for (int i = 0; i < delaisVoulu; i++)
        _delay_us(10);
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
            int dureeDeA = DUREE_CYCLE_MSx100;
            for (int j = 0; j < 100; j++)
            {
                for (int i = 0; i < 30; i++)
                {
                    mettreLedRouge();
                    monDelais_ms(dureeDeA);
                    eteindreLed();
                    monDelais_ms(DUREE_CYCLE_MSx100 - dureeDeA);
                }
                dureeDeA--;
            }
            etatBouton = relache;
        }
    }
    return 0;
}
