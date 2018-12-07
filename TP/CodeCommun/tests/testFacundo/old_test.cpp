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
uint16_t a = 56583;
uint16_t b = 70;
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
    }
    resultat = resultat / 16;
    DEBUG_PRINT(("lecture can réussie"));
    
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
            _delay_ms(1000);
    LED led(PD,7,6);
    led.ambre(100);
    DEBUG_PRINT(("c'est parti \n"));
    for(int valeurslues; valeurslues < NB_MAX_VALEURS; valeurslues++){
        //if(boutonPoussoir == 1){

        resultat = read(can);
        DEBUG_PRINT(("on a lu une valeur : "));
        
            
        intToString(resultat, resultatTab);
        intToString(resultat,chiffre);
        DEBUG_PRINT((resultatTab));
        
        
        for(int i = 0; i < 10; ++i){
            if(chiffre[i] != resultatTab[i]){
                chiffre[i] = '-';
            }
        }
        disp.write(chiffre,16);
        if(resultat < 1300){_delay_ms(1000);}
            //boutonPoussoir = 0;
        //}
    }
    disp.write("all ok", 16);
    _delay_ms(2000);
    intToString(nbvaleurs, resultatTab);
    disp.write(resultatTab, 16);
    _delay_ms(2000);
    int lenght;
    int difference;
    uint16_t compteur = 0;
    uint16_t lastDifference = 0;
    for(int i = 1; i < NB_MAX_VALEURS; i + 2){
        if(tableau[i] > tableau[i-1]) {
            difference = tableau[i] - tableau [i-1];
//            DEBUG_PRINT(("lenght : "));
            intToString(lenght, resultatTab);
            DEBUG_PRINT((resultatTab));
        }
        else if (tableau[(i-1)] > tableau[i]){
            difference = tableau[i-1] - tableau[i];
        }
        if(difference > 100){
            if(lastDifference < 100){

            lenght += difference;
            compteur++;
            }
            
        }
    }
    intToString(compteur, resultatTab);
    disp.write(resultatTab, 16);
    _delay_ms(2000);
    led.green();
    if(lenght > 20000) led.red();
    intToString(lenght, resultatTab);
    disp.write(resultatTab, 16);


}

ISR(INT0_vect) {
    boutonPoussoir = 1;
    //DEBUG_PRINT(("interrupt detected"));
    EIFR |= (1 << INTF0);
}