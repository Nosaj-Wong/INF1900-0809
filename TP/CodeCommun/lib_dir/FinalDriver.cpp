/*
** file: FinalDriver.cpp
** Implementation de la classe driver permettant de controler les roues du robot
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

#include "FinalDriver.h"

Driver::Driver(PWM &leftMotor, PWM &rightMotor)
    : leftMotor_(&leftMotor), rightMotor_(&rightMotor) {}

PWM *Driver::getLeftMotor() { return leftMotor_; }

PWM *Driver::getRightMotor() { return rightMotor_; }

void Driver::forwardStraight() {
  leftMotor_->setDirection(Forward);
  rightMotor_->setDirection(Forward);

  uint8_t captorState = lineCaptor();

  switch (captorState) {
    case hugeLeftDeviation:
      leftMotor_->setPercent(BASICSPEED + 25);
      break;

    case bigLeftDeviation:
      leftMotor_->setPercent(BASICSPEED + 20);
      break;

    case mediumLeftDeviation:
      leftMotor_->setPercent(BASICSPEED + 15);
      break;

    case smallLeftDeviation:
      leftMotor_->setPercent(BASICSPEED + 10);
      break;

    case center:
      leftMotor_->setPercent(BASICSPEED);
      rightMotor_->setPercent(BASICSPEED);
      break;

    case smallRightDeviation:
      rightMotor_->setPercent(BASICSPEED + 10);
      break;

    case mediumRightDeviation:
      rightMotor_->setPercent(BASICSPEED + 15);
      break;

    case bigRightDeviation:
      rightMotor_->setPercent(BASICSPEED + 20);
      break;

    case hugeRightDeviation:
      rightMotor_->setPercent(BASICSPEED + 25);
      break;

    case noLine:
      break;

    default:
      leftMotor_->setPercent(BASICSPEED);
      rightMotor_->setPercent(BASICSPEED);
      break;
  }
}

void Driver::stop() {
  leftMotor_->setPercent(0);
  leftMotor_->setDirection(Forward);

  rightMotor_->setPercent(0);
  rightMotor_->setDirection(Forward);
}

void Driver::backwardStraight() {
  leftMotor_->setDirection(Backward);
  rightMotor_->setDirection(Backward);

  uint8_t captorState = lineCaptor();

    switch (captorState) {
    case hugeLeftDeviation:
      leftMotor_->setPercent(BASICSPEED - 15);
      break;

    case bigLeftDeviation:
      leftMotor_->setPercent(BASICSPEED - 15);
      break;

    case mediumLeftDeviation:
      leftMotor_->setPercent(BASICSPEED - 15);
      break;

    case smallLeftDeviation:
      leftMotor_->setPercent(BASICSPEED - 15);
      break;

    case center:
      leftMotor_->setPercent(BASICSPEED);
      rightMotor_->setPercent(BASICSPEED);
      break;

    case smallRightDeviation:
      rightMotor_->setPercent(BASICSPEED - 15);
      break;

    case mediumRightDeviation:
      rightMotor_->setPercent(BASICSPEED - 15);
      break;

    case bigRightDeviation:
      rightMotor_->setPercent(BASICSPEED - 15);
      break;

    case hugeRightDeviation:
      rightMotor_->setPercent(BASICSPEED - 15);
      break;

    case noLine:
      break;

    default:
      leftMotor_->setPercent(BASICSPEED);
      rightMotor_->setPercent(BASICSPEED);
      break;
  }
}

void Driver::forwardPercent(uint8_t percent) {
  leftMotor_->setDirection(Forward);
  rightMotor_->setDirection(Forward);

  leftMotor_->setPercent(percent);
  rightMotor_->setPercent(percent);
}

void Driver::backwardPercent(uint8_t percent) {
  leftMotor_->setDirection(Backward);
  rightMotor_->setDirection(Backward);

  leftMotor_->setPercent(percent);
  rightMotor_->setPercent(percent);
}

void Driver::turnBackwardRight() {
  leftMotor_->setPercent(0);

  boostRightWheel(Backward);
  rightMotor_->setPercent(TURNINGSPEED);
}

void Driver::turnBackwardLeft() {
  rightMotor_->setPercent(0);

  boostLeftWheel(Backward);
  leftMotor_->setPercent(TURNINGSPEED);
}

void Driver::turnPivotLeft() {
  boostPivotLeft();

  leftMotor_->setPercent(TURNINGSPEED);
  rightMotor_->setPercent(TURNINGSPEED);
}

void Driver::turnPivotRight() {
  boostPivotRight();
  leftMotor_->setPercent(TURNINGSPEED);
  rightMotor_->setPercent(TURNINGSPEED);
}

void Driver::boostLeftWheel(Direction direction) {
  leftMotor_->setDirection(direction);
  leftMotor_->setPercent(100);
  _delay_ms(BOOSTDURATION);
  leftMotor_->setPercent(BASICSPEED);
}

void Driver::boostRightWheel(Direction direction) {
  rightMotor_->setDirection(direction);
  rightMotor_->setPercent(100);

  _delay_ms(BOOSTDURATION);

  rightMotor_->setPercent(BASICSPEED);
}

void Driver::boostBothWheel(Direction direction) {
  leftMotor_->setDirection(direction);
  rightMotor_->setDirection(direction);

  rightMotor_->setPercent(100);
  leftMotor_->setPercent(100);

  _delay_ms(BOOSTDURATION);

  rightMotor_->setPercent(BASICSPEED);
  leftMotor_->setPercent(BASICSPEED);
}
void Driver::boostPivotLeft() {
  leftMotor_->setDirection(Backward);
  rightMotor_->setDirection(Forward);

  leftMotor_->setPercent(100);
  rightMotor_->setPercent(100);

  _delay_ms(BOOSTDURATION);
}

void Driver::boostPivotRight() {
  leftMotor_->setDirection(Forward);
  rightMotor_->setDirection(Backward);

  leftMotor_->setPercent(100);
  rightMotor_->setPercent(100);

  _delay_ms(BOOSTDURATION);
}

void Driver::goTroughFork() {
  while (lineCaptor() & forkOrFullLine) {
    forwardStraight();
  }
}

uint8_t Driver::lineCaptor() { return ((PINA & 0b01111100) >> 2); }