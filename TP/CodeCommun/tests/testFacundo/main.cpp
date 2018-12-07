#include "timer.h"
#include "LED.h"
#include <util/delay.h>
#include "singTimer.h"
#include "sing.h"
#include "debug.h"
#include "functions.h"
#include "can.h"
#include <string.h>
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"


#define DEMO_DDR	DDRC // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT	PORTC // Port AVR occup� par l'afficheur

#define MAX_NB_OF_VALUES 96 
//temps entre chaque interrupt par le timer
// 48/ MAX_NB_OF_VALUES / Vitesse = INTERRUPT_TIME
#define INTERRUPT_TIME 10 //100ms
//temps de delai entre chaque lecture
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

volatile uint8_t boutonPoussoir;
volatile uint8_t state;

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


void initialisation(void) {
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'être interrompu alors que
    // le microcontroleur n'est pas prêt...
    cli();
    // configurer et choisir les ports pour les entrées
    // et les sorties. DDRx... Initialisez bien vos variables
    //DDRC = 0xff;  //PORTC -> Out
    //DDRD = 0x00;  //PORTD -> In
    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes
    EIMSK |= (1 << INT0);
    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA |= (1 << ISC00);
    // sei permet de recevoir à nouveau des interruptions.
    sei();
}

//Simple timer functions

void setTimerB(){
    // OC3A mis sur "compare match", output = high level (p.128)
    TCCR1A |= (1 << COM1B1) | (1 << COM1B0);           
    // Diminution de l'horloge a 1/1024 (p.131) (bit CS10 mit a 0)
    TCCR1B |=  (1 << CS10) | (1 << CS12);
    TCCR1B &= ~(1 << CS11); 
    TCCR1C = 0;   
}
//start timer
void Sapristi(uint16_t duration){
    TCNT1 = 0;
    OCR1B =  78 * duration;
    TIMSK1 |= (1 << OCIE1B);
}

//read and calculate distance
void read(can &can){
    //cli();
    //PORTD = 0b10101010;
    uint16_t resultatDroite = 0;
    uint16_t resultatGauche = 0;
    for(int i = 0; i < VALUES_PER_READ; i++){
        resultatDroite += can.lecture(0); // capteur droit
        _delay_ms(TIME_PER_READ);
        resultatGauche += can.lecture(0);
    }
    resultatDroite = resultatDroite / VALUES_PER_READ;
    resultatGauche = resultatGauche / VALUES_PER_READ;
    char resultatTab[10];
    //intToString(resultat, resultatTab);
    //DEBUG_PRINT(("voltage sur 10 bit: "));
    //DEBUG_PRINT((resultatTab));            

    resultatDroite = RIGHT_NUMERATOR/(resultatDroite - RIGHT_DENOMINATOR_CONSTANT) - RIGHT_CORRECTION_CONSTANT;
    resultatGauche = LEFT_NUMERATOR/(resultatGauche - LEFT_DENOMINATOR_CONSTANT) - LEFT_CORRECTION_CONSTANT;

    resultatDroite = (resultatDroite * LENGHT_PRECISION ) / 16;
    resultatGauche = (resultatGauche * LENGHT_PRECISION ) / 16;
    tableauDroite[nbValues] = resultatDroite;
    tableauGauche[nbValues] = resultatGauche;
    nbValues++;
    //sei();
}

