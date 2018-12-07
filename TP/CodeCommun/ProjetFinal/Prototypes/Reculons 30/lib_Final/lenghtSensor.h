#include <util/delay.h>
#include <math.h>
#include "can.h"

//temps de delai entre chaque lecture
//delaiTotal =  2 x VALUES_PER_READ x TIME_PER_READ
#define TIME_PER_READ 2
//nombre de valeurs lues pour faire une moyenne et retourner une valeur
#define VALUES_PER_READ 70

//constantes pour obtenir la distance en fonction du voltage
//valeurs pour le capteur droit
#define RIGHT_NUMERATOR1 38670
#define RIGHT_DENOMINATOR_CONSTANT1 0
#define RIGHT_CORRECTION_CONSTANT1 5

#define RIGHT_NUMERATOR2 20287
#define RIGHT_DENOMINATOR_CONSTANT2 0
#define RIGHT_CORRECTION_CONSTANT2 83

#define RIGHT_NUMERATOR3 12645
#define RIGHT_DENOMINATOR_CONSTANT3 0
#define RIGHT_CORRECTION_CONSTANT3 133

//valeurs pour le capteur gauche
#define LEFT_NUMERATOR1 39699
#define LEFT_DENOMINATOR_CONSTANT1 0
#define LEFT_CORRECTION_CONSTANT1 5

#define LEFT_NUMERATOR2 22040
#define LEFT_DENOMINATOR_CONSTANT2 0
#define LEFT_CORRECTION_CONSTANT2 82


//precision des valeurs lues
#define LENGHT_PRECISION 10

//tableau contenant la proportion entre l'hypotenuses et la cathete normalisee
//des triangles ayant des cathetes avec un ratio de 0,5 à 2,5
//valeur dans tableau = proportion x 100 -100
//calcul fait pour le sauvegarder en 8bit
const uint8_t tableauHyp[39] = {
//  0.55 | 0.60 | 0.65 | 0.70 | 0.75 | 0.80 | 0.85 | 0.90 | 0.95 | 1.00 |
     14,    16,    19,    22,    25,    28,    31,    34,    37,    41, 
//  1.05 | 1.10 | 1.15 | 1.20 | 1.25 | 1.30 | 1.35 | 1.40 | 1.45 | 1.50 |
     45,    48,    52,    56,    60,    64,    68,    72,    76,    80, 
//  1.55 | 1.60 | 1.65 | 1.70 | 1.75 | 1.80 | 1.85 | 1.90 | 1.95 | 2.00 |
     84,    88,    92,    97,    101,   105,   110,   114,   119,   123,
//  2.05 | 2.10 | 2.15 | 2.20 | 2.25 | 2.30 | 2.35 | 2.40 | 2.45 |
     128,   132,   137,   141,   146,   150,   155,   160,   164
};

void read(can &can, uint16_t* tableauGauche, uint16_t* tableauDroite, uint16_t &nbValues, int8_t leftAdjust, int8_t rightAdjust);
uint32_t calculateLenght(uint16_t* tableauDroite, uint16_t &nbValues);
uint32_t calculateLenghtTest1(uint16_t* tableauDroite, uint16_t nbValues);
uint32_t calculateLenghtTest2(uint16_t* tableauDroite, uint16_t nbValues);
uint32_t calculateLenghtTest3(uint16_t* tableauDroite, uint16_t nbValues);
