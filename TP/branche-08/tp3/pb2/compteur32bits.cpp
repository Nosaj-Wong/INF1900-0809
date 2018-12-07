

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

void _my_delay_10us(unsigned long us) {
    for (unsigned long i = 0; i < us; i++) {
        _delay_us(8);
    }
}

void motor_PWM_percentage(int percentage, double seconds, double frequency) {
    const double periode = 1 / frequency;
    const double a = percentage / 100.0 * periode;
    const int nbRepetition = seconds / periode;
    const long int nb10usDans1s = 1000000 / 10;

    for (int i = 0; i < nbRepetition; i++) {
        PORTC = 0b00000011;
        _my_delay_10us(a * nb10usDans1s);
        PORTC = 0b00000010;
        _my_delay_10us((periode - a) * nb10usDans1s);
    }
}

void note(int note, double temps) {
    double hz = 0;
    switch (note) {
        case 1:
            hz = 440 / 2;
            break;
        case 2:
            hz = 493 / 2;
            break;
        case 3:
            hz = 554 / 2;
            break;
        case 4:
            hz = 587 / 2;
            break;
        case 5:
            hz = 659 / 2;
            break;
        case 6:
            hz = 739 / 2;
            break;
        case 7:
            hz = 830 / 2;
            break;
        case 11:
            hz = 440;
            break;
        case 22:
            hz = 493;
            break;
        case 33:
            hz = 554;
            break;
        case 44:
            hz = 587;
            break;
        case 55:
            hz = 659;
            break;
        case 66:
            hz = 739;
            break;
        case 77:
            hz = 830;
            break;
        case 111:
            hz = 880;
            break;
    }
    motor_PWM_percentage(100, temps, hz);
    motor_PWM_percentage(100, 0.1, 0);  //0
}

int main() {
    DDRC = 0xff;

    //Motor
    // motor_PWM_percentage(0, 2, 60);
    // motor_PWM_percentage(25, 2, 60);
    // motor_PWM_percentage(50, 2, 60);
    // motor_PWM_percentage(75, 2, 60);
    // motor_PWM_percentage(100, 2, 60);
    // motor_PWM_percentage(0, 2, 400);
    // motor_PWM_percentage(25, 2, 400);
    // motor_PWM_percentage(50, 2, 400);
    // motor_PWM_percentage(75, 2, 400);
    // motor_PWM_percentage(100, 2, 400);

    //Alarme
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);
    // motor_PWM_percentage(100, 0.2, 800);
    // motor_PWM_percentage(0, 0.2, 800);

    for (;;) {
        note(5, 0.1 * 3);
        note(5, 0.1 * 3);
        note(5, 0.1 * 3);

        note(11, 0.3 * 3);
        note(55, 0.3 * 3);

        note(44, 0.1 * 3);
        note(33, 0.1 * 3);
        note(22, 0.1 * 3);
        note(111, 0.3 * 3);
        note(55, 0.2 * 3);

        note(44, 0.1 * 3);
        note(33, 0.1 * 3);
        note(22, 0.1 * 3);
        note(111, 0.3 * 3);
        note(55, 0.2 * 3);

        note(44, 0.1 * 3);
        note(33, 0.1 * 3);
        note(44, 0.1 * 3);
        note(22, 0.3 * 3);
    }
}
