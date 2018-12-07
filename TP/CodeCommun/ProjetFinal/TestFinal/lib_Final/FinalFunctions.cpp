/*
** file: FinalFuntions.cpp
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

#include "FinalFunctions.h"

// Fonction qui cree un delais de 10us
void _my_delay_10us(unsigned long us) {
  for (unsigned long i = 0; i < us; i++) {
    // On met 8 puisque la boucle for elle-meme en prend 2
    _delay_us(8);
  }
}

// Fonction qui produit un signal PWM logiciel sur le port B0 a une frequence,
// duree et pourcentage voulue
void PWM_software(int percentage, double duration, double frequency) {
  const double periode = 1 / frequency;
  const double a = percentage / 100.0 * periode;
  const int nbRepetition = duration / periode;
  const long int nb10usDans1s = 1000000 / 10;

  for (int i = 0; i < nbRepetition; i++) {
    // On met le PWM a 1 pour la duree de A calculee
    PORTB |= 0b11000000;
    _my_delay_10us(a * nb10usDans1s);

    // On met le PWM a 0 pour la duree manquante pour completer B
    PORTB &= ~0b01000000;
    _my_delay_10us((periode - a) * nb10usDans1s);
  }
}

// Fonction qui verifie si le bouton-pressoir usager est actuellement presse
bool buttonPressed() { return (PIND & 0x04); }

// Fonction qui transforme un nombre en son equivalent dans un tableau de char
void intToString(uint32_t numberToTransform, char *string) {
  // Copie du nombre pour manipulations initiales
  uint32_t numberCopy = numberToTransform;

  // Variable qui contiendra la taille du nombre (combien de chiffre)
  uint8_t numberSize = 1;

  // Diviseur qui s'ajustera a la taille du nombre pour les isoler
  uint32_t divider = 1;

  // 1) Determiner la taille du nombre et le diviseur adequat pour isoler le
  // premier chiffre
  // Ex: pour isoler le 5 de 5332, il faut un diviseur valant 1000: 5332/1000=5

  // Si le nombre est plus grand que 10, cela signifie que nous ne somme pas
  // encore rendu au dernier chiffre
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

  // 2) Extraire les nombres 1 par un, les transformer en char et les ajouter
  // au string

  // Nous devons isoler autant de chiffre que la taille du nombre
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

  // Ajout du changement de ligne et du null terminated caracter a la fin du
  // string
  string[numberSize] = '\n';
  string[numberSize + 1] = '\0';
}

// Fonction qui affiche via SerialUSB le tunnel en prenant deux tableaux et
// leur taille en parametre, soit les distances des parties gauche et droite du
// circuit
void printTunnel(uint16_t nElements, uint16_t *leftDistances,
                 uint16_t *rightDistances) {
  // Instancion du transmetteur
  UART transmitter;

  // Meme si nous avons 96 lignes a produire, nous faison 0-95
  const uint8_t TOTAL_LINES = 95;

  // Pour la moitie de la table, nous avons 96 caractere, mais un de celui-ci
  // doit etre le | de la ligne jaune, donc nous faisons -1
  const uint8_t N_HALF_COLUMNS = ((192 / 2) - 1);  /// 95

  // La conversion entre la taille et le nombre de caractere a afficher est
  // simplement 4 (24 pcs =96 caracteres)
  const uint8_t N_CHARACTERS_PER_INCH = 4;

  // Nous stokons les inch sous la forme 1 inch=10, pour eviter les double
  const uint16_t INCH_CONVERSION = 10;

  // Pour les 95 lignes, en commencant par la derniere et en decrementant
  for (int8_t i = TOTAL_LINES; i >= 0; i--) {
    // nElements - 1 est l'indice du dernier element a aller visiter
    // Nous voulons prendre la donnee placee en position proportionnelle par
    // rapport a ou nous en sommes rendu dans notre affichage.
    // Par exemple, si nous sommes rendu au milieu de l'affichage (donc ligne
    // environ 48), nous voulons la donner environ au centre du tableau, donc
    // (indice du dernier element) * 48/95= indice de l'element au centre.
    uint16_t indexDistance =
        (nElements - 1) * i / N_HALF_COLUMNS;  /// i.e. 199 * 95 / 95

    // Distance (en characteres) entre le mur et le centre
    uint16_t leftWalltoCenter = (leftDistances[indexDistance] *
                                 N_CHARACTERS_PER_INCH / INCH_CONVERSION) -
                                1;

    uint16_t rightWalltoCenter = (rightDistances[indexDistance] *
                                  N_CHARACTERS_PER_INCH / INCH_CONVERSION) -
                                 1;

    // Le resultat de "leftDistances[indexDistance] * N_CHARACTERS_PER_INCH /
    // INCH_CONVERSION" pourrait etre inferieur a 1 si la distance mesuree est
    // plus petite que 1/4 de pouce. En faisant -1 on s'expose a une erreur

    // De la meme facon, si notre donnee est plus grande de 24 pouces, il y aura
    // une erreur a l'affichage puisque cette valeur entraine l'affiche de 0
    // caractere de mur. Une plus grande valeur entrainerait l'affichge d'un
    // nombre negatif de caractere = erreur

    if (leftWalltoCenter == 65535)
      leftWalltoCenter = 0;
    else if (leftWalltoCenter > 95)
      leftWalltoCenter = 95;

    if (rightWalltoCenter == 65535)
      rightWalltoCenter = 0;
    else if (rightWalltoCenter > 95)
      rightWalltoCenter = 95;

    // Distance entre le bord de la table et le mur est le complement de la
    // distance entre la table et le mur
    uint16_t leftBorderToWall = N_HALF_COLUMNS - leftWalltoCenter;
    uint16_t rightBorderToWall = N_HALF_COLUMNS - rightWalltoCenter;

    // Afficher la partie gauche: extrémité gauche -> mur
    for (uint8_t nCharacters = 0; nCharacters < leftBorderToWall;
         nCharacters++) {
      transmitter.transmit('@');
    }

    // Afficher la partie gauche: mur -> centre
    for (uint8_t nCharacters = 0; nCharacters < leftWalltoCenter; nCharacters++)
      transmitter.transmit(' ');

    // Afficher la ligne jaune
    transmitter.transmit('|');
    transmitter.transmit('|');

    // Afficher la partie droite: centre -> mur
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

void displayFinalResults(LCM &disp, const uint32_t &lengthLeft,
                         const uint32_t &lengthRight) {
  // Transforme lengthLeft en string et nettoyage de string
  char stringLengthLeft[10];
  intToString(lengthLeft, stringLengthLeft);
  for (int i = 0; i < 10; ++i) {
    // Si le charactere correspond a un chiffre en ASCII
    if (stringLengthLeft[i] < '0' || stringLengthLeft[i] > '9' ) {
      stringLengthLeft[i] = 0;
    }
  }
  // Transforme lengthRight en string et nettoyage de string
  char stringLengthRight[10];
  intToString(lengthRight, stringLengthRight);
  for (int i = 0; i < 10; ++i) {
    // Si le charactere correspond a un chiffre en ASCII
    if (stringLengthRight[i] < '0' || stringLengthRight[i] > '9' ) {
      stringLengthRight[i] = 0;
    }
  }
  // Afficher la longueur des murs
  disp.clear();
  disp.write("GAUCHE : ");
  disp << (stringLengthLeft);
  disp << " cm";
  disp.write("DROITE : ", 16);
  disp << (stringLengthRight);
  disp << " cm";
}
