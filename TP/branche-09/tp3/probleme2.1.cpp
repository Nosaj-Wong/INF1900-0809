/*
 * Nom: Problème 2: Contrôle des moteurs du robot
 * Description: Ceci est un programme variant la led et le pwm du moteur du
 * robot. Version: 1.0
 */
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

enum PWM  // etats du pwm
{ OFF,
  PWM60HZ,
  PWM400HZ,
  END };

PWM pwm = OFF;
void directionPortD()  // PORT D est en mode sortie (sauf D2)
{
  DDRD = 0x03;
}
void directionPortA()  // PORT A est en mode sortie
{
  DDRA = 0xff;
}

void ledOff()  // fonction eteint la LED
{
  PORTA = 0x00;
}

void ledOn()  // fonction allume la LED
{
  PORTA = 0x01;
}

void motorOff()  // fonction eteint le moteur
{
  PORTD = 0x00;
}

void motorOn()  // fonction fait patir le moteur
{
  PORTD = 0x01;
}

void buttonPress()  // fonction verifie si le bouton a ete pese
{
  if (PIND & 0x04)  // D2
    _delay_ms(10);  // anti-rebond
  if (PIND & 0x04)
    pwm = PWM60HZ;  // if button pressed
  else
    pwm = OFF;
}

void delayModifier167(int repetitions)  // fonction modifiant le delay
{
  for (int i = 0; i < repetitions; i++)  //
  {
    _delay_us(167);
  }
}
void delayModifier25(int repetitions)  // fonction modifiant le delay
{
  for (int i = 0; i < repetitions; i++)  //
  {
    _delay_us(25);
  }
}

void pwmModifier60()  // fonction impose des fréquences de 60 Hz pour la
                      // fréquence de la période du signal PWM
{
  int a = 0;
  int b = 100;
  int NBR_LEVELS_PWM = 5;
  int NBR_CYCLES = 120;

  for (int i = 0; i < NBR_LEVELS_PWM; i++) {
    for (int i = 0; i < NBR_CYCLES; i++)  // 120*100*167 = ~2 secondes
    {
      motorOn();
      ledOn();
      delayModifier167(a);
      motorOff();
      ledOff();
      delayModifier167(b - a);
    }
    a += 25;
  }
}

void pwmModifier400()  // fonction impose des fréquences de 400 Hz pour la
                       // fréquence de la période du signal PWM pendant 1
                       // seconde
{
  int a = 0;
  int b = 100;
  int NBR_LEVELS_PWM = 5;
  int NBR_CYCLES = 800;

  for (int i = 0; i < NBR_LEVELS_PWM;
       i++)  // 5 niveaux : 0%, 25%, 50%, 75% et 100%
  {
    for (int i = 0; i < NBR_CYCLES; i++)  // 800*100*25= 2 secondes
    {
      motorOn();
      ledOn();
      delayModifier25(a);
      motorOff();
      ledOff();
      delayModifier25(b - a);
    }
    a += 25;
  }
}

int main() {
  directionPortD();  // PORT D est en mode sortie (sauf D2)
  directionPortA();  // PORT A est en mode sortie

  for (;;)  // boucle sans fin
  {
    // machine: PWM
    switch (pwm) {
      case OFF:
        motorOff();
        ledOff();
        buttonPress();
        break;

      case PWM60HZ:
        pwmModifier60();
        pwm = PWM400HZ;

      case PWM400HZ:
        pwmModifier400();
        pwm = END;
      case END:
        motorOff();
        ledOff();
        pwm = END;
        break;
    }
  }
  return 0;
}
