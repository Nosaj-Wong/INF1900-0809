/*
** file: FinalPWM.cpp
** Implementation de la classe PWM permettant la creation et le controle d'un
**  PWM materiel
**
** date: 09 novembre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
*/

#include "FinalPWM.h"

// Le constructeur prend en parametre le port de sortie du PWM au format Dn
// (convention ou pin 0 existe), la pin de direction au format n (convention ou
// pin 0 existe), la direction (ou 0 signifie avant et 1 arriere) et le
// poucentage de puissance entre 0 et 100
PWM::PWM(PinPWM pinPWM, uint8_t pinDirection)
    : direction_(0),
      percent_(0),
      pinDirection_(pinDirection) {
        
  // Selon le port du PWM, attribution de la bonne correspondance aux
  // registres-attributs de la classe
  switch (pinPWM) {
    case B3:
      OCRnx_ = &OCR0A;
      COMnxn_ = COM0A0;
      break;

    case B4:
      OCRnx_ = &OCR0B;
      COMnxn_ = COM0B0;
      break;
  }

  // Modification des registres

  // PWM 8 bit phase correcte et activation du mode compare en downcounting
  TCCR0A |= (0b10 << COMnxn_) | (1 << WGM10);

  // Prescaler a 1/8
  TCCR0B |= (1 << CS11);

  // Miste du registre de comparaison au nombre souhaite
  *OCRnx_ = (255 * percent_) / 100;

  // Mise de la pin de direction a la direction souhaitee
  PORTB |= (direction_ << pinDirection_);
}

// Methodes d'acces
bool PWM::getDirection() const { return direction_; }

uint8_t PWM::getPercent() const { return percent_; }

uint8_t PWM::getPinDirection() const { return pinDirection_; }


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

void PWM::setDirection(bool direction) {
  direction_ = direction;
  if (direction_ == true)
    PORTB |= (1 << pinDirection_);
  else if (direction_ == false)
    PORTB &= ~(1 << pinDirection_);
}

void PWM::changeDirection() {
  direction_ = !direction_;
  PORTB ^= (1 << pinDirection_);
}
