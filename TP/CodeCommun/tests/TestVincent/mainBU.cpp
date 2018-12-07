
#define F_CPU 8000000
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include "FinalLED.h"
#include "can.h"
#include "debug.h"
#include "driver.h"
#include "functions.h"
#include "sing.h"

enum lineCaptor
{
  hugeLeftDeviation = 0b10000,
  bigLeftDeviation = 0b11000,
  mediumLeftDeviation = 0b01000,
  smallLeftDeviation = 0b01100,
  center = 0b00100,
  smallRightDeviation = 0b00110,
  mediumRightDeviation = 0b00010,
  bigRightDeviation = 0b00011,
  hugeRightDeviation = 0b00001,
  leftFork = 0b00111,
  rightFork = 0b11100,
  fullLine = 0b11111,
  noLine = 0b00000
};

uint8_t lineFollower() { return ((PINA & 0b01111100) >> 2); }

int main()
{
  DDRA = 0x80;
  DDRB = 0x3f;
  DDRD = 0xA0;

  PORTA |= 0x80;
  PORTB |= 0x02;
  can can;
  PWM leftMotor(B4, 2, 0, 0);
  PWM rightMotor(B3, 5, 0, 0);
  FinalLED led;
  uint8_t moveCounter = 0;

  Driver driver(leftMotor, rightMotor);
  for (;;)
  {
    if (PIND & 0x04)
    {
      for (;;)
      {
        if (lineFollower() == smallLeftDeviation)
        {
          driver.getLeftMotor()->setPercent(55);
        }

        else if (lineFollower() == smallRightDeviation)
        {
          driver.getRightMotor()->setPercent(55);
        }
        else if (lineFollower() == mediumRightDeviation)
        {
          driver.getRightMotor()->setPercent(60);
        }
        else if (lineFollower() == mediumLeftDeviation)
        {
          driver.getLeftMotor()->setPercent(60);
        }

        else if (lineFollower() == center)
        {
          driver.getLeftMotor()->setPercent(50);
          driver.getRightMotor()->setPercent(50);
        }

        else if (lineFollower() == bigLeftDeviation)
          driver.getLeftMotor()->setPercent(65);

        else if (lineFollower() == bigRightDeviation)
          driver.getRightMotor()->setPercent(65);

        else if (lineFollower() == hugeLeftDeviation)
          driver.getLeftMotor()->setPercent(70);

        else if (lineFollower() == hugeRightDeviation)
          driver.getRightMotor()->setPercent(70);

        else if (lineFollower() == fullLine)
        {
          led.red();
        }

        else if (lineFollower() == noLine)
        {
          if (moveCounter == 0)
          {
            moveCounter++;
            driver.forward(0);
            driver.getRightMotor()->setDirection(1);
            driver.getRightMotor()->setPercent(50);
            while (lineFollower() != center)
            {
              ;
            }
            driver.forward(0);
          }
          else if (moveCounter == 1)
          {
            moveCounter++;
            _delay_ms(1000);
            driver.forward(0);
            driver.getLeftMotor()->setDirection(1);
            driver.getRightMotor()->setPercent(50);
            driver.getLeftMotor()->setPercent(50);
            while (lineFollower() != center &&
                   lineFollower() != smallLeftDeviation &&
                   lineFollower() != smallRightDeviation)
            {
              ;
            }

            driver.forward(0);
            _delay_ms(500);
          }
        }

        else
        {
          driver.getLeftMotor()->setPercent(50);
          driver.getRightMotor()->setPercent(50);
        }
      }
    }
  }
}