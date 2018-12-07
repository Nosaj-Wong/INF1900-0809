/*
 * Nom: pb2vincent.cpp
 * Description: Resolution du probleme 2 du tp4
 * Auteurs: Vincent L'Ecuyer-Simard (1925167) et Simon Berhanu (1934201)
 * Creation: 18/09/2018
 * Derniere modification : 18/09/2018
 * Configuration : LED libre relie a A0 et A1, fleche (positif) sur connecteur vis-a-vis A0 et LED+

 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void initialisationUART(void)
{

    // 2400 bauds. Nous vous donnons la valeur des deux

    // premier registres pour vous éviter des complications

    UBRR0H = 0;

    UBRR0L = 0xCF;

    // permettre la reception et la transmission par le UART0

    UCSR0A = 0;

    UCSR0B = 1 << RXEN0 | 1 << TXEN0; //Enable la transmission et la reception (Rx,Tx) (p.186)

    // Format des trames: 8 bits, 1 stop bits, none parity

    UCSR0C = 1 << UCSZ01 | 1 << UCSZ00; // mode 8 bit, pas de parity, 1 stop bit (p.188) (deja comme ca de base)
}

// De l'USART vers le PC

void transmissionUART(uint8_t donnee)
{

    while (!(UCSR0A & (1 << UDRE0))) // Attendre que le buffer d'envoi soit vide
        ;
    UDR0 = donnee; //Envoyer la donnee
}

int main()
{
    DDRA=0xff;
    initialisationUART();
    char mots[21] = "Le robot en INF1900\n";

    uint8_t i, j;

    for (i = 0; i < 100; i++)
    {

        for (j = 0; j < 20; j++)
        {
            PORTA = 0x01;
            transmissionUART(mots[j]);
            PORTA = 0x00;
        }
    }
}
