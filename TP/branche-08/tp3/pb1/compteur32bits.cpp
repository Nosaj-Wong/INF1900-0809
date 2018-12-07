//Chez AC
//XiChen
//bottonPressed -> Rouge, sinon noir.

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

bool buttonPressed() {
    if (PIND & 0x04) {
        _delay_ms(10);
        if (PIND & 0x04) {
            return 1;
        }
    }
    return 0;
}

void lightGreen() {
    PORTC = 0x01;
}

void lightRed() {
    PORTC = 0x02;
}

void lightOff() {
    PORTC = 0x00;
}

void _my_delay_10us(unsigned long us) {
    for (unsigned long i = 0; i < us; i++) {
        _delay_us(8);
    }
}

int main() {
    DDRC = 0xff;
    DDRD = 0x00;
    const unsigned int PalierTroisSecondes = 100;
    const unsigned int NbRepParPalier = 30;

    for (;;) {
        if (buttonPressed()) {
            while (buttonPressed()) {
                lightRed();
                _delay_ms(10);
            }
            for (unsigned long compteur = 0; compteur <= PalierTroisSecondes; compteur++) {
                for (unsigned int compteur2 = 0; compteur2 < NbRepParPalier; compteur2++) {
                    lightGreen();
                    _my_delay_10us(100 - compteur);
                    lightOff();
                    _my_delay_10us(compteur);
                }
            }
        }
    }
}
