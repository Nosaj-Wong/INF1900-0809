/*
** file: driver.h
** Declaration de la classe driver permettant de controler les roues du robot
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

#ifndef DRIVER_H
#define DRIVER_H

#include <util/delay.h>

#include "PWM.h"

class Driver
{
public:
  Driver(PWM &leftMotor, PWM &rightMotor);

  // Methodes d'acces
  PWM* getLeftMotor();
  PWM* getRightMotor();

  // Methodes de modification
  void stop();
  void resume();

  void forward(uint8_t percent);
  void backward(uint8_t percent);

  void turnLeft90();
  void turnRight90();
  void turn180();

private:
  PWM* leftMotor_;
  PWM* rightMotor_;
};

#endif