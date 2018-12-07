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

// Constructeur qui assigne les deux PWM aux moteurs
Driver::Driver(PWM &leftMotor, PWM &rightMotor)
    : leftMotor_(&leftMotor), rightMotor_(&rightMotor) {}

// Getter qui permet de manipuler directement le moteur gauche
PWM *Driver::getLeftMotor() { return leftMotor_; }

// Getter qui permet de manipuler directement le moteur droit
PWM *Driver::getRightMotor() { return rightMotor_; }

// Fonction qui ne garde que les bits associes au capteur de ligne du PORTA
uint8_t Driver::lineCaptor() { return ((PINA & 0b01111100) >> 2); }

// Fonction principale de suivi de ligne vers l'avant
void Driver::forwardStraight() {
  // On commence par mettre les 2 moteurs vers l'avant
  leftMotor_->setDirection(Forward);
  rightMotor_->setDirection(Forward);

  // On regarde a quoi ressemble la ligne captee
  uint8_t captorState = lineCaptor();

  // Selon l'etat de la ligne, on applique la correction appropriee
  switch (captorState) {
    case hugeLeftDeviation:
      leftMotor_->setPercent(BASIC_SPEED + 25);
      break;

    case bigLeftDeviation:
      leftMotor_->setPercent(BASIC_SPEED+ LEFT_ADJUST + 20);
      break;

    case mediumLeftDeviation:
      leftMotor_->setPercent(BASIC_SPEED+ LEFT_ADJUST + 10);
      break;

    case smallLeftDeviation:
      leftMotor_->setPercent(BASIC_SPEED+ LEFT_ADJUST + 5);
      break;

    case center:
      // LEFT_ADJUST permet d'aller le plus possible en ligne droite si on se
      // trouve au centre
      leftMotor_->setPercent(BASIC_SPEED + LEFT_ADJUST);
      rightMotor_->setPercent(BASIC_SPEED);
      break;

    case smallRightDeviation:
      rightMotor_->setPercent(BASIC_SPEED + 5);
      break;

    case mediumRightDeviation:
      rightMotor_->setPercent(BASIC_SPEED + 5);
      break;

    case bigRightDeviation:
      rightMotor_->setPercent(BASIC_SPEED + 15);
      break;

    case hugeRightDeviation:
      rightMotor_->setPercent(BASIC_SPEED + 20);
      break;

    // Si on ne voit pas de ligne, on continu avec la derniere force, ainsi on
    // pourra revenir sur la ligne eventuellement
    case noLine:
      break;

    // Si on a quelque chose d'imprevu, on va en ligne droite
    default:
      leftMotor_->setPercent(BASIC_SPEED + LEFT_ADJUST);
      rightMotor_->setPercent(BASIC_SPEED);
      break;
  }
}

// Arrete les 2 moteurs en mettant leur puissance a 0
void Driver::stop() {
  leftMotor_->setPercent(0);
  leftMotor_->setDirection(Forward);

  rightMotor_->setPercent(0);
  rightMotor_->setDirection(Forward);
}

// Fonction principale de suivi de ligne vers l'arriere
void Driver::backwardStraight() {
  // On commence par mettre les 2 moteurs vers l'arriere
  leftMotor_->setDirection(Backward);
  rightMotor_->setDirection(Backward);

      leftMotor_->setPercent(BACKWARD_SPEED + LEFT_ADJUST);
      rightMotor_->setPercent(BACKWARD_SPEED);
     
}

// Permet d'aller vers l'avant a une puissance voulue
void Driver::forwardPercent(uint8_t percent) {
  // Met les moteur dans la bonne direction
  leftMotor_->setDirection(Forward);
  rightMotor_->setDirection(Forward);

  // Assigne la bonne puissance au moteurs
  leftMotor_->setPercent(percent);
  rightMotor_->setPercent(percent);
}

// Permet d'aller vers l'arriere a une puissance voulue
void Driver::backwardPercent(uint8_t percent) {
  // Met les moteur dans la bonne direction
  leftMotor_->setDirection(Backward);
  rightMotor_->setDirection(Backward);

  // Assigne la bonne puissance au moteurs
  leftMotor_->setPercent(percent);
  rightMotor_->setPercent(percent);
}

// Permet de tourner en fixant la roue gauche et en reculant avec la droite
void Driver::turnBackwardRight() {
  leftMotor_->setPercent(0);

  // On commence a tourner avec un boost
  boostRightWheel(Backward);
  rightMotor_->setPercent(TURNING_SPEED);
}

// Permet de tourner en fixant la roue droite et en reculant avec la gauche
void Driver::turnBackwardLeft() {
  rightMotor_->setPercent(0);

  boostLeftWheel(Backward);
  leftMotor_->setPercent(TURNING_SPEED + LEFT_ADJUST);
}

// Permet de tourner en pibotant vers la gauche
void Driver::turnPivotLeft() {
  boostPivotLeft();

  // Pour plus de precision, on ajoute l'ajustement et on retire un peu de
  // puissance pour tourner moins rapidement
  leftMotor_->setPercent(TURNING_SPEED + LEFT_ADJUST - 5);
  rightMotor_->setPercent(TURNING_SPEED - 5);
}

// Permet de tourner en pibotant vers la droite
void Driver::turnPivotRight() {
  boostPivotRight();

  // Pour plus de precision, on ajoute l'ajustement et on retire un peu de
  // puissance pour tourner moins rapidement
  leftMotor_->setPercent(TURNING_SPEED + LEFT_ADJUST - 5);
  rightMotor_->setPercent(TURNING_SPEED - 5);
}

// Permet un boost pour une seule roue, puis retour a la vitesse de base
void Driver::boostLeftWheel(Direction direction) {
  // On va dans la direction voulue
  leftMotor_->setDirection(direction);

  // On boost la roue
  leftMotor_->setPercent(100);
  _delay_ms(BOOST_DURATION);

  // On revient a la vitesse de base
  leftMotor_->setPercent(BASIC_SPEED);
}

// Permet un boost pour une seule roue, puis retour a la vitesse de base
void Driver::boostRightWheel(Direction direction) {
  // On va dans la direction voulue
  rightMotor_->setDirection(direction);

  // On boost la roue
  rightMotor_->setPercent(100);
  _delay_ms(BOOST_DURATION);

  // On revient a la vitesse de base
  rightMotor_->setPercent(BASIC_SPEED);
}

// Permet un boost pour les deux roues, puis retour a la vitesse de base
void Driver::boostBothWheel(Direction direction) {
  // On va dans la direction voulue
  leftMotor_->setDirection(direction);
  rightMotor_->setDirection(direction);

  // On boost les roues
  rightMotor_->setPercent(90);
  leftMotor_->setPercent(100);
  _delay_ms(BOOST_DURATION);

  // On revient a la vitesse de base
  rightMotor_->setPercent(BASIC_SPEED);
  leftMotor_->setPercent(BASIC_SPEED);
}

// Permet un boost des roues dans des direction opposees pour le pivot
void Driver::boostPivotLeft() {
  // On va dans la direction voulue
  leftMotor_->setDirection(Backward);
  rightMotor_->setDirection(Forward);

  // On boost les roues
  leftMotor_->setPercent(100);
  rightMotor_->setPercent(100);
  _delay_ms(BOOST_DURATION);
}

// Permet un boost des roues dans des direction opposees pour le pivot
void Driver::boostPivotRight() {
  // On va dans la direction voulue
  leftMotor_->setDirection(Forward);
  rightMotor_->setDirection(Backward);

  // On boost les roues
  leftMotor_->setPercent(100);
  rightMotor_->setPercent(100);
  _delay_ms(BOOST_DURATION);
}

// Dans le cas ou on desire passer a travers une fourche ou une ligne plein
// avant de faire quelque chose d'autre
void Driver::goTroughFork() {
  // Tant qu'on voit une fork ou une ligne pleine, on va en ligne droite
  while (lineCaptor() & forkOrFullLine) {
    leftMotor_->setPercent(BASIC_SPEED + LEFT_ADJUST);
    rightMotor_->setPercent(BASIC_SPEED);
  }
}

// Dans le cas ou on doit avancer a l'aveuglette (sans indice du capteur)
void Driver::forwardBlind() {
  // On va en ligne droite
  leftMotor_->setPercent(BASIC_SPEED + LEFT_ADJUST);
  rightMotor_->setPercent(BASIC_SPEED);
}
