/*
 * Nom: pb1.cpp
 * Description: Resolution du probleme 1 du tp6
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu (1934201)
 * Creation: 05/10/2018
 * Derniere modification : 16/10/2018
 * Configuration : Fil de sortie du breadbord en D2, fil pour led sur A0 et A1,
 * avec fleche vis-a-vis A0 et LED+,montage du breadboard selon l'enonce en
 * ligne
 */

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

// Fonction qui retourne true si le bouton est presse, sinon false
// (Puisque le bouton est a 0 lorsque presse, on inverse l'entree PIND avant de
// faire le test habituel)
bool boutonEstPresse() { return (~PIND & 0x04); }

// Fonctions pour simplifier le controle des led
void ledVert() { PORTA = 0x02; }
void ledRouge() { PORTA = 0x01; }
void ledEteint() { PORTA = 0x00; }

int main() {
  // Mise du Port A en sortie (LED) et D en entree (bouton)
  DDRA = 0xff;
  DDRD = 0x00;

  // Initialisation du compteur et de son booleen d'atteinte du max de compte
  uint8_t compteur = 0;
  bool compteurEstMax = false;

  for (;;) {
    // Une fois que le bouton est pese, on reste dans la boucle tant qu'on ne le
    // relache pas ou que le compteur n'est pas au max
    while (boutonEstPresse() && !compteurEstMax) {
      // On attend 100 ms entre chaque incrementation (10x par secondes)
      _delay_ms(100);
      compteur++;

      // On verifie si on a atteint le max du compteur
      if (compteur == 120) {
        compteurEstMax = true;
      }

      // Agit comme double verification du relachement (avec la condition du
      // debut de la boucle) afin de s'assurer d'un relachement veritable avant
      // de quitter la boucle
      if (!boutonEstPresse()) {
        _delay_ms(30);
      }
    }

    // Seulement lors de la sortie de la boucle precedente
    // (donc seuelement apres avoir incrementer le compteur au moins une fois)
    if (compteur != 0) {
      
      ledVert();
      _delay_ms(500);
      ledEteint();
      _delay_ms(2000);

      // On fera compteur/2 cycles de une demi-seconde
      // afin d'obtenir 2 clignotements par seconde
      // On fait une division de int puisque cela change
      // peu le nombre de clignotement et evite les double
      for (uint8_t i = 0; i < (compteur / 2); i++) {
        ledRouge();
        _delay_ms(250);
        ledEteint();
        _delay_ms(250);
      }

      ledVert();
      _delay_ms(1000);
      ledEteint();

      // Reinitialisation des parametres
      compteur = 0;
      compteurEstMax = false;
    }
  }
  return 0;
}
