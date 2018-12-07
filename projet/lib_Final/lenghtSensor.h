
/*
** file: lenghtSensor.h
** Declaration de la classe lenghtSensor qui gere le calcule et la prise de
** distances
**
** date: 16 novembre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
*/

#include <math.h>
#include <util/delay.h>
#include "can.h"

// temps de delai entre chaque lecture
// delaiTotal =  2 x VALUES_PER_READ x TIME_PER_READ
#define TIME_PER_READ 2
// nombre de valeurs lues pour faire une moyenne et retourner une valeur
#define VALUES_PER_READ 90

// constantes pour obtenir la distance en fonction du voltage (issu de prise de
// mesure et d'extraction de courbe de tendance sur excel) valeurs pour le
// capteur droit
#define RIGHT_NUMERATOR1 38685
#define RIGHT_DENOMINATOR_CONSTANT1 0
#define RIGHT_CORRECTION_CONSTANT1 6

#define RIGHT_NUMERATOR2 22254
#define RIGHT_DENOMINATOR_CONSTANT2 0
#define RIGHT_CORRECTION_CONSTANT2 82

// valeurs pour le capteur gauche
#define LEFT_NUMERATOR1 43158
#define LEFT_DENOMINATOR_CONSTANT1 0
#define LEFT_CORRECTION_CONSTANT1 13

#define LEFT_NUMERATOR2 25383
#define LEFT_DENOMINATOR_CONSTANT2 0
#define LEFT_CORRECTION_CONSTANT2 72

// precision des valeurs lues
#define LENGHT_PRECISION 10

void read(can &can, uint16_t *tableauGauche, uint16_t *tableauDroite,
          uint16_t &nbValues, int8_t leftAdjust, int8_t rightAdjust);

uint32_t calculateLenghtSqrt(uint16_t *tableauDroite, uint16_t nbValues);
