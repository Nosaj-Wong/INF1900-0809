/*
** file: FinalFuntions.h
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

#ifndef F_CPU
#define F_CPU 8000000
#endif

#ifndef FINALFUNCTIONS_H
#define FINALFUNCTIONS_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"

void _my_delay_10us(unsigned long us);

void PWM_software(int percentage, double duration, double frequency);

bool buttonPressed();

void intToString(uint32_t numberToTransform, char *string);

void printTunnel(uint16_t nElements, uint16_t *leftDistances, uint16_t *rightDistances);

void displayFinalResults(LCM &disp, const uint32_t &lengthLeft, const uint32_t &lengthRight);

#endif