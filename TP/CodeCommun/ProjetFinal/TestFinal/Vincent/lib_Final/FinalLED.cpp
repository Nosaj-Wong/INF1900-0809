/*
** file: FinalLED.h
** Declaration de la classe LED du projet final
** date: 08 novembre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
*/

#include "FinalLED.h"

FinalLED::FinalLED() {}

// Met la LED au rouge
void FinalLED::red() {
  stopCompare();
  // Pin 5 a 0
  PORTD &= ~(1 << 5);
  // Pin 7 a 1
  PORTD |= (1 << 7);
}

// Met la LED au vert
void FinalLED::green() {
  stopCompare();
  // Pin 7 a 0
  PORTD &= ~(1 << 7);
  // Pin 5 a 1
  PORTD |= (1 << 5);
}

// Produit un clignotement 2x/seconde vert de la led materiel
void FinalLED::flashingGreen() {

  // On cesse la comparaison
  stopCompare();
  PORTD &= ~(1 << 7);

  // On active le comparateur A tu du timer 1
  TCCR1A = (1 << COM1A0);

  // On le met en mode CTC et toggle et prescaler 1/64
  TCCR1B = (1 << WGM12 | 1 << CS11 | 1 << CS10);

  // Pas de capture
  TCCR1C = 0;

  // Fait 0.25 secondes a 1/64 de clock
  OCR1A = 31250;
  TCNT1 = 0;
}

// Produit une lueur ambre de facon materielle
void FinalLED::ambre() {
  // On arrete de comparer, on met la led a off
  stopCompare();
  PORTD &= ~(1 << 7 | 1 << 5);

  // On active le comparateur A et on met le mode PWM 8 bit
  // Pour synchroniser les 2 timers utilises
  // On met un des timer qui set en montant et l'autre qui set
  // en descendant (pour qu'ils alternent 0-1/ 1-0)
  TCCR1A = (1 << COM1A1 | 1 << WGM10);
  TCCR2A = (1 << COM2A1 | 1 << COM2A0 | 1 << WGM20);

  // Prescaler 1/8
  TCCR1B = (1 << CS11);
  TCCR2B = (1 << CS21);

  TCCR1C = 0;

  // On met la meme valeur pour qu'ils changent en meme temps
  OCR1A = 190;
  OCR2A = 190;


  // On part les timers a la meme valeur pour le synchronisme
  TCNT1 = 0;
  TCNT2 = 0;
}

// On met les 2 pin a off et on cesse la comparaison
void FinalLED::off() {
  stopCompare();
  PORTD &= ~(1 << 7 | 1 << 5);
}

// On deconnecte tous les comparateurs
void FinalLED::stopCompare() {
  TCCR1A &= ~(1 << COM1A1 | 1 << COM1A0);
  TCCR2A &= ~(1 << COM2A1 | 1 << COM2A0);
}