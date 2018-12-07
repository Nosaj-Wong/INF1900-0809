/*
 * Nom: Probleme 1
 * Description: Ceci est un programme qui écrit la chaîne de caractères
 * « *P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*» suivi d'un 0x00 en mémoire
 * externe. Version: 1.1
 */

#define F_CPU 8000000
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int compteur;
bool buttonReleased = false;


// un compteur qui incrémente 10 fois par seconde
void incremente10fois() {
  for (int i = 0; ((i < 10) && (~PIND & 0x04)); i++) {
    _delay_ms(100);
    compteur++;
  }
}

// LED rouge devra clignoter (compteur / 2) fois au rythme de 2 fois par
// seconde.
void ledClignote2fois() {
  for (int i = 0; i < (compteur / 2); i++) {
    PORTA = 0x01; // ROUGE
    _delay_ms(250);
    PORTA = 0x00; // ETEINTE
    _delay_ms(250);
    PORTA = 0x01; // ROUGE
    _delay_ms(250);
    PORTA = 0x00; // ETEINTE
    _delay_ms(250);
  }
}

int main() {

  DDRD = 0x00; // PD2
  DDRA = 0xff;

  for (;;) {
    while (~PIND & 0x04) { // while "button pressed"
      incremente10fois();
      buttonReleased == true;
    }
    if (compteur == 120 || buttonReleased == true) {
      PORTA = 0x02; // VERT
      _delay_ms(500);

      PORTA = 0x00; // ETEINTE
      _delay_ms(2000);

      ledClignote2fois(); // ROUGE

      PORTA = 0x02; // VERT
      _delay_ms(1000);
      PORTA = 0x00; // ETEINTE

      compteur = 0;
    }
  }

  return 0;
}
