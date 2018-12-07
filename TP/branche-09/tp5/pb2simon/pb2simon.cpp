/*
 * Nom: Probleme 1
 * Description: Ceci est un programme qui écrit la chaîne de caractères
 * « *P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*» suivi d'un 0x00 en mémoire
 * externe. Version: 1.1
 */

#define F_CPU 8000000
#include <avr/interrupt.h>
#include <avr/io.h>
#include <memoire_24.h>
#include <util/delay.h>

// Placer le cavalier DbgEN (mode actif).
//  Les broches D0 et D1 du USART0 du ATmega324PA seront reliées au USART du
//  ATMega8.

void initialisationUART(void) {

  // 2400 bauds. Nous vous donnons la valeur des deux premier registres pour
  // vous éviter des complications

  UBRR0H = 0;

  UBRR0L = 0xCF;

  // permettre la reception et la transmission par le UART0

  UCSR0A = (1 << UDRE0); // p.185 Flag indicates if the transmit buffer (UDRn)
                         // is ready to receive new data.
                         // a utiliser dans transmissionUART
  UCSR0B |=
      (1 << RXEN0) | (1 << TXEN0); // p.187 - Writing this bit to '1' enables
                                   // the USART Receiver/Transmitter.

  // Format des trames: 8 bits, 1 stop bits, none parity

  UCSR0C |=
      (1 << UCSZ01) | (1 << UCSZ00); // p.188 (8 bit) (le reste est a zero)
}

// De l'USART vers le PC (envoie un octet de la carte vers le PC)

void transmissionUART(uint8_t donnee) {
  //p .174

      /* Wait for empty transmit buffer */
      while (!(UCSR0A & (1 << UDRE0))); // reste dans boucle si UDRE0 == 0

  /* Put data into buffer, sends the data */
  UDR0 = donnee;
}

int main() {
  //
  // DDRD = 0x00; // géré par le processeur Atmel324

  initialisationUART();

  char mots[21] = "Le robot en INF1900\n";

  uint8_t i, j;

  for (i = 0; i < 100; i++) {

    for (j = 0; j < 20; j++) {

      transmissionUART(mots[j]);
    }
  }

  return 0;
}
