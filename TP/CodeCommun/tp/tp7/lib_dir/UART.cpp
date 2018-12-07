/*
** file: UART.cpp
** Implementation de la classe UART permettant de communiquer du robot au PC
** 
** date: 19 octobre 2018
** 
** authors: 
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
** 
*/

#include "UART.h"

//Constructeur par defaut de UART. Initialise les registres necessaires a la transmission
UART::UART()
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

//Cette methode prends une donne en parametre, attends que le buffer soit vide et la transmet par communication serielle
void UART::transmit(uint8_t data)
{

	while (!(UCSR0A & (1 << UDRE0))) // Attendre que le buffer d'envoi soit vide
		;
	UDR0 = data; //Envoyer la donnee
}