//calculate total wall lenght
void calculateLenght(){
    uint32_t difference = 0;
    uint32_t lenght = 0;
    uint16_t dx = 4800 / nbValues; //dx en pouces/100
    uint16_t hyp = 0;

    DEBUG_PRINT(("******* Calculate Lenght ********"))

    char resultatTab[10];
    intToString(dx, resultatTab);
    DEBUG_PRINT(("dx: "))
    DEBUG_PRINT((resultatTab));

    for(uint16_t i = 1; i < nbValues; i++){
        //on calcule la difference de distance entre 2 points consecutifs
        if(tableauDroite[i] > tableauDroite[i-1]){
            difference = tableauDroite[i] - tableauDroite[i-1];
        }
        else{
            difference = tableauDroite[i-1] - tableauDroite[i];
        }
        intToString(difference, resultatTab);
        DEBUG_PRINT(("difference: "))
        DEBUG_PRINT((resultatTab));

        //on exprime difference en pouces/10 000
        difference = difference * 1000;
        //on exprime la difference comme multiple de (dx * 100)
        difference = difference / dx;
        intToString(difference, resultatTab);
        DEBUG_PRINT(("difference facteur de dx: "))
        DEBUG_PRINT((resultatTab));

        if(difference > 250){                //ratio de 2.50
            //approximation hyp equivaut a:
            // (ratio + 0,1) * dx = hyp
            //donne hyp en pouces/100
            hyp = (difference + 10) *dx / 100;
            intToString(hyp, resultatTab);
            DEBUG_PRINT(("hyp (d > 250): "))
            DEBUG_PRINT((resultatTab));

            lenght += hyp;
        }
        else if(difference < 54)
        {
            //approx de hyp equivaut a
            //((ratio/10) + 1) * dx = diff/10 + dx
            //resultat en pouces/100
            hyp = (difference + 1000) *dx / 1000;
            intToString(hyp, resultatTab);
            DEBUG_PRINT(("hyp (d < 54): "))
            DEBUG_PRINT((resultatTab));

            lenght += hyp;
        }
        else {
            //on transforme le ratio en index d'un tableau contenant les hypotenuses 
            //des triangles ayant un ratio entre 0.55 et 2.45 au 0.05 pres
            difference = (difference - 50) / 5;
            hyp = (tableauHyp[difference] + 100) * dx / 100;
            intToString(hyp, resultatTab);
            DEBUG_PRINT(("hyp tableau: "))
            DEBUG_PRINT((resultatTab));

            lenght += hyp;
        }
    }
    rightLenght = lenght;
    
}


int main(){
    DDRB = 0xff;
    DDRD = 0x00;

	char* nom = "ATmegaReussi";
	char chiffre[10];
	intToString(0,chiffre);

	for(int i = 0; i < 10; ++i){
		if(chiffre[i] == '\n' || chiffre[i] == '\0'){
			chiffre[i] = 0;
		}
	}

	LCM disp(&DEMO_DDR, &DEMO_PORT);
	disp.write(nom);
	disp.write(chiffre,16);


    initialisation();
    uint32_t resultat;
    can can;
    setTimerB();
    Sapristi(INTERRUPT_TIME);
    state = 0;
    char resultatTab[10];
    intToString(MAX_NB_OF_VALUES, resultatTab);
    DEBUG_PRINT(("MAX NUMBER OF VALUES: "))
    DEBUG_PRINT((resultatTab));
    for(;;){
        switch(state){
            default:
            case 0: //wait
                //DEBUG_PRINT(("case 0 \n"));
                //state = 0;
                break;

            case 1: //read
                // timer.start(1000);
                read(can);     
                //DEBUG_PRINT(("case 1 \n"));
                //PORTB = 0b10101010;
                state++;
                //disp.write("red a value", 16);
                break;
            case 2: //display
                intToString(tableauDroite[nbValues -1], resultatTab);
                DEBUG_PRINT(("droite : "));
                DEBUG_PRINT((resultatTab));
                intToString(nbValues, resultatTab);
                DEBUG_PRINT(("nb valeurs : "));
                DEBUG_PRINT((resultatTab));
                //resultat = 100 + tableauDroite[nbValues - 1] - tableauGauche[nbValues - 1];
                //intToString(resultat, resultatTab);
                //DEBUG_PRINT(("difference + 100: "));
                //DEBUG_PRINT((resultatTab));
                // intToString(resultat,chiffre);
                // for(int i = 0; i < 10; ++i){
                    //  if(chiffre[i] != resultatTab[i]){
                        //  chiffre[i] = '-';
                    //  }
                //  }
                //disp.write(chiffre,16);
                state = 0;
                //PORTB = 0;

                break;
            case 3:
                calculateLenght();
                for(;;){

                intToString(rightLenght,resultatTab);
                DEBUG_PRINT(("case 3 - longueur droite: "));
                DEBUG_PRINT((resultatTab));
                //resultat = calculateLenght(tableauGauche);
                intToString(resultat,resultatTab);
                DEBUG_PRINT(("case 3 - longueur gauche: "));
                DEBUG_PRINT((resultatTab));
                }
                
        }



    }

    disp.write("over", 16);

}

ISR(INT0_vect) {
//     state = 1;
     //DEBUG_PRINT(("interrupt detected"));
//     EIFR |= (1 << INTF0);
state = 3;
}

ISR(TIMER1_COMPB_vect){
    //DEBUG_PRINT(("interrupt \n"));
    if(nbValues < MAX_NB_OF_VALUES){
        state = 1;
    }
    else {
        state =3;
        cli();
        //DEBUG_PRINT(("TOUTES LES VALEURS ONT ETE LUES \n"));
        }
    //PORTB = 0b01010101;
    Sapristi(INTERRUPT_TIME);
}