
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

uint32_t calculateLenght(uint16_t *tableau, uint16_t nbValues);

void smoothingTunnel(uint16_t nElements, uint16_t *leftDistances, uint16_t *rightDistances);

void printTunnel(uint16_t nElements, uint16_t *leftDistances,
                 uint16_t *rightDistances);

int main() {
    // ifstream entree("tunnel_2_70.txt");
    ifstream entree("printCapteurBrise.txt");
    // ifstream entree("tunnel_1_120.txt");
    // ifstream entree("print.txt");
    entree.exceptions(ios::failbit);

    vector<uint16_t> tableauG;
    vector<uint16_t> tableauD;
    string string_trash;
    char char_trash;

    while (!ws(entree).eof()) {
        uint16_t countG = 0;
        uint16_t countD = 0;

        getline(entree, string_trash, '_');
        entree >> char_trash;
        while (char_trash == '_') {
            countG++;
            entree >> char_trash;
        };

        getline(entree, string_trash, '_');
        entree >> char_trash;
        while (char_trash == '_') {
            countD++;
            entree >> char_trash;
        };

        tableauG.push_back(countG * 2.5 + 2.5);
        tableauD.push_back(countD * 2.5 + 2.5);

        getline(entree, string_trash);
    }

    // for (int i = 0; i < 96; i++) {
    //     cout << tableauG[i] << endl;
    // }

    uint16_t *ptr_tableauG = &tableauG[0];
    uint16_t *ptr_tableauD = &tableauD[0];

    uint16_t nbValues = 96;
    uint16_t lengthG = calculateLenght(ptr_tableauD, nbValues);
    uint16_t lengthD = calculateLenght(ptr_tableauG, nbValues);

    lengthG = lengthG * 254 / 100;
    if (lengthG % 10 >= 5) {
        lengthG = (lengthG + 10) / 10;
    } else {
        lengthG /= 10;
    }

    lengthD = lengthD * 254 / 100;
    if (lengthD % 10 >= 5) {
        lengthD = (lengthD + 10) / 10;
    } else {
        lengthD /= 10;
    }

    cout << "Gauche: " << lengthG << " cm" << endl;
    cout << "Droite: " << lengthD << " cm" << endl;

    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    printTunnel(96, ptr_tableauG, ptr_tableauD);
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    smoothingTunnel(96, ptr_tableauG, ptr_tableauD);
    printTunnel(96, ptr_tableauG, ptr_tableauD);
}

