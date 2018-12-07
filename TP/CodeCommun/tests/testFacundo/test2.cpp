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

#define NB_MAX_VALEURS 96

volatile uint8_t boutonPoussoir;
volatile uint16_t tableau[NB_MAX_VALEURS];
volatile uint16_t nbvaleurs = 0;
uint32_t a = 66561;
uint16_t b = 68;
uint8_t k = 0;

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

uint16_t read(can &can){
    uint16_t resultat = 0;
    for(int i = 0; i < 16; i++){
        resultat += can.lecture(1);
        _delay_ms(10);
    }
    resultat = resultat / 16;


    resultat = a/(resultat - b) - k;
    resultat = (resultat * 100 ) >> 4;
    tableau[nbvaleurs] = resultat;
    nbvaleurs++;
    return resultat;

}

int main(){
    DDRD = 0b1111000;
    DDRB = 0xff;
	PORTB = 0x1;

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
    char resultatTab[10];
    can can;

    for(int valeurslues = 0; valeurslues < NB_MAX_VALEURS; valeurslues++){

        resultat = read(can);        
            
        intToString(resultat, resultatTab);
        intToString(resultat,chiffre);
        DEBUG_PRINT((resultatTab));
        
        
        for(int i = 0; i < 10; ++i){
            if(chiffre[i] != resultatTab[i]){
                chiffre[i] = '-';
            }
        }
        disp.write(chiffre,16);
        _delay_ms(250);
    }
    disp.write("all ok", 16);
    _delay_ms(2000);
    intToString(nbvaleurs, resultatTab);
    disp.write(resultatTab, 16);
    _delay_ms(2000);

    //CALCULATE LENGHT
    int lastDifference;
    int middleValue;
    uint32_t difference = 1000;
    for(int value = 1; value < nbvaleurs; value++){
        difference += tableau[value-1];
        difference -= tableau[value];
        
    }
    difference -= 1000;
    disp.write("difference", 16);
    _delay_ms(2000);
    intToString(difference, resultatTab);
    disp.write(resultatTab, 16);
    _delay_ms(2000);



}

ISR(INT0_vect) {
    boutonPoussoir = 1;
    //DEBUG_PRINT(("interrupt detected"));
    EIFR |= (1 << INTF0);
}