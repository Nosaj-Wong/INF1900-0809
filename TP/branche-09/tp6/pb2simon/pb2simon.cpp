/*
 * Nom: Probleme 1
 * Description: Ceci est un programme qui écrit la chaîne de caractères
 * « *P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*» suivi d'un 0x00 en mémoire
 * externe. Version: 1.1
 */

#define F_CPU 8000000
#include <avr/interrupt.h>
#include <avr/io.h>
#include <can.h>
#include <util/delay.h>

can convertisseur;

// luminosité moyenne:  10*5/(10+0.8)   = 4.6V    (234.6B/255B)
// luminosité basse:    10*5/(10+2)    = 4.16V  (106.233/255B)
// luminosité haute:    10*5/(10+0.120) = 4.9407V (255B/255B)

uint8_t luminositeLu;


// luminosite basse
uint8_t borne2 = 225;
// luminosite moyenne
uint8_t borne3 = 239;
// luminosite haute


void ledAmbre(uint8_t &alternateur) {
  alternateur = ~alternateur;
  PORTB = alternateur;
}

void ledVerte() { PORTB = 0x02; }

void ledRouge() { PORTB = 0x01; }

int main() {

  uint8_t alternateur = 0x01;

  DDRA = 0x00;
  DDRB = 0xff;

  for (;;) {

    luminositeLu = (convertisseur.lecture(0x00) >> 2);

    if (luminositeLu <= borne2) {
      ledVerte();
    } else if ((luminositeLu >= borne2) && (luminositeLu <= borne3)) {
      ledAmbre(alternateur);
    } else {
      ledRouge();
    }
  }
}
