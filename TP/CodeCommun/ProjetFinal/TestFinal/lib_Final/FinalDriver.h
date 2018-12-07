/*
** file: FinalDriver.h
** Declaration de la classe driver permettant de controler les roues du robot
**
** date: 9 novembre 2018
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

#ifndef FINALDRIVER_H
#define FINALDRIVER_H

#include <util/delay.h>

#include "FinalPWM.h"

enum lineCaptor {
  hugeLeftDeviation = 0b10000,
  bigLeftDeviation = 0b11000,
  mediumLeftDeviation = 0b01000,
  smallLeftDeviation = 0b01100,
  center = 0b00100,
  smallRightDeviation = 0b00110,
  mediumRightDeviation = 0b00010,
  bigRightDeviation = 0b00011,
  hugeRightDeviation = 0b00001,
  forkOrFullLine = 0b10001,
  fullLine = 0b11111,
  noLine = 0b00000
};

// Vitesse de base du robot si centre
const uint8_t BASIC_SPEED = 30;

// Vitesse de base du robot reculan si centre
const uint8_t BACKWARD_SPEED = 40;

// Vitesse de rotations
const uint8_t TURNING_SPEED = 40;

// Duree des boosts lors des depart (ms)
const uint8_t BOOST_DURATION = 100;

// Ajustement des forces du moteur gauche pour equilibrer les moteurs
// (propre a notre robot)
const uint8_t LEFT_ADJUST = 4;

// Simplification pour les fonctiones demandant des direction
enum Direction { Forward = 0, Backward = 1 };

class Driver {
 public:
  Driver(PWM &leftMotor, PWM &rightMotor);

  // Methodes d'acces
  PWM *getLeftMotor();
  PWM *getRightMotor();

  // Methodes de modification
  uint8_t lineCaptor();

  void forwardStraight();
  void backwardStraight();

  void forwardPercent(uint8_t percent);
  void forwardBlind();
  void backwardPercent(uint8_t percent);

  void stop();

  void turnBackwardRight();
  void turnBackwardLeft();
  void turnPivotLeft();
  void turnPivotRight();

  void boostLeftWheel(Direction direction);
  void boostRightWheel(Direction direction);
  void boostBothWheel(Direction direction);
  void boostPivotLeft();
  void boostPivotRight();

  void goTroughFork();

 private:
  PWM *leftMotor_;
  PWM *rightMotor_;
};

#endif
