/*
 * Nom: TP1
 * Description: Programme qui met la LED rouge/ambre/vert quelques secondes chacune
 * Version: 1.1
 */

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include <time.h>

int main()
{
  DDRA = 0xff; // PORT A est en mode sortie

  for (;;) // boucle sans fin
  {
    PORTA = 0x02;
    _delay_ms(3000);
    PORTA = 0x01;
    _delay_ms(3000);
    uint8_t alternateur = 0x01;
    for (int i = 0; i < 300; i++)
    {
      alternateur = ~alternateur;
      PORTA = alternateur;
      _delay_ms(10);
    }
  }
  return 0;
}
