/*
 * Nom: Probleme 1
 * Description: Ceci est un programme qui écrit la chaîne de caractères
 * « *P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*» suivi d'un 0x00 en mémoire
 * externe. Version: 1.1
 */

#define F_CPU 8000000
#include <avr/interrupt.h>
#include <avr/io.h>
#include <memoire_24.h>
#include <util/delay.h>

int main() {

  DDRA = 0xff; // PORT A est en mode sortie
  DDRC = 0x00; // PORT C est en mode entree
  DDRD = 0x00; // PORT D est en mode entree

  uint8_t NBCARATERES = 45;
  unsigned char polyEcrit[NBCARATERES] = {
      '*', 'P', '*', 'O', '*', 'L', '*', 'Y', '*', 'T', '*', 'E',
      '*', 'C', '*', 'H', '*', 'N', '*', 'I', '*', 'Q', '*', 'U',
      '*', 'E', '*', '*', 'M', '*', 'O', '*', 'N', '*', 'T', '*',
      'R', '*', 'E', '*', 'A', '*', 'L', '*', 0x00};
  unsigned char polyLec[NBCARATERES] = {};
  bool memeCaractere = true;
  Memoire24CXXX memoire;

  for (;;) {
    if (PIND & 0x04) {
      _delay_ms(30); // antirebond
      if (PIND & 0x04) {
        // écrit la chaîne de caractères en mémoire externe
        uint8_t memoireEcrite = memoire.ecriture(0x00, polyEcrit, NBCARATERES);

        // prévoir un délai de 5 ms après l'écriture d'un octet en mémoire
        // pour que la mémoire peut compléter son cycle d'écriture sans problème
        _delay_ms(5);

        // lire la chaine de caracteres en memoire externe
        uint8_t memoireLu = memoire.lecture(0x00, polyLec, NBCARATERES);

        // Comparaison de la chaîne de caractères envoyée à la mémoire et celle
        // relue.
          polyLec[6] = 'z';
        for (int i = 0; i < NBCARATERES; i++) {
          if (polyEcrit[i] != polyLec[i]){
            memeCaractere = false;
          }
            
        }
        
        // Afficher la DEL en vert si elles sont identiques et en rouge dans le
        // cas contraire
        if (memeCaractere)
          PORTA = 0x02; // LED VERT
        else
          PORTA = 0x01; // LED ROUGE

        // Prévoir un délai de 5 ms apres l'accès suivant à la mémoire
        _delay_ms(5);
      }
    }
  }

  return 0;
}
