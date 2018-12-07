/*
 * Nom: MachineEtat1.cpp
 * Description: Resolution du probleme 1 du TP2, soit mise en marche de la DEL apres 5 cycles de pression-relachement de l'interrupteur
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu (1934201)
 * Creation: 31/08/2018
 * Derniere modification : 07/09/2018
 * Configuration : LED libre reliee a A0 et A1, fleche (positif) sur connecteur vis-a-vis A0 et LED+

Machine de Mealy pour filtrer les pressions (anti-rebonds et signal pour un seul cycle)
	Etat Present	|	Interrupteur (1=presse, 0=relache)	|      Etat suivant		|	Sortie bool boutonAEtePresse
	________________|_______________________________________|_______________________|_________________________________________________________________
	relache			|	0									|	relache				|	0
	relache			|	1									|	presse				|	0
	presse			|	0									|	relache				|	1 (envoie le signal qu'un cycle pression-relache est complete)
	presse			|	1									|	presse				|	0

Machine de Moore pour compter le nombre de cycles pression-relachement et allumage de la DEL
	Etat Present	|     bool boutonAEtePresse (1=true, 0=false)	|     Etat suivant	|	Sortie LED (0=eteinte 1=allumee)
	________________|_______________________________________________|___________________|______________________________________________________
	p0				|	0											|	p0				|	0
	p0				|	1											|	p1				|	
	p1				|	0											|	p1  			|	0
	p1				|	1											|	p2				|	
	p2				|	0											|	p2				|	0
	p2				|	1											|	p3				|	
	p3				|	0											|	p3				|	0
	p3				|	1											|	p4				|	
	p4				|	0											|	p4 				|	0
	p4				|	1											|	p5				|	
	p5				|	0											|	p0				|	1 (puisque 5 cycles ont ete complete)
	p5				|	1											|	p0				|	

 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

int main()
{
	DDRD = 0x00; // Captera les changements au niveau de l'interrupteur donc en entree
	DDRA = 0xFF; // Utilisera la LED donc en sortie

	enum Compteur
	{
		p0,
		p1,
		p2,
		p3,
		p4,
		p5
	}; // px ou x est le nombre de cycle pression-relachement effectue

	Compteur etatCompteur = p0; 

	enum Interrupteur
	{
		relache,
		presse
	};

	Interrupteur etatInterrupteur = relache;

	bool boutonAEtePresse = false; // information qui sera transmise de la premiere machine a la completion d'un cycle pression-relachement

	for (;;)
	{
		// Premiere machine a etat pour filtrer les pressions
		switch (etatInterrupteur)
		{
		case relache:
			boutonAEtePresse = false;
			if (PIND & 0x04) // Anti-rebond logiciel
			{
				_delay_ms(10);
				if (PIND & 0x04)
				{
					etatInterrupteur = presse; // Conclusion que le bouton est presse, changement d'etat
				}
			}
			else
			{
				etatInterrupteur = relache;
			}
			break;

		case presse:
			if (PIND & 0x04) // Pas besoin d'anti-rebond puisque deja presse
			{
				etatInterrupteur = presse;
				boutonAEtePresse = false; // Tant qu'il est presse, le cycle n'a pas ete complete
			}
			else
			{
				etatInterrupteur = relache;
				boutonAEtePresse = true; // On a ici complete un cycle pression-relachement
			}
			break;
		}

		// Deuxieme machine a etat pour compter le nombre de pressions
		switch (etatCompteur)
		{
		case p0:
			PORTA = 0x00;
			if (boutonAEtePresse)
			{
				etatCompteur = p1;
			}
			else
			{
				etatCompteur = p0;
			}
			break;

		case p1:
			PORTA = 0x00;
			if (boutonAEtePresse)
			{
				etatCompteur = p2;
			}
			else
			{
				etatCompteur = p1;
			}
			break;

		case p2:
			PORTA = 0x00;
			if (boutonAEtePresse)
			{
				etatCompteur = p3;
			}
			else
			{
				etatCompteur = p2;
			}
			break;

		case p3:
			PORTA = 0x00;
			if (boutonAEtePresse)
			{
				etatCompteur = p4;
			}
			else
			{
				etatCompteur = p3;
			}
			break;
		case p4:
			PORTA = 0x00;
			if (boutonAEtePresse)
			{
				etatCompteur = p5;
			}
			else
			{
				etatCompteur = p4;
			}
			break;
		case p5:
			PORTA = 0x01; // On atteint 5 cycles, donc on allume la LED et on attend 1 seconde, puis on recommence a p0
			_delay_ms(1000);
			etatCompteur = p0;
			break;
		}
	}
	return 0;
}
