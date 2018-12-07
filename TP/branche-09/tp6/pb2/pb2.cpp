/*
 * Nom: pb3.cpp
 * Description: Resolution du probleme 3 du tp6
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu (1934201)
 * Creation: 05/10/2018
 * Derniere modification : 16/10/2018
 * Configuration : Fil de sortie du breadbord en A0, fil pour led sur B0 et B1,
 * avec fleche vis-a-vis B0 et LED+, VCC et GND provenant des pin du port A,
 * montage du breadboard selon l'enonce en ligne
 */

/*
 * Selon nos calculs a l'aide du multimetre, notre photo-resistance a des
 * resistance de 2k lorsque legerement cachee, de 0.8k a la luminosite ambiante
 * du laboratoire et de 0.1k avec une lampe de poche appliquee dessus.
 *
 * Sachant que l'autre resistance du circuit vaut 10k, nous obtenons donc les
 * tensions suivante en sortie:
 * -Legerement cachee: (10/12)*5 = 4.16V
 * -Luminosite du laboratoire: (10/10.8)*5=4.63V
 * -Lampe de poche: (10/10.1)*5= 4.95V
 *
 * Etant donne que le CAN converti ces voltages sur la plage 0-5V, et que nous
 * la mettons sur 8 bits, nous obtenons les entiers suivants:
 * -Legerement cachee: (4.16/5)*255 = 212
 * -Luminosite du laboratoire: (4.63/5)*255 = 236
 * -Lampe de poche: (4.95/5)*255 = 252
 *
 * Nous avons donc choisi les plages suivantes:
 * -Legerement cachee: < 220
 * -Luminosite laboratoire: 220-245
 * -Lampe de poche:  >245
 */

#include <avr/io.h>

#include "can.h"

// Fonctions pour simplifier le controle des led
void ledVert() { PORTB = 0x02; }
void ledRouge() { PORTB = 0x01; }
void ledEteint() { PORTB = 0x00; }

// Pour la led ambre, fait alterner les bit 0 et 1 de alternateur
// a chaque appel de la fonction et met ce nombre sur le port B
void ledAmbre(uint8_t& alternateur) {
  PORTB = alternateur;
  alternateur = ~alternateur;
}

int main() {
  // Mise du PORT A en entree (CAN) et B en sortie (LED)
  DDRA = 0x00;
  DDRB = 0xff;

  // Instanciation d'un objet de la classe d'interface avec le CAN
  can luminosite;

  uint8_t intensiteLumineuse = 0;

  // Definition des seuils (voir commentaire au debut du programme)
  uint8_t seuilLuminositeAmbiante = 220;
  uint8_t seuilLuminositeElevee = 245;

  // Initiation de l'alternateur pour la LED ambre
  uint8_t alternateurAmbre = 0x01;

  for (;;) {
    // Lecture de la luminosite actuelle
    intensiteLumineuse = (luminosite.lecture(0) >> 2);

    // Modification du LED selon la luminosite actuelle et les seuils
    if (intensiteLumineuse < seuilLuminositeAmbiante) {
      ledVert();
    } else if (intensiteLumineuse < seuilLuminositeElevee) {
      ledAmbre(alternateurAmbre);
    } else {
      ledRouge();
    }
  }
  return 0;
}
