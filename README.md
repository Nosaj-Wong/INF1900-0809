# Project Title
Projet initial de système embarqué du cours INF1900 

## Authors:
Simon Berhanu,
Facundo Giannotti,
Vincent L'Ecuyer-Simard,
Xi Chen Shen

## Getting Started
Nous avons separer, comme vous pouvez le voir, notre dossier projet en 2 sections, Exec et lib_Final.
Comme leur nom le suppose, Exec contient notre executable et lib_Final notre librairie.

Vision globale:
  - Notre robot fait le suivi de ligne dans le main et une prise de donnee par interruption
  - Cette interruption prend plus de 80 donnees de chaque cote, en fait une moyenne
  - Nous stockons les moyennes obtenues dans un tableau
  - Nous utilisons comme unites les pouces *10, (par exemple 24 pouces sera stocke comme 240)
  - A la toute fin nous calculons la distance totale grace a un echantillon de 96 donness (comme l'affichage)

Parlons tout d'abord d'Exec:
  - Il est divise en deux sections, soit le main et l'interruption

  - Le main est divise en etats qui represente les differentes parties du trajet
  - Un etat peut etre reutiliser pour plusieurs sections du trajet similaire (exemple virage 1 et 3)
  - Dans chacune de ces parties, il fait appel a des fonctions de la librarie pour effectuer ses taches
  - A sa compilation vous verrez le warning: deprecated conversion from string constant to â€˜char*â€™ [-Wwrite-strings]
   char *robotName = "  ATmegaReussi";
    Nous pensons que ce warning est benin et qu'il s'agit d'une facon adequate de stocker un string dans une variable 
    dans le contexte ou le type string n'est pas disponible pour le robot.
  - Nous avons laisser les DE>BUG_PRINT en place pour vous montrer l'utilisation que nous en avons fait

  - L'interruption gere la prise de donnes de distance, a une frequence etablie, la miuterie 1 genere cele-ci
  si nous sommes dans une section propice a la prise de donne (entre M1 et M2 et hors d'un virage)
  - Ensuite le robot retourne a sa tache de suivi de ligne

Parlons maintenant de la librarie
  - Elle est presque entierement formee de classes

Pour le suivi de ligne:
  - FinalDriver fourni les principales methodes de suivi utilise par le main
  - FinalDriver se trouve a un niveau d'abstraction eleve, permettant des methode telle que 
      driver.forward(pourcentage de puissance) ou encore driver.forwardStraight().
  - Notre principale methode de suivi de ligne, forwardStraight, augmente la force de correction
      du moteur voulu proportionellement a l'importance de la deviation
  - FinalDriver utilise princpalement les methodes de FinalPWM, qui est un niveau d'abstraction moindre
      avec des methode comme PWM.setPercent(95).

Pour la prise de mesures:
  - lenghtSensor fourni 2 methodes essentielle, soit read, qui prend en parametre les 2 tableaux de distances,
      et fait une moyenne des distances a gauche et a droite et on ajoute ces moyennes aux tableaux
  - CalculateLenghtSqrt qui, grace a pythagore, additionne les diffrerence entre chaque mesures

Pour le controle de la LED:
  - FinalLed s'occupe de gerer la LED
  - Il effectue bien sur l'ouverture et la fermeture de la DEL, mais egalement le clignotement vert
    et l'apparition de l'ambre.
  - Il gere ces deux derniers de facon cmpletement materiel, avec 1 timer CTC pour le clignotement vert
    et 2 timer PWM pour l'ambre pour permettre l'alternance entre vert et rouge avec une proportion de 80-20

Pour la melodie:
  - singMario5SEcondes se charge de transmettre la suite frequence adequate pour la duree appropriee
    au piezo electrique pour faire jouer notre melodie

Pour la gestion de la minuterie d'interruption:
  - FinalTimer est en mesure de debuter une minuterie a une duree voulu (en centieme de sec) et de
    declencher une interruption apres ce delais. Nous l'utilisons pour synchroniser notre prise de mesures

Plusieurs fonctions orpheline se retrouve dans FInalFonctions, par exemple:
  -_my_delay_10us, permettant de prendre une variable en parametre, contrairement a _delay_ms
  - PWM software, qui fait un PWM de facon logicielle, il est utilise pour la melodie
  - intToString, qui prend en parametre un int et un tableau de char et ramplie ce char avec 
    la version en string du int a l'aide de decomposition nombre par nombre
  - printTunnel, qui affiche le tunnel en passant a travers le tableau de valeur de distance et affiche
    les @ et les espaces selon les distances mesurees
  - displayFinalResult, qui affiche les distances totales sur l'ecran LCD
  - Smoothing tunnel qui rend droit les parois des tunels qui comportent de petites deviations


Merci beaucoup, et nous esperons avoir pu rendre votre correction plus facile!

Equipe 08-09
AtmegaReussi
5 decembre 2018
