
/*
** file: lenghtSensor.cpp
** Implementation de la classe lenghtSensor qui gere le calcule et la prise de
** distances
**
** date: 16 novembre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
*/

#include "lenghtSensor.h"

// Lit, fait une moyenne des distances et met cette moyenne dans le tableau
void read(can &can, uint16_t *tableauGauche, uint16_t *tableauDroite,
          uint16_t &nbValues, int8_t leftAdjust, int8_t rightAdjust) {
  // On initialise les variables contenant la somme
  uint32_t resultatDroite = 0;
  uint32_t resultatGauche = 0;

  // On fait plusieurs lectures qu'on ajoute a la somme
  for (int i = 0; i < VALUES_PER_READ; i++) {
    resultatDroite += can.lecture(1);  // capteur droit
    _delay_ms(TIME_PER_READ);
    resultatGauche += can.lecture(0);  // capteur gauche
    _delay_ms(TIME_PER_READ);
  }

  // on calcule la moyenne
  resultatDroite = resultatDroite / VALUES_PER_READ;
  resultatGauche = resultatGauche / VALUES_PER_READ;

  // calcul pasant du voltage sur 10 bit a la longeur en (pouces*10) selon des
  // formules obtenu sur Excel
  if (resultatDroite >= 185)
    resultatDroite =
        RIGHT_NUMERATOR1 / (resultatDroite - RIGHT_DENOMINATOR_CONSTANT1) -
        RIGHT_CORRECTION_CONSTANT1;

  else
    resultatDroite =
        RIGHT_NUMERATOR2 / (resultatDroite - RIGHT_DENOMINATOR_CONSTANT2) +
        RIGHT_CORRECTION_CONSTANT2;

  if (resultatGauche >= 213)
    resultatGauche =
        LEFT_NUMERATOR1 / (resultatGauche - LEFT_DENOMINATOR_CONSTANT1) -
        LEFT_CORRECTION_CONSTANT1;
  else
    resultatGauche =
        LEFT_NUMERATOR2 / (resultatGauche + LEFT_DENOMINATOR_CONSTANT2) +
        LEFT_CORRECTION_CONSTANT2;

  // Ajout des corrections
  resultatDroite += rightAdjust;
  resultatGauche += leftAdjust;

  // Mise dans les bornes acceptable (soit de 0.25 pouces a 24 pouces)
  if (resultatDroite > 240)
    resultatDroite = 240;
  else if (resultatDroite < 25)
    resultatDroite = 25;

  if (resultatGauche > 240)
    resultatGauche = 240;
  else if (resultatGauche < 25)
    resultatGauche = 25;

  // On ajoute les valeurs aux tableaux correspondants
  tableauDroite[nbValues] = resultatDroite;
  tableauGauche[nbValues] = resultatGauche;
  // on incremente le nombre de valeurs lues
  nbValues++;
}

// Calcul de la distance totale avec pythagore
uint32_t calculateLenghtSqrt(uint16_t *tableauDistance, uint16_t nbValues) {
  // On prendra 96 mesures sur 48 pouces, donc .5 pouce par mesure
  const uint8_t dx = 5;

  // On initialise la longueur a 0
  float lenght = 0;

  //On prepare les indices de notre tableau que nous allons utiliser 
  uint16_t indexPrec;
  uint16_t indexSuiv;

  // On preparer les float qui nous servirons a calculer les cote et
  // lhypothenuse des triangles
  float difference;
  float hypo;
  float pytha;

  for (uint8_t i = 0; i <= 95; i++) {
    // On divise en 97 points, donc 96 differences en arrondissant a l<entier le
    // plus pres
    indexPrec = round((nbValues - 1) * i / 96.0);
    indexSuiv = round((nbValues - 1) * (i + 1) / 96.0);

    // On fait la difference en valeur absolue entre ces deux valeurs
    difference = fabs(float(tableauDistance[indexPrec]) -
                      float(tableauDistance[indexSuiv]));

    // Si la difference est tres grande, on neglige dx
    if (difference >= 200) {
      hypo = difference;
    }

    // Si la difference est tres petite, on neglige celle-ci (incertiture
    // probable)
    else if (difference < 3) {
      hypo = dx;
    }

    // Si elle est quelque part entre les 2, on fait pythafore
    else {
      pytha = difference * difference + dx * dx;

      hypo = sqrt(pytha);
    }

    // On ajoute cette distance a notre somme
    lenght += hypo;
  }

  // On retourne la valeur converti en cm le plus pres
  return round(lenght * 2.54 / 10);
}