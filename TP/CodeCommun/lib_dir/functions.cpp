/*
** file: functions.cpp
** Implementation de diverses fonctions utiles
**
** date: 22 octobre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
*/

#include "functions.h"

// Fonction qui cree un delais de 10us
void _my_delay_10us(unsigned long us) {
  for (unsigned long i = 0; i < us; i++) {
    _delay_us(8);
  }
}

// Fonction qui produit un signal PWM logiciel sur le port B0
void PWM_software(int percentage, double duration, double frequency) {
  const double periode = 1 / frequency;
  const double a = percentage / 100.0 * periode;
  const int nbRepetition = duration / periode;
  const long int nb10usDans1s = 1000000 / 10;

  for (int i = 0; i < nbRepetition; i++) {
    PORTB |=  0b11000000;
    _my_delay_10us(a * nb10usDans1s);
    PORTB &= ~0b01000000;
    _my_delay_10us((periode - a) * nb10usDans1s);
  }
}

// Fonction qui met le bouton-pressoir usager en mode interruption
// pour en declencher lors d'une variation du edge choisie
void initializeButtonInterrupt(Edge edge) {
  // cli est une routine qui bloque toutes les interruptions.
  // Il serait bien mauvais d'être interrompu alors que
  // le microcontroleur n'est pas prêt...
  cli();

  // cette procédure ajuste le registre EIMSK
  // de l’ATmega324PA pour permettre les interruptions externes
  EIMSK |= (1 << INT0);

  // il faut sensibiliser les interruptions externes aux
  // changements de niveau du bouton-poussoir choisi
  // en ajustant le registre EICRA
  EICRA &= ~0x03;  // remise a zero des deux premier bits
  EICRA |= edge;

  // sei permet de recevoir à nouveau des interruptions.
  sei();
}

// Fonction qui verifie si le bouton-pressoir usager est actuellement presse
bool buttonPressed() { return (PIND & 0x04); }

// Fonction qui verifie si le bouton-pressoir usager est actuellement presse en
// plus d'un anti-rebond
bool debounceButtonPress() {
  if (buttonPressed()) {
    _delay_ms(30);
    if (buttonPressed()) {
      return true;
    }
  }
  return false;
}

// Fonction qui transforme un nombre en son equivalent dans un tableau de char
void intToString(uint32_t numberToTransform, char *string) {

  // Copie du nombre pour manipulations initiales
  uint32_t numberCopy = numberToTransform;

  // Variable qui contiendra la taille du nombre (combien de chiffre)
  uint8_t numberSize = 1;

  // Diviseur qui s'ajustera a la taille du nombre pour les isoler
  uint32_t divider = 1;

  // ----Determiner la taille du nombre et le diviseur adequat pour isoler le
  // premier chiffre---- Ex: pour isoler le 5 de 5332, il faut un diviseur
  // valant 1000: 5332/1000=5 Si le nombre est plus petit que 10, cela signifie
  // que nous ne somme pas encore rendu au dernier chiffre
  while (numberCopy >= 10) {
    // Donc on le divise par 10 pour lui enlever son dernier chiffre et reduire
    // sa taille
    numberCopy /= 10;

    // Le diviseur devra etre plus gros pour nous permettre d'isoler le premier
    // chiffre
    divider *= 10;

    // La taille du nombre est augmentee
    numberSize++;
  }

  // Variable contenant le chiffre qui est presentement le plus significatif
  uint8_t mostSignificativeNumber;

  // ---- Extraire les nombres 1 par un, les transformer en char et les ajouter
  // au string--- Nous devons isoler autant de chiffre que la taille du nombre
  for (uint8_t i = 0; i < numberSize; i++) {
    // Cette operation isole le chiffre le plus significatif
    // Ex: 5332/1000=5
    mostSignificativeNumber = numberToTransform / divider;

    // Cette operation permet d'eliminer le chiffre le plus significatif
    // Ex: 5332%1000=332
    numberToTransform = numberToTransform % divider;

    // Le diviseur doit diminuer pour s'adapter a la diminution de la taille
    // du nombre a diviser
    divider /= 10;

    // On place dans notre string de sortie ce premier chiffre isole
    // En ASCII, 0 est le premier des nombres, suivi de 1,2,3,etc.
    // Donc si on fait le char 0 + un nombre, on tombe sur la valeur
    // en char de ce nombre, il suffit ensuite de reconvertir en char
    string[i] = char('0' + mostSignificativeNumber);
  }

  //Ajout du changement de ligne et du null terminated caracter a la fin du string
  string[numberSize] = '\n';
  string[numberSize+1] = '\0';
}

// Fonction qui affiche via SerialUSB le tunnel en prenant deux tableaux et
// leur taille en parametre, soit les distances des parties gauche et droite du circuit
void printTunnel(uint16_t nElements, uint16_t *leftDistances,
                 uint16_t *rightDistances)
{

  const uint8_t TOTAL_LINES = 95;
  const uint8_t N_HALF_COLUMNS = ((192 / 2) - 1); /// 95
  const uint8_t N_CHARACTERS_PER_INCH = 4;
  const uint16_t INCH_CONVERSION = 1000;

  UART transmitter;
  for (int8_t i = TOTAL_LINES; i >= 0; i--)
  {

    // nElements - 1: Derniere elements a aller visiter
    uint16_t indexDistance = (nElements - 1) * i / N_HALF_COLUMNS; /// 199 * 95 / 95
    
    // Distance (en characteres) entre le mur et le centre
    uint32_t distanceTemp = leftDistances[indexDistance]; 
    uint16_t leftWalltoCenter = (distanceTemp * N_CHARACTERS_PER_INCH / INCH_CONVERSION) - 1; /// (120 * 4 / 1000)- 1 = -1 (65 535)

    distanceTemp = rightDistances[indexDistance];
    uint16_t rightWalltoCenter = (distanceTemp * N_CHARACTERS_PER_INCH / INCH_CONVERSION) - 1;

    /// Le resultat de "leftDistances[indexDistance] * N_CHARACTERS_PER_INCH / INCH_CONVERSION" pourrait etre
    /// 0 et donnerait -1 apres la soustraction par 1.

    // Il est possible que les variables leftWalltoCenter et/ou rightWalltoCenter soit -1.
    if (leftWalltoCenter == 65535 || rightWalltoCenter == 65535)
    {
      leftWalltoCenter = 0;
      rightWalltoCenter = 0;
    }

    // Distance entre le bord de la table et le mur
    uint16_t leftBorderToWall = N_HALF_COLUMNS - leftWalltoCenter;
    uint16_t rightBorderToWall = N_HALF_COLUMNS - rightWalltoCenter;

    // Afficher la partie gauche: extrémité gauche -> mur
    for (uint8_t nCharacters = 0; nCharacters < leftBorderToWall;
         nCharacters++)
    {
      transmitter.transmit('@');
    }
    // Afficher la partie gauche: mur -> milieu
    for (uint8_t nCharacters = 0; nCharacters < leftWalltoCenter; nCharacters++)
      transmitter.transmit(' ');

    transmitter.transmit('|');
    transmitter.transmit('|');

    // Afficher la partie droite: milieu -> mur
    for (uint8_t nCharacters = 0; nCharacters < rightWalltoCenter;
         nCharacters++)
      transmitter.transmit(' ');
    // Afficher la partie droite: mur -> extrémité droite
    for (uint8_t nCharacters = 0; nCharacters < rightBorderToWall;
         nCharacters++)
      transmitter.transmit('@');

    // Saut de ligne
    transmitter.transmit('\n');
  }
}
