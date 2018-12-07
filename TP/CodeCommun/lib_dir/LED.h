/*
** file: LED.h
** Declaration de la classe LED permettant le controle d'une LED
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

#ifndef LED_H
#define LED_H

#include <avr/io.h>
#include <util/delay.h>

enum Port { PA,
            PB,
            PC,
            PD };

class LED {
   public:
    // constructeur par defaut, par defaut, on est branche sur PORTA et le positif est sur le PIN 0 et le negatif est sur le PIN 1
    LED(Port port = PA, uint8_t pinPlus = 0, uint8_t pinMoins = 1);

    // methode acces
    Port getPort() const;
    uint8_t getPinPlus() const;
    uint8_t getPinMoins() const;

    void red() const;
    void green() const;
    void off() const;
    // on passe en parametre la dure de l'etat ambre, en unite de nombre de 10 ms
    void ambre(uint16_t nbDixMs = 100) const;

   private:
    Port port_;
    uint8_t pinPlus_;
    uint8_t pinMoins_;
};

#endif