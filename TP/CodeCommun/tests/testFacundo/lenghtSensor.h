//temps entre chaque interrupt par le timer
// 48/ MAX_NB_OF_VALUES / Vitesse = INTERRUPT_TIME
#define INTERRUPT_TIME 10 //100ms
//temps de delai entre chaque lecture
//delaiTotal =  2 x VALUES_PER_READ x TIME_PER_READ
#define TIME_PER_READ 16
//nombre de valeurs lues pour faire une moyenne et retourner une valeur
#define VALUES_PER_READ 10

//constantes pour obtenir la distance en fonction du voltage
//valeurs pour le capteur droit
#define RIGHT_NUMERATOR 61233
#define RIGHT_DENOMINATOR_CONSTANT 47
#define RIGHT_CORRECTION_CONSTANT 0

//valeurs pour le capteur gauche
#define LEFT_NUMERATOR 61233
#define LEFT_DENOMINATOR_CONSTANT 47
#define LEFT_CORRECTION_CONSTANT 0

//precision des valeurs lues
#define LENGHT_PRECISION 10

//tableaux contenant les distances lues par chaque capteur
volatile uint16_t tableauDroite[MAX_NB_OF_VALUES];
volatile uint16_t tableauGauche[MAX_NB_OF_VALUES];
//nombre de valeurs lues par chaque capteur
volatile uint16_t nbValues = 0;

//longueur des murs en pouces/100
volatile uint32_t rightLenght = 0;
volatile uint32_t leftLenght = 0;

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

void read(can &can);

void calculateLenght();
