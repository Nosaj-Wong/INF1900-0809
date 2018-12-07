/*
** file: LED.h
** Implementation de la classe LED permettant le controle d'une LED
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

#include "LED.h"

LED::LED(Port port, uint8_t pinPlus, uint8_t pinMoins) {
    port_ = port;
    // si pinPlus est 3, pinPlus_ est 1 << 3 donc 00000100, on cree un masque pour faire des operations sur le 3e bit
    pinPlus_ = 1 << pinPlus;
    // si pinPlus est 4, pin_ est 1 << 4 donc 00001000, on cree un masque pour faire des operations sur le 4e bit
    pinMoins_ = 1 << pinMoins;
}

// methode acces
Port LED::getPort() const{
    return port_;
}
uint8_t LED::getPinPlus() const{
    return pinPlus_;
}
uint8_t LED::getPinMoins() const{
    return pinMoins_;
}

// methode modification
void LED::red() const {
    switch (port_) {
        case A:
            // on cherche a mettre le pinPlus a 1 sans modifier les autres pins
            // xxxxxxxx |= 00000100 est egale a
            // xxxxx1xx
            PORTA |= pinPlus_;
            // on cherche a mettre le pinMoins a 0 sans modifier les autres pins
            // xxxxxxxx &= ~(00001000) est egale a
            // xxxxxxxx &= 11110111 est egale a
            // xxxx0xxx
            PORTA &= ~pinMoins_;
            break;
        case B:
            PORTB |= pinPlus_;
            PORTB &= ~pinMoins_;
            break;
        case C:
            PORTC |= pinPlus_;
            PORTC &= ~pinMoins_;
            break;
        case D:
            PORTD |= pinPlus_;
            PORTD &= ~pinMoins_;
            break;
    }
}

void LED::green() const {
    switch (port_) {
        case A:
            PORTA |= pinMoins_;
            PORTA &= ~pinPlus_;
            break;
        case B:
            PORTB |= pinMoins_;
            PORTB &= ~pinPlus_;
            break;
        case C:
            PORTC |= pinMoins_;
            PORTC &= ~pinPlus_;
            break;
        case D:
            PORTD |= pinMoins_;
            PORTD &= ~pinPlus_;
            break;
    }
}

void LED::off() const {
    switch (port_) {
        case A:
            PORTA &= ~pinMoins_;
            PORTA &= ~pinPlus_;
            break;
        case B:
            PORTB &= ~pinMoins_;
            PORTB &= ~pinPlus_;
            break;
        case C:
            PORTC &= ~pinMoins_;
            PORTC &= ~pinPlus_;
            break;
        case D:
            PORTD &= ~pinMoins_;
            PORTD &= ~pinPlus_;
            break;
    }
}

void LED::ambre(uint8_t nbDixMs) const {
    for (int i = 0; i < nbDixMs; i++) {
        red();
        _delay_ms(3);
        green();
        _delay_ms(7);
        off();
    }
}