uint32_t calculateLenght(uint16_t *tableau, uint16_t nbValues) {
    const uint8_t dx = 5;
    float lenght = 0;
    uint16_t indexPrec;
    uint16_t indexSuiv;
    uint16_t difference;
    float hypo;
    uint32_t pytha;

    for (uint8_t i = 0; i <= 95; i++) {
        indexPrec = (nbValues - 1) * i / 96;
        indexSuiv = (nbValues - 1) * (i + 1) / 96;

        if (tableau[indexPrec] > tableau[indexSuiv]) {
            difference = tableau[indexPrec] - tableau[indexSuiv];
        } else {
            difference = tableau[indexSuiv] - tableau[indexPrec];
        }

        if (difference >= 100) {
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
    return lenght;
}

// La fonction affichage de Simon rend le smoothing un peu plus subtile...
// Si on a trois mesures : 100 150 101, on va vouloir smooth la valeur 150
// Par contre, 100 != 101, donc on ne peut pas simplement faire
// if(leftDistances[indexLine - 1] == leftDistances[indexLine + 1]){ smooth la mesure du milieu ... }
// Dans la fonction affichage de Simon les distance vont etre divise par quatre,
// donc tant que la mesure avant et la mesure apres sont dans le range de +/- 4,
// (meme si elles ne sont pas identique) on peut quand meme smooth la mesure du milieu
void smoothingTunnel(uint16_t nElements, uint16_t *leftDistances, uint16_t *rightDistances) {
    // Smoothing le mur gauche
    // On skip la premiere et la derniere donnee
    // Par contre, pour smooth les pics, on est en train de chercher des valeurs qui sont en dehors de
    // notre tableau (pointeur)
    // Ceci n'est pas suppose de causer une erreur car on modifie seuelement les valeurs qui sont dans
    // notre tableau (pointeur)
    for (uint8_t indexLine = 1; indexLine <= 94; ++indexLine) {
        // On a 5 valeurs qui sont a l'index -2 -1 0 1 2
        // On verifie d'abord si -1 et 1 sont a peu pres la meme chose
        if (leftDistances[indexLine - 1] <= leftDistances[indexLine + 1] + 2 &&
            leftDistances[indexLine - 1] >= leftDistances[indexLine + 1] - 2) {
            // Pic vers la gauche
            // On verifie si -2 et 2 sont a peu pres la meme chose
            if (leftDistances[indexLine - 2] <= leftDistances[indexLine + 2] + 2 &&
                leftDistances[indexLine - 2] >= leftDistances[indexLine + 2] - 2 &&
                // On verifie si -2 est suffissament plus grand que -1
                leftDistances[indexLine - 2] > leftDistances[indexLine - 1] + 4) {
                // Si cest 2 1 1 1 2, on ajoute 1 du milieu devient 0
                // Si cest 4 2 2 2 4, on ajoute 2 du milieu devient 0
                // Si cest 6 3 3 3 6, on ajoute 3 du milieu devient 0
                // On lui enleve la difference entre 2 valeurs consecutives
                leftDistances[indexLine] -= leftDistances[indexLine - 2] - leftDistances[indexLine - 1];
                // Pic vers la droite
                // On verifie si -2 et 2 sont a peu pres la meme chose
            } else if (leftDistances[indexLine - 2] <= leftDistances[indexLine + 2] + 2 &&
                       leftDistances[indexLine - 2] >= leftDistances[indexLine + 2] - 2 &&
                       // On verifie si -2 est suffissament plus petit que -1
                       leftDistances[indexLine - 2] < leftDistances[indexLine - 1] - 4) {
                // Si cest 0 1 1 1 0, on ajoute 1 du milieu devient 2
                // Si cest 0 2 2 2 0, on ajoute 2 du milieu devient 4
                // Si cest 0 3 3 3 0, on ajoute 3 du milieu devient 6
                // On lui ajoute la difference entre 2 valeurs consecutives
                leftDistances[indexLine] += leftDistances[indexLine - 1] - 4 - leftDistances[indexLine - 2];
            } else {
                // Smooth mur plat
                leftDistances[indexLine] = leftDistances[indexLine - 1];
            }
        }
    }
    // Smoothing le mur droite
    // Voir mur gauche
    for (uint8_t indexLine = 0; indexLine <= 95; ++indexLine) {
        if (rightDistances[indexLine - 1] <= rightDistances[indexLine + 1] + 2 &&
            rightDistances[indexLine - 1] >= rightDistances[indexLine + 1] - 2) {
            if (rightDistances[indexLine - 2] <= rightDistances[indexLine + 2] + 4 &&
                rightDistances[indexLine - 2] >= rightDistances[indexLine + 2] - 4 &&
                rightDistances[indexLine - 2] > rightDistances[indexLine - 1] + 4) {
                rightDistances[indexLine] -= rightDistances[indexLine - 2] - rightDistances[indexLine - 1];
            } else if (rightDistances[indexLine - 2] <= rightDistances[indexLine + 2] + 4 &&
                       rightDistances[indexLine - 2] >= rightDistances[indexLine + 2] - 4 &&
                       rightDistances[indexLine - 2] < rightDistances[indexLine - 1] - 4) {
                rightDistances[indexLine] += rightDistances[indexLine - 1] - 4 - rightDistances[indexLine - 2];
            } else {
                rightDistances[indexLine] = rightDistances[indexLine - 1];
            }
        }
    }
}

void printTunnel(uint16_t nElements, uint16_t *leftDistances,
                 uint16_t *rightDistances) {
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
            (nElements - 1) * i / TOTAL_LINES;  /// i.e. 199 * 95 / 95

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

        // Distance entre le bord de la table et le mur est le complement de la
        // distance entre la table et le mur
        uint16_t leftBorderToWall = N_HALF_COLUMNS - leftWalltoCenter;
        uint16_t rightBorderToWall = N_HALF_COLUMNS - rightWalltoCenter;

        // Afficher la partie gauche: extrémité gauche -> mur
        for (uint8_t nCharacters = 0; nCharacters < leftBorderToWall;
             nCharacters++) {
            cout << ('@');
        }

        // Afficher la partie gauche: mur -> centre
        for (uint8_t nCharacters = 0; nCharacters < leftWalltoCenter; nCharacters++)
            cout << (' ');

        // Afficher la ligne jaune
        cout << ('|');
        cout << ('|');

        // Afficher la partie droite: centre -> mur
        for (uint8_t nCharacters = 0; nCharacters < rightWalltoCenter;
             nCharacters++)
            cout << (' ');
        // Afficher la partie droite: mur -> extrémité droite
        for (uint8_t nCharacters = 0; nCharacters < rightBorderToWall;
             nCharacters++)
            cout << ('@');

        // Saut de ligne
        cout << ('\n');
    }
}
