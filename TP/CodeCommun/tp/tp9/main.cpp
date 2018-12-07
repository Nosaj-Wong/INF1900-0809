/*
** file: main.cpp
** Programme permettant d'executer une serie d'actions codes en memoire externe
**
** date: 1 novembre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
** Configuration: Led usager en A0/A1
**                Moteur gauche PWM B4, dir B2
**                Moteur droit  PWM B3, dir B5
**                Haut-Parleur  D7/D5
*/


#include <avr/io.h>
#include <string.h>

#include "FinalLED.h"
#include "can.h"
#include "debug.h"
#include "FinalDriver.h"
#include "functions.h"
#include "sing.h"

// enum pour simplifier la lecture des instructions
enum instruction {
  Begin = 0x01,
  Wait = 0x02,
  LedOn = 0x44,
  LedOff = 0x45,
  SoundOn = 0x48,
  SoundOff = 0x09,
  MotorOffA = 0x60,
  MotorOffB = 0x61,
  Forward = 0x62,
  Backward = 0x63,
  RightTurn = 0x64,
  LeftTurn = 0x65,
  BegLoop = 0xC0,
  EndLoop = 0xC1,
  End = 0xFF
};

int main() {
  // Initialisation des entrees/sorties

  // LED
  DDRA = 0x03;
  // Moteurs
  DDRB = 0x3C;
  // Memoire Externe
  DDRC = 0x00;
  // Haut-Parleur
  DDRD = 0xA0;

  // -------Initialisation des variables locales-------
  // Taille de fichier
  uint16_t fileSize;

  // Gestion du compte d'instruction et des boucles
  uint16_t progCounter = 0x00;
  uint16_t loopAdress;
  uint8_t loopCounter;

  // Variable de stockage de l'instruction et de l'operande en cours
  uint8_t instruction;
  uint8_t operand;

  // Variable affectee par l'instruction de debut (voir apres le switch case)
  bool isActive = false;

  // -------Instanciation des objets utiles au probleme------
  // La communication avec la memoire externe
  Memoire24CXXX memory;

  // Le controle de la LED
  LED led(PA, 0, 1);

  // Les PWM pour les 2 roues et la direction
  PWM leftWheel(B4, 2, 0, 0);
  PWM rightWheel(B3, 5, 0, 0);
  Driver driver(leftWheel, rightWheel);

  // Le SingTimer pour jouer les sonorites
  SingTimer singer;

  // ----Gestion des 2 premiers octets pour obtenir la taille du fichier----
  memory.lecture(progCounter, &instruction);
  progCounter++;
  fileSize = instruction;

  memory.lecture(progCounter, &instruction);
  progCounter++;

  // Les 8 premiers bits sont mis au poids fort, les 8 suivants au poids faible
  fileSize = (fileSize << 8) + instruction;

  DEBUG_PRINT(("Nombre d'instruction comptee\n"));

  // Signal lumineux pour indiquer la completion des etapes initiales. On
  // choisit le rouge pour le differencier des signaux verts du switch case
  led.red();
  _delay_ms(2000);
  led.off();

  //-----Debut du programme d'interpretation des instructions----

  // Considerant que nous commencons a l'adresse 0x00, nous finirons la
  // lecture en memoire avec avoir parcouru la taille du fichier en memoire
  while (progCounter < fileSize) {
    // Lecture de l'instruction et de son operande
    memory.lecture(progCounter, &instruction);
    progCounter++;

    memory.lecture(progCounter, &operand);
    progCounter++;

    // Nous executons l'instruction seulement si on a rencontrer dbt (gestion
    // apres le switch case)
    if (isActive) {
      switch (instruction) {
        case Wait:
          DEBUG_PRINT(("J'attends, c'est long!\n"));
          for (uint8_t i = 0; i < operand; i++) {
            _delay_ms(25);
          }
          break;

        case LedOn:
          DEBUG_PRINT(("J'allume la lumiere!\n"));
          led.green();
          break;

        case LedOff:
          DEBUG_PRINT(("Mais qui a eteint la lumiere!\n"));
          led.off();
          break;

        case SoundOn:

          if (operand >= 45 && operand <= 81) {
            DEBUG_PRINT(("C'est ma toune!\n"));
            singer.note(operand);
          }
          break;

        case SoundOff:
          DEBUG_PRINT(("On arrete la musique!\n"));
          singer.stop();
          break;

        case MotorOffA:
          DEBUG_PRINT(("Je fais mon stop!\n"));

          driver.forward(0);
          break;

        case MotorOffB:
          DEBUG_PRINT(("Je fais mon stop!\n"));
          driver.forward(0);
          break;

        case Forward:
          DEBUG_PRINT(("En avant, marche!\n"));
          driver.forward(operand * 100 / 255);
          break;

        case Backward:
          DEBUG_PRINT(("Bip Bip Bip Bip!\n"));
          driver.backward(operand * 100 / 255);
          break;

        case RightTurn:
          DEBUG_PRINT(("A tribord toute!\n"));
          // Reinitialisation de la direction et on attend de tomber a l'arret
          // avant de tourner
          driver.forward(0);
          _delay_ms(750);

          driver.turnRight90();

          // On attend de tomber a l'arret avant de faire le prochain mouvement
          _delay_ms(750);
          break;

        case LeftTurn:
          DEBUG_PRINT(("A babord toute!\n"));
          // Reinitialisation de la direction et on attend de tomber a l'arret
          // avant de tourner
          driver.forward(0);
          _delay_ms(750);

          driver.turnLeft90();

          // On attend de tomber a l'arret avant de faire le prochain mouvement
          _delay_ms(750);
          break;

        case BegLoop:
          DEBUG_PRINT(("On va avoir un impression de deja vu!\n"));

          // progCounter contient presentement l'adresse de la prochaine
          // instruction et c'est exactement la que nous souhaitons reprendre
          // notre boucle
          loopAdress = progCounter;
          loopCounter = operand;
          break;

        case EndLoop:
          DEBUG_PRINT(("Retour vers le passe!\n"));
          if (loopCounter > 0) {
            // On retourne a l'adresse qui suivait le debut de la boucle
            progCounter = loopAdress;
            loopCounter--;
          }
          break;

        case End:
          DEBUG_PRINT(("Terminus, tout le monde descend!\n"));
          // Forcera la sortie de la boucle while en simulant que nous avons lu
          // toutes les instructions
          progCounter = fileSize;

          driver.stop();
          singer.stop();
          led.off();
          break;
      }
    }
    // Mis a la fin pour eviter de parcourir le switch case inutilement la
    // premiere fois
    if (instruction == Begin) {
      isActive = true;
      DEBUG_PRINT(
          ("Attention, j'ai lu le signal de debut, je suis pres pour "
           "l'action!\n"));
    }
  }
  return 0;
}