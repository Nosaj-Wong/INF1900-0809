/*
** file: driver.cpp
** Implementation de la classe driver permettant de controler les roues du robot
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

#include "driver.h"

Driver::Driver(PWM &leftMotor, PWM &rightMotor)
    : leftMotor_(&leftMotor), rightMotor_(&rightMotor) {}

PWM* Driver::getLeftMotor() { return leftMotor_; }

PWM* Driver::getRightMotor() { return rightMotor_; }

void Driver::stop() {
  leftMotor_->stop();
  rightMotor_->stop();
}

void Driver::resume() {
  leftMotor_->resume();
  rightMotor_->resume();
}

void Driver::forward(uint8_t percent) {
  leftMotor_->setDirection(0);
  rightMotor_->setDirection(0);

  leftMotor_->setPercent(percent);
  rightMotor_->setPercent(percent);
}

void Driver::backward(uint8_t percent) {
  leftMotor_->setDirection(1);
  rightMotor_->setDirection(1);

  leftMotor_->setPercent(percent);
  rightMotor_->setPercent(percent);
}

void Driver::turnLeft90() {
  uint8_t previousPercent = rightMotor_->getPercent();
  leftMotor_->stop();
  rightMotor_->setPercent(75);

  _delay_ms(1150);

  leftMotor_->resume();
  rightMotor_->setPercent(previousPercent);
}

void Driver::turnRight90() {
  uint8_t previousPercent = leftMotor_->getPercent();
  rightMotor_->stop();
  leftMotor_->setPercent(75);

  _delay_ms(1150);

  rightMotor_->resume();
  leftMotor_->setPercent(previousPercent);
}
void Driver::turn180() {
  uint8_t previousPercentLeft = leftMotor_->getPercent();
  uint8_t previousPercentRight = rightMotor_->getPercent();

  bool previousDirectionLeft = leftMotor_->getDirection();
  bool previousDirectionRight = rightMotor_->getDirection();

  leftMotor_->setDirection(0);
  rightMotor_->setDirection(1);

  leftMotor_->setPercent(75);
  rightMotor_->setPercent(75);

  _delay_ms(1500);

  leftMotor_->setDirection(previousDirectionLeft);
  rightMotor_->setDirection(previousDirectionRight);

  leftMotor_->setPercent(previousPercentLeft);
  rightMotor_->setPercent(previousPercentRight);
}