/*
** file: debug.h
** Declaration et implementation de la macro de deboguage
**
** date: 23 octobre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
*/

#include "UART.h"
#include <string.h>

// Si DEBUG est defini, on transmet le string passe en parametre au PC
// On transmet caractere par caractere pour la longreur (strlen) du string
#ifdef DEBUG
UART uart;
#define DEBUG_PRINT(str)                    \
  ;                                         \
  for (uint8_t i = 0; i < strlen(str); i++) \
  {                                         \
    uart.transmit(str[i]);                  \
  }

#else

// Sinon on ne fait rien
#define DEBUG_PRINT(str) \
  do                     \
  {                      \
  } while (0)

#endif
