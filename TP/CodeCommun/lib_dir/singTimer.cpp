/*
** file: sing.h
** Implementation de la classe singTimer
** Cette classe permet de produire des notes ou des melodies sur le haut-parleur du robot à l'aide du timer (PWM materiel)
** 
** date: 30 octobre 2018
** 
** authors: 
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
** 
*/

#include "singTimer.h"

// Constructeur modifiant la duree et les repetitions de la chanson
SingTimer::SingTimer() {
    //TABLEAU AVEC LES NOTES POSSIBLES:
    notes_[0] = 36364;   //note #0 , correspond a note #45 dans tableau tp9
    notes_[1] = 34323;   //note #1 , correspond a note #46 dans tableau tp9
    notes_[2] = 32396;   //note #2 , correspond a note #47 dans tableau tp9
    notes_[3] = 30578;   //note #3 , correspond a note #48 dans tableau tp9
    notes_[4] = 28862;   //note #4 , correspond a note #49 dans tableau tp9
    notes_[5] = 27242;   //note #5 , correspond a note #50 dans tableau tp9
    notes_[6] = 25713;   //note #6 , correspond a note #51 dans tableau tp9
    notes_[7] = 24270;   //note #7 , correspond a note #52 dans tableau tp9
    notes_[8] = 22908;   //note #8 , correspond a note #53 dans tableau tp9
    notes_[9] = 21622;   //note #9 , correspond a note #54 dans tableau tp9
    notes_[10] = 20408;  //note #10, correspond a note #55 dans tableau tp9
    notes_[11] = 19263;  //note #11, correspond a note #56 dans tableau tp9
    notes_[12] = 18182;  //note #12, correspond a note #57 dans tableau tp9
    notes_[13] = 17161;  //note #13, correspond a note #58 dans tableau tp9
    notes_[14] = 16198;  //note #14, correspond a note #59 dans tableau tp9
    notes_[15] = 15289;  //note #15, correspond a note #60 dans tableau tp9
    notes_[16] = 14431;  //note #16, correspond a note #61 dans tableau tp9
    notes_[17] = 13621;  //note #17, correspond a note #62 dans tableau tp9
    notes_[18] = 12856;  //note #18, correspond a note #63 dans tableau tp9
    notes_[19] = 12135;  //note #19, correspond a note #64 dans tableau tp9
    notes_[20] = 11454;  //note #20, correspond a note #65 dans tableau tp9
    notes_[21] = 10811;  //note #21, correspond a note #66 dans tableau tp9
    notes_[22] = 10204;  //note #22, correspond a note #67 dans tableau tp9
    notes_[23] = 9631;   //note #23, correspond a note #68 dans tableau tp9
    notes_[24] = 9091;   //note #24, correspond a note #69 dans tableau tp9
    notes_[25] = 8581;   //note #25, correspond a note #70 dans tableau tp9
    notes_[26] = 8099;   //note #26, correspond a note #71 dans tableau tp9
    notes_[27] = 7645;   //note #27, correspond a note #72 dans tableau tp9
    notes_[28] = 7215;   //note #28, correspond a note #73 dans tableau tp9
    notes_[29] = 6810;   //note #29, correspond a note #74 dans tableau tp9
    notes_[30] = 6428;   //note #30, correspond a note #75 dans tableau tp9
    notes_[31] = 6067;   //note #31, correspond a note #76 dans tableau tp9
    notes_[32] = 5727;   //note #32, correspond a note #77 dans tableau tp9
    notes_[33] = 5405;   //note #33, correspond a note #78 dans tableau tp9
    notes_[34] = 5102;   //note #34, correspond a note #79 dans tableau tp9
    notes_[35] = 4816;   //note #35, correspond a note #80 dans tableau tp9
    notes_[36] = 4545;   //note #36, correspond a note #81 dans tableau tp9
    //FIN TABLEAU
}

// Chante une note en modifiant les attribut note et duration
void SingTimer::note(uint8_t note) {
    PWM_.setPeriod(notes_[note - 45]);
    PWM_.start();
}

void SingTimer::stop() {
    PWM_.stop();
}

// void SingTimer::noteDuree(uint8_t note) {
//     note(note);
// }
