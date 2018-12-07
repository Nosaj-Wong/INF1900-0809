#include "lenghtSensor.h"

// read and calculate distance
void read(can &can, uint16_t *tableauGauche, uint16_t *tableauDroite,
          uint16_t &nbValues, int8_t leftAdjust, int8_t rightAdjust) {
  uint16_t resultatDroite = 0;
  uint16_t resultatGauche = 0;
  for (int i = 0; i < VALUES_PER_READ; i++) {
    resultatDroite += can.lecture(1);  // capteur droit
    _delay_ms(TIME_PER_READ);
    resultatGauche += can.lecture(0);  // capteur gauche
    _delay_ms(TIME_PER_READ);
  }
  // on calcule la moyenne
  resultatDroite = resultatDroite / VALUES_PER_READ;
  resultatGauche = resultatGauche / VALUES_PER_READ;

  // calcul pasant du voltage sur 10 bit a la longeur en (pouces*10)

if (resultatDroite>=185)
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

  // Mise dans les bornes acceptable
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

// calculate total wall lenght
uint32_t calculateLenght(uint16_t *tableauDroite, uint16_t &nbValues) {
  // dx en pouces/100, correspond a la distance entre chaque lecture
  uint16_t dx = 4800 / nbValues;

  // variables utilisees pour les calculs
  uint32_t difference = 0;
  uint32_t lenght = 0;
  uint16_t hyp = 0;

  /****************************** CALCUL MUR DROIT
   * ******************************/

  for (uint16_t i = 1; i < nbValues; i++) {
    // on calcule la difference de distance entre 2 points consecutifs
    // en valeur absolue
    if (tableauDroite[i] > tableauDroite[i - 1]) {
      difference = tableauDroite[i] - tableauDroite[i - 1];
    } else {
      difference = tableauDroite[i - 1] - tableauDroite[i];
    }

    // on exprime difference en pouces/10 000
    difference = difference * 1000;

    // on exprime la difference comme multiple de (dx * 100)
    difference = difference / dx;

    if (difference > 250) {  // ratio de plus de 2.50

      // approximation hyp equivaut a: (ratio + 0,1) * dx = hyp
      // donne hyp en pouces/100
      hyp = (difference + 10) * dx / 100;
      lenght += hyp;
    } else if (difference < 54)  // ratio de moins de 0.54
    {
      // approx de hyp equivaut a: ((ratio/10) + 1) * dx = diff/10 + dx
      // resultat en pouces/100
      hyp = (difference + 1000) * dx / 1000;
      lenght += hyp;
    } else {
      // on transforme le ratio en index d'un tableau contenant la proportion
      // des hypotenuses de triangles ayant un ratio entre 0.55 et 2.45 au 0.05
      // pres
      difference = (difference - 50) / 5;
      hyp = (tableauHyp[difference] + 100) * dx / 100;
      // on ajoute 100 a la valeur retournee par le tableau car les valeurs
      // du tableau ont ete diminues pour entrer sur 8 bits
      lenght += hyp;
    }
  }
  return lenght;
}

uint32_t calculateLenghtTest1(uint16_t *tableauDroite, uint16_t nbValues) {
  const uint8_t dx = 5;
  uint32_t lenght = 0;
  uint16_t indexPrec;
  uint16_t indexSuiv;
  uint16_t difference;
  uint16_t hypo;
  uint32_t pytha;

  float floatNbr;
  int32_t intNbr;

  for (uint8_t i = 0; i <= 95; i++) {
    indexPrec = (nbValues - 1) * i / 96;
    indexSuiv = (nbValues - 1) * (i + 1) / 96;

    if (tableauDroite[indexPrec] > tableauDroite[indexSuiv]) {
      difference = tableauDroite[indexPrec] - tableauDroite[indexSuiv];
    } else {
      difference = tableauDroite[indexSuiv] - tableauDroite[indexPrec];
    }

    if (difference >= 25) {
      hypo = difference;
    }

    else if (difference < 3) {
      hypo = dx;
    }

    else {
      pytha = difference * difference + dx * dx;

      floatNbr = pytha;
      intNbr = *(int *)&floatNbr;
      intNbr = 0x1fbc551d + (intNbr >> 1);
      floatNbr = *(float *)&intNbr;
      if (int(floatNbr * 2) > (int(floatNbr) * 2))
        hypo = floatNbr + 1;
      else
        hypo = floatNbr;
    }
    lenght += hypo;
  }
  return lenght;
}

uint32_t calculateLenghtTest2(uint16_t *tableauDroite, uint16_t nbValues) {
  const uint8_t dx = 5;
  uint32_t lenght = 0;
  uint16_t indexPrec;
  uint16_t indexSuiv;
  uint16_t difference;
  uint16_t hypo;
  uint32_t pytha;

  for (uint8_t i = 0; i <= 95; i++) {
    indexPrec = (nbValues - 1) * i / 96;
    indexSuiv = (nbValues - 1) * (i + 1) / 96;

    if (tableauDroite[indexPrec] > tableauDroite[indexSuiv]) {
      difference = tableauDroite[indexPrec] - tableauDroite[indexSuiv];
    } else {
      difference = tableauDroite[indexSuiv] - tableauDroite[indexPrec];
    }

    if (difference >= 25) {
      hypo = difference;
    }

    else if (difference < 3) {
      hypo = dx;
    }

    else {
      pytha = difference * difference + dx * dx;

      uint16_t racine = 0;
      while ((racine * racine) < pytha) {
        racine++;
      }
      if (racine * racine - pytha < pytha - (racine - 1) * (racine - 1))
        racine--;

      hypo = racine;
    }
    lenght += hypo;
  }
  return lenght;
}

uint32_t calculateLenghtTest3(uint16_t *tableauDistance, uint16_t nbValues) {
  const uint8_t dx = 5;
  float lenght = 0;
  uint16_t indexPrec;
  uint16_t indexSuiv;
  float difference;
  float hypo;
  float pytha;

  for (uint8_t i = 0; i <= 95; i++) {
    indexPrec = round((nbValues - 1) * i / 96.0);
    indexSuiv = round((nbValues - 1) * (i + 1) / 96.0);

    difference= fabs(float(tableauDistance[indexPrec])-float(tableauDistance[indexSuiv]));

    if (difference >= 200) {
      hypo = difference;
    }

    else if (difference < 3) {
      hypo = dx;
    }

    else {
      pytha = difference * difference + dx * dx;

      hypo = sqrt(pytha);
    }
    lenght += hypo;
  }
  return round(lenght*2.54/10);
}