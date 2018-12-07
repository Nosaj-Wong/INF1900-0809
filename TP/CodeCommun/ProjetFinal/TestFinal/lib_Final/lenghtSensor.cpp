#include "lenghtSensor.h"

//read and calculate distance
void read(can &can, uint16_t* tableauGauche, uint16_t* tableauDroite, uint16_t &nbValues,int8_t leftAdjust, int8_t rightAdjust){
    uint16_t resultatDroite = 0;
    uint16_t resultatGauche = 0;
    for(int i = 0; i < VALUES_PER_READ; i++){
        
        resultatDroite += can.lecture(1); // capteur droit
        _delay_ms(TIME_PER_READ);
        resultatGauche += can.lecture(1); // capteur gauche
        _delay_ms(TIME_PER_READ);
    }
    //on calcule la moyenne
    resultatDroite = resultatDroite / VALUES_PER_READ;
    resultatGauche = resultatGauche / VALUES_PER_READ;

    //calcul pasant du voltage sur 10 bit a la longeur en (pouces/16)
    resultatDroite = RIGHT_NUMERATOR/(resultatDroite - RIGHT_DENOMINATOR_CONSTANT) - RIGHT_CORRECTION_CONSTANT;
    resultatGauche = LEFT_NUMERATOR/(resultatGauche - LEFT_DENOMINATOR_CONSTANT) - LEFT_CORRECTION_CONSTANT;

    //conversion de (pouces/16) a (pouces/LENGHT_PRECISION)
    resultatDroite = (resultatDroite * LENGHT_PRECISION ) / 16;
    resultatGauche = (resultatGauche * LENGHT_PRECISION ) / 16;

    //On ajoute les valeurs aux tableaux correspondants
    tableauDroite[nbValues] = resultatDroite+rightAdjust;
    tableauGauche[nbValues] = resultatGauche+leftAdjust;
    //on incremente le nombre de valeurs lues
    nbValues++;
}

//calculate total wall lenght
uint32_t calculateLenght(uint16_t* tableauDroite, uint16_t &nbValues){
    //dx en pouces/100, correspond a la distance entre chaque lecture
    uint16_t dx = 4800 / nbValues;

    //variables utilisees pour les calculs
    uint32_t difference = 0;
    uint32_t lenght = 0;
    uint16_t hyp = 0;

/****************************** CALCUL MUR DROIT ******************************/

    for(uint16_t i = 1; i < nbValues; i++){
        //on calcule la difference de distance entre 2 points consecutifs
        //en valeur absolue
        if(tableauDroite[i] > tableauDroite[i-1]){
            difference = tableauDroite[i] - tableauDroite[i-1];
        }
        else{
            difference = tableauDroite[i-1] - tableauDroite[i];
        }

        //on exprime difference en pouces/10 000
        difference = difference * 1000;

        //on exprime la difference comme multiple de (dx * 100)
        difference = difference / dx;

        if(difference > 250){       //ratio de plus de 2.50
            
            //approximation hyp equivaut a: (ratio + 0,1) * dx = hyp
            //donne hyp en pouces/100
            hyp = (difference + 10) *dx / 100;
            lenght += hyp;
        }
        else if(difference < 54) //ratio de moins de 0.54
        {
            //approx de hyp equivaut a: ((ratio/10) + 1) * dx = diff/10 + dx
            //resultat en pouces/100
            hyp = (difference + 1000) *dx / 1000;
            lenght += hyp;
        }
        else {
            //on transforme le ratio en index d'un tableau contenant la proportion
            // des hypotenuses de triangles ayant un ratio entre 0.55 et 2.45 au 0.05 pres
            difference = (difference - 50) / 5;
            hyp= (tableauHyp[difference] +100) * dx / 100;
            //on ajoute 100 a la valeur retournee par le tableau car les valeurs
            //du tableau ont ete diminues pour entrer sur 8 bits
            lenght += hyp;
        }
    }
    return lenght;

}
