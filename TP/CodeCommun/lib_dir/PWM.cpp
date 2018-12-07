/*
** file: PWM.cpp
** Implementation de la classe PWM permettant la creation et le controle d'un
*  PWM materiel
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

#include "PWM.h"

// Le constructeur prend en parametre le port de sortie du PWM au format Dn
// (convention ou pin 0 existe), la pin de direction au format n (convention ou
// pin 0 existe), la direction (ou 0 signifie avant et 1 arriere) et le
// poucentage de puissance entre 0 et 100
PWM::PWM(PinPWM pinPWM, uint8_t pinDirection, bool direction, uint8_t percent)
    : direction_(direction),
      isStop_(false),
      percent_(percent),
      pinDirection_(pinDirection) {
  // Selon le port du PWM, attribution de la bonne correspondance aux
  // registres-attributs de la classe
  switch (pinPWM) {
    case B3:
      TCCRnA_ = &TCCR0A;
      TCCRnB_ = &TCCR0B;
      OCRnx_ = &OCR0A;
      COMnxn_ = COM0A0;
      PORTx_ = &PORTB;
      break;

    case B4:
      TCCRnA_ = &TCCR0A;
      TCCRnB_ = &TCCR0B;
      OCRnx_ = &OCR0B;
      COMnxn_ = COM0B0;
      PORTx_ = &PORTB;
      break;

    case D4:
      TCCRnA_ = &TCCR1A;
      TCCRnB_ = &TCCR1B;
      TCCRnC_ = &TCCR1C;
      OCRnx_ = &OCR1BL;
      COMnxn_ = COM1B0;
      PORTx_ = &PORTD;
      break;

    case D5:
      TCCRnA_ = &TCCR1A;
      TCCRnB_ = &TCCR1B;
      TCCRnC_ = &TCCR1C;
      OCRnx_ = &OCR1AL;
      COMnxn_ = COM1A0;
      PORTx_ = &PORTD;
      break;

    case D6:
      TCCRnA_ = &TCCR2A;
      TCCRnB_ = &TCCR2B;
      OCRnx_ = &OCR2B;
      COMnxn_ = COM2B0;
      PORTx_ = &PORTD;
      break;

    case D7:
      TCCRnA_ = &TCCR2A;
      TCCRnB_ = &TCCR2B;
      OCRnx_ = &OCR2A;
      COMnxn_ = COM2A0;
      PORTx_ = &PORTD;
      break;
  }

  // Modification des registres

  // PWM 8 bit phase correcte et activation du mode compare en downcounting
  *TCCRnA_ |= (0b10 << COMnxn_) | (1 << WGM10);

  // Prescaler a 1/8
  *TCCRnB_ |= (1 << CS11);

  // Mise du TCCR1C a 0 (existe seulement pour timer1)
  if (pinPWM == D4 || pinPWM == D5) {
    *TCCRnC_ = 0;
  }

  // Miste du registre de comparaison au nombre souhaite
  *OCRnx_ = (255 * percent_) / 100;

  // Mise de la pin de direction a la direction souhaitee
  *PORTx_ |= (direction_ << pinDirection_);
}

// Methodes d'acces
bool PWM::getDirection() const { return direction_; }

uint8_t PWM::getPercent() const { return percent_; }

uint8_t PWM::getPinDirection() const { return pinDirection_; }

bool PWM::isStop() const { return isStop_; }

// Methodes de modification
void PWM::setPercent(uint8_t percent) {
  percent_ = percent;
  *OCRnx_ = (255 * percent_) / 100;
}

void PWM::increase(uint8_t percent) {
  percent_ += percent;
  if (percent_ > 100) {
    percent_ = 100;
  }
  *OCRnx_ = (255 * percent_) / 100;
}

void PWM::decrease(uint8_t percent) {
  if (percent_ < percent) {
    percent_ = 0;
  } else
    percent_ -= percent;
  *OCRnx_ = (255 * percent_) / 100;
}

void PWM::stop() {
  isStop_ = true;
  *TCCRnA_ &= ~(0b10 << COMnxn_);
}

void PWM::resume() {
  isStop_ = false;
  *TCCRnA_ |= (0b10 << COMnxn_);
}

void PWM::setDirection(bool direction) {
  direction_ = direction;
  if (direction_ == true)
    *PORTx_ |= (1 << pinDirection_);
  else if (direction_ == false)
    *PORTx_ &= ~(1 << pinDirection_);
}

void PWM::changeDirection() {
  direction_ = !direction_;
  *PORTx_ ^= (1 << pinDirection_);
}
