/*
** file: PWM.h
** Declaration de la classe PWM permettant la creation et le controle d'un PWM materiel
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

#ifndef FINALPWM_H
#define FINALPWM_H

#include <avr/io.h>

enum PinPWM { B3, B4};

class PWM {
 public:
  PWM(PinPWM pinPWM, uint8_t pinDirection);

  // Methode d'acces
  bool getDirection() const;
  bool isStop() const;
  uint8_t getPercent() const;
  uint8_t getPinDirection() const;

  // Methode de modification
  void setPercent(uint8_t percent);
  void increase(uint8_t percent);
  void decrease(uint8_t percent);
  void setDirection(bool direction);
  void changeDirection();

 private:
  bool direction_;
  uint8_t percent_;
  uint8_t pinDirection_;

  // Registres et indices a memoriser
  volatile uint8_t* TCCRnA_;
  volatile uint8_t* TCCRnB_;
  volatile uint8_t* OCRnx_;
  volatile uint8_t* PORTx_;
  uint8_t COMnxn_;
};

#endif