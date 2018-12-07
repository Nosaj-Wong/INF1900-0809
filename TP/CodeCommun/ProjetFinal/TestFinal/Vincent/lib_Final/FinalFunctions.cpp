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
  const long int secTo10us = 1000000 / 10;

  for (int i = 0; i < nbRepetition; i++) {
    // On met le PWM a 1 pour la duree de A calculee
    PORTB |= 0b11000000;
    _my_delay_10us(a * secTo10us);

    // On met le PWM a 0 pour la duree manquante pour completer B
    PORTB &= ~0b01000000;
    _my_delay_10us((periode - a) * secTo10us);
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
void printTunnel(uint32_t nElements, uint16_t *leftDistances,
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
        (nElements - 1) * i * 10 / TOTAL_LINES;  /// i.e. 199 * 95 / 95

    if (indexDistance % 10 >= 5) indexDistance += 10;
    indexDistance /= 10;

// 1 -> affichage originale de Simon
// 0 -> affichage avec arrondissement vers l'unite plus pres
#if (0)
    // Distance (en characteres) entre le mur et le centre
    uint16_t leftWalltoCenter = (leftDistances[indexDistance] *
                                 N_CHARACTERS_PER_INCH / INCH_CONVERSION) -
                                1;

    uint16_t rightWalltoCenter = (rightDistances[indexDistance] *
                                  N_CHARACTERS_PER_INCH / INCH_CONVERSION) -
                                 1;
#else
    uint16_t leftWalltoCenter =
        (leftDistances[indexDistance] * N_CHARACTERS_PER_INCH) - 10;
    if (leftWalltoCenter % 10 >= 5) {
      leftWalltoCenter = (leftWalltoCenter + 10) / INCH_CONVERSION;
    } else {
      leftWalltoCenter /= INCH_CONVERSION;
    }

    uint16_t rightWalltoCenter =
        (rightDistances[indexDistance] * N_CHARACTERS_PER_INCH) - 10;
    if (rightWalltoCenter % 10 >= 5) {
      rightWalltoCenter = (rightWalltoCenter + 10) / INCH_CONVERSION;
    } else {
      rightWalltoCenter /= INCH_CONVERSION;
    }
#endif

    // Le resultat de "leftDistances[indexDistance] * N_CHARACTERS_PER_INCH /
    // INCH_CONVERSION" pourrait etre inferieur a 1 si la distance mesuree est
    // plus petite que 1/4 de pouce. En faisant -1 on s'expose a une erreur

    // De la meme facon, si notre donnee est plus grande de 24 pouces, il y
    // aura une erreur a l'affichage puisque cette valeur entraine l'affiche
    // de 0 caractere de mur. Une plus grande valeur entrainerait l'affichge
    // d'un nombre negatif de caractere = erreur

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
    // Si le charactere correspond a un saut de ligne \n
    if (stringLengthLeft[i] == '\n') {
      stringLengthLeft[i] = '\0';
      break;
    }
  }
  // Transforme lengthRight en string et nettoyage de string
  char stringLengthRight[10];
  intToString(lengthRight, stringLengthRight);
  for (int i = 0; i < 10; ++i) {
    // Si le charactere correspond a un saut de ligne \n
    if (stringLengthRight[i] == '\n') {
      stringLengthRight[i] = '\0';
      break;
    }
  }
  // Afficher la longueur des murs
  disp.clear();
  disp.write("GAUCHE: ");
  disp << (stringLengthLeft);
  disp << " cm";
  disp.write("DROITE: ", 16);
  disp << (stringLengthRight);
  disp << " cm";
}

// La fonction affichage de Simon rend le smoothing un peu plus subtile...
// Si on a trois mesures : 100 150 101, on va vouloir smooth la valeur 150
// Par contre, 100 != 101, donc on ne peut pas simplement faire
// if(distances[indexLine - 1] == distances[index2]){ smooth la mesure
// du milieu ... } Dans la fonction affichage de Simon les distance vont etre
// divise par quatre, donc tant que la mesure avant et la mesure apres sont
// dans le range de +/- 4, (meme si elles ne sont pas identique) on peut quand
// meme smooth la mesure du milieu
void smoothingTunnel(uint16_t nElements, uint16_t *distance) {
  // Smoothing le mur gauche
  // On skip la premiere et la derniere donnee
  // Par contre, pour smooth les pics, on est en train de chercher des valeurs
  // qui sont en dehors de notre tableau (pointeur) Ceci n'est pas suppose de
  // causer une erreur car on modifie seuelement les valeurs qui sont dans
  // notre tableau (pointeur)
    
  for (uint8_t i = 2; i <= 93; ++i) {

    uint16_t index2 = (nElements - 1) * (i-1) * 10 / 95;  /// i.e. 199 * 95 / 95
    uint16_t index3 = (nElements - 1) * i * 10 / 95;  /// i.e. 199 * 95 / 95
    uint16_t index4 = (nElements - 1) * (i+1) * 10 / 95;  /// i.e. 199 * 95 / 95

    if (index2 % 10 >= 5) index2 += 10;
    index2 /= 10;

    if (index3 % 10 >= 5) index3 += 10;
    index3 /= 10;

    if (index4 % 10 >= 5) index4 += 10;
    index4 /= 10;

    // On a 5 valeurs qui sont a l'index -2 -1 0 1 2
    // On verifie d'abord si -1 et 1 sont a peu pres la meme chose
    if (distance[index2] <= distance[index4] + 2 &&
        distance[index2] >= distance[index4] - 2) {
      // // Pic vers la gauchek
      // // On verifie si -2 et 2 sont a peu pres la meme chose
      // if (distance[index1] <= distance[index5] + 2 &&
      //     distance[index1] >= distance[index5] - 2 &&
      //     // On verifie si -2 est suffissament plus grand que -1
      //     distance[index1] > distance[index2] + 4) {
      //   // Si cest 2 1 1 1 2, on ajoute 1 du milieu devient 0
      //   // Si cest 4 2 2 2 4, on ajoute 2 du milieu devient 0
      //   // Si cest 6 3 3 3 6, on ajoute 3 du milieu devient 0
      //   // On lui enleve la difference entre 2 valeurs consecutives
      //   distance[index3] -=
      //       distance[index1] - distance[index2];
      //   // Pic vers la droite
      //   // On verifie si -2 et 2 sont a peu pres la meme chose
      // } else if (distance[index1] <= distance[index5] + 2 &&
      //            distance[index1] >= distance[index5] - 2 &&
      //            // On verifie si -2 est suffissament plus petit que -1
      //            distance[index1] < distance[index2] - 4) {
      //   // Si cest 0 1 1 1 0, on ajoute 1 du milieu devient 2
      //   // Si cest 0 2 2 2 0, on ajoute 2 du milieu devient 4
      //   // Si cest 0 3 3 3 0, on ajoute 3 du milieu devient 6
      //   // On lui ajoute la difference entre 2 valeurs consecutives
      //   distance[index3] +=
      //       distance[index2] - 4 - distance[index1];
      // } else {
        // Smooth mur plat
        distance[index3] = distance[index2];
      // }
    }
  }
}
