/*
** file: UART.h
** Declaration de la classe UART permettant de communiquer du robot au PC
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

#ifndef UART_H
#define UART_H

#include <avr/io.h>

class UART {
 public:
  UART();
  void transmit(uint8_t data);
  uint8_t receive();

 private:
};

#endif