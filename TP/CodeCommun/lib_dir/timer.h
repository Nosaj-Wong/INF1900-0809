/*
** file: timer.h
** Declaration de la classe timer qui permet de gerer une minuterie a interruption
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

#ifndef F_CPU
#define F_CPU 8000000
#endif

#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h> //on inclue interrupt pour s'assurer que l'on ne l'oublie pas dans le main


class Timer {
    public:

    //constructeur
    Timer(uint8_t timerCounterNumber = 1, uint16_t clockDivider = 1024);

    //methodes de modification
    void start(uint16_t duration);
    void stop();
    void setClockDivider(uint16_t divider);
    void setInterrupt(bool isEnabled);

    //methodes d'acces
    const bool getIsStarted();
    const bool getInterruptIsEnabled();
    const uint16_t getClockDivider();

    private:
    bool isStarted_;
    bool interruptIsEnabled_;
    uint16_t clockDivider_;
    uint16_t clockFrequence_;
    uint8_t timerNumber_;

    //registres: Ce sont des registres generiques que l'on peut changer l'adresse afin qu'il
    //correspondent au registres de la minuterie que l'on veut utiliser.
    volatile uint8_t* TCCRnA;
    volatile uint8_t* TCCRnB;
    volatile uint8_t* TCNTn;
    volatile uint8_t* OCRnA;
    volatile uint8_t* TIMSKn;

};

#endif