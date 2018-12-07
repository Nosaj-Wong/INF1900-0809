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
    const unsigned int PalierTroisSecondes = 3000;

    for (;;) {
        if (buttonPressed()) {
            while (buttonPressed()) {
                lightRed();
                _delay_ms(10);
            }
            unsigned int a = 100;
            unsigned int bma = 0;
            for (unsigned long compteur = 1; compteur <= PalierTroisSecondes; compteur++) {
                lightGreen();
                _my_delay_10us(b);
                lightOff();
                _my_delay_10us(bma);
                if((compteur%3) = 0 ){
                    a--;
                    bma++;
                }
            }
        }
    }
}
