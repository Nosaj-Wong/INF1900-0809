/*
** file: timer.cpp
** Implementation de la classe timer qui permet de gerer une minuterie a interruption
** Dans les methodes, on utilise les noms des bits du timer1, cette pratique fonctionne 
** car la position des bits utilises est la meme
** dans les registres des 3 timers
** 
** date: 20 octobre 2018
** 
** authors: 
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
** 
*/

#include "timer.h"

//constructeur qui va partir le compteur avec un diviseur
Timer::Timer(uint8_t timerCounterNumber,uint16_t clockDivider){
    //on associe les bonnes adresses a nos registres generiques
    switch(timerCounterNumber){
        case 0: 
            TCCRnA = &TCCR0A;
            TCCRnB = &TCCR0B;
            TCNTn = &TCNT0;
            OCRnA = &OCR0A;
            TIMSKn = &TIMSK0;
            timerNumber_=0;
        break;

        default: //compteur par défaut est compteur 1
        case 1:
            TCCRnA = &TCCR1A;
            TCCRnB = &TCCR1B;
            //TCNTn =   //on n'associe pas la variable au registre du compteur 1 
            //OCRnA =   //car la variable prends l'adresse de 1 registre et ces registres sont en fait composes de 2 registres

            //counter 1 a un registre TCCR1C non declare ici car aucun equivalent existe pour counter 0 et 2
            TIMSKn = &TIMSK1;
            timerNumber_ = 1;
        break;

        case 2:
            TCCRnA = &TCCR2A;
            TCCRnB = &TCCR2B;
            TCNTn = &TCNT2;
            OCRnA = &OCR2A;
            TIMSKn = &TIMSK2;
            timerNumber_ = 2;
        break;
    }

    setClockDivider(clockDivider);
    isStarted_ = false;
    interruptIsEnabled_ = false;
}

//Methode qui part la minuterie pour une duree en ms
// mis en mode compare match, compteur a 0, pas de force output compare, interruption active
void Timer::start(uint16_t duration){
   //Programmer la minuterie pour calculer 1 secondes avant d'interrompre
    *TCCRnA |= (1 << COM1A1) | (1 << COM1A0);           // OC3A mis sur "compare match", output = high level (p.128)

    if(timerNumber_ == 1){                               //Dans le cas qu'on prend timer 1, certains registres sont geres differement, ici on modifie les registres que l'on touchait pas dans le constructeur
    
        TCCR1C = 0;                                     //Pas de force output compare (p.131)
        TCNT1 = 0;                                      //TCNT1 est set ici car pour timer 1 il a 16 bit
        OCR1A = clockFrequence_ * duration / 1000;      //OCR1A est set ici car pour timer 1 il a 16 bit
    
    }

    else{                                               //registres a gerer pour timer 0 et timer 2

        *TCNTn = 0;                                     //Commencer le compteur a 0
        *OCRnA = clockFrequence_ * duration / 1000;      //clockFrequence_ / 1000 = valeur equivalent a 1 ms, on multiplie par duration avant pour garder plus de precision
    
    }
    
    *TIMSKn = (1 << OCIE1A);                            // Enable l'interrupt pour compare A (p.134)
    interruptIsEnabled_ = true;
    isStarted_ = true;
 
}

//methode qui desactive le timer et remet les ports dans leur mode d'operation normal
void Timer::stop(){
    *TCCRnB &= ~((1 << CS10) | (1 << CS11) | (1 << CS12)); // No clock source, timer/counter stopped 
    *TCCRnA &= ~((1 << COM1A1) | (1 << COM1A0));           // Normal port operation, OC3A disconnected (p.128) COM1A1 AND COM1A0 are set to zero
}

//methode qui permet d'activer ou desactiver l'interrupt genere par le timer
void Timer::setInterrupt(bool isEnabled){
    if(isEnabled){
        *TIMSKn |= (1 << OCIE1A);
        interruptIsEnabled_ = true;
    }
    else {
        *TIMSKn &= ~(1 << OCIE1A); //OCIE1A is set to 0
        interruptIsEnabled_ = false;
    }
}

//methode qui permet de choisir le diviseur de l'horloge
void Timer::setClockDivider(uint16_t divider){
    clockDivider_ = divider;
    clockFrequence_ = F_CPU / divider;
    switch(divider){
        case 1: // CS1n = 001
        *TCCRnB |=  (1 << CS10);
        *TCCRnB &= ~((1 << CS11) | (1 << CS12)); // Diminution de l'horloge a 1/1 (p.131) (bit CS11 et CS12 mit a 0)
        break;

        case 8: //CS1n = 010
        *TCCRnB |=  (1 << CS11);
        *TCCRnB &= ~((1 << CS10) | (1 << CS12)); // Diminution de l'horloge a 1/8 (p.131) (bit CS10 et CS12 mit a 0)
        break;

        case 64: //CS1n = 011
        *TCCRnB |=  (1 << CS10) | (1 << CS11);
        *TCCRnB &= ~(1 << CS12); // Diminution de l'horloge a 1/64 (p.131) (bit CS12 mit a 0)
        break;

        case 256: //CS1n = 100
        *TCCRnB |=  (1 << CS12);
        *TCCRnB &= ~((1 << CS10) | (1 << CS11)); // Diminution de l'horloge a 1/256 (p.131) (bit CS10 et CS11 mit a 0)
        break;

        case 1024: //CS1n = 101
        default: //by default clock is divided by 1024
        *TCCRnB |=  (1 << CS11) | (1 << CS12);
        *TCCRnB &= ~(1 << CS10); // Diminution de l'horloge a 1/1024 (p.131) (bit CS10 mit a 0)
        break;  
    }
}

const bool Timer::getIsStarted(){
    return isStarted_;
}

const bool Timer::getInterruptIsEnabled(){
    return interruptIsEnabled_;
}

const uint16_t Timer::getClockDivider(){
    return clockDivider_;
}



