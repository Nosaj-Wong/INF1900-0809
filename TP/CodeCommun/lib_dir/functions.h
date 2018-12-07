/*
** file: aGerer.cpp
** Declaration de diverses fonctions utiles
** 
** date: 22 octobre 2018
** 
** authors: 
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
** 
*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "debug.h"

void _my_delay_10us(unsigned long us);

void PWM_software(int percentage, double duration, double frequency);

enum Edge { low = 0, any = 1, falling = 2, rising = 3 };
void initializeButtonInterrupt(Edge edge);
            
bool buttonPressed();

bool debounceButtonPress();

void intToString(uint32_t numberToTransform,char* string);

void printTunnel(uint16_t nElements, uint16_t *leftDistances, uint16_t *rightDistances);

#endif