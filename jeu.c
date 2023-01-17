#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"

/* ------------------------------------------------ */
// Fichier source qui regroupe toutes les fonctions 
// relatives au jeu
/* ------------------------------------------------ */


/* 
  param1 : numéro du processus fils
  retour : retourne le numéro de joueur associé a ce processus fils
*/

int numFilsToJoueur(int numFils) {
    return numFils + 1;
}

/* 
  param1 : numéro du joueur
  retour : retourne le numéro du processus fils associé a ce joueur
*/

int numJoueurToFils(int numJoueur) {
    return numJoueur - 1;
}


/* 
  param1 : numéro du processus qui a été désigné par le pere pour joueur
  param2: nombre de joueur
  retour : retourne le numéro du prochain processus fils qui va recuperer les informations dans l'anneau
*/

int getNumeroDuProchain(int numQuiVientDeJouer, int nombreJoueurs) { 
    return (numQuiVientDeJouer == nombreJoueurs - 1 ? (0) : (numQuiVientDeJouer + 1));
}


/* 
  param1 : plateau, matrice de caractère 15*15
  cette fonction initiale le plateau, (les écuries, les cases a parcourir, les escaliers, etc ...)
  !!! Elle ne place aucun pions !!!!
*/


void initPlateau(char plateau[15][15]) { //
    int i, j;

    for(i=0;i<15;i++){
		for(j=0;j<15;j++){
			plateau[i][j]='*';
			if((i<6)&&(j<6)){
                plateau[i][j]='1';}// écurie 1 (en haut a gauche)
			else if((i<6)&&(j>8)){
                plateau[i][j]='2'; //écurie 2  (en haut a droite)
            }
			else if((i>8)&&(j>8)){ //écurie 3 (en bas a droite)
                plateau[i][j]='3';
            }
			else if((i>8)&&(j<6)){ //écurie 4	(en bas a gauche)
                plateau[i][j]='4';
            }
		}	
	}
	plateau[7][7]='X';// centre du plateau (case 80 en cordonnée 1D)
	for(i=1;i<7;i++){
        plateau[7][i]= '0' + i;
    }//escalier joueur 1
	for(i=1;i<7;i++){
        plateau[i][7]= '0' + i;
    }//escalier joueur 2
	for(i=8;i<14;i++){
        plateau[i][7]= '7'- i + 7;
    }//escalier joueur 3
	for(i=8;i<14;i++){
        plateau[7][i]= '7'- i+7;
    }
    //escalier joueur 4
}



/* 
  param1 :plateau sous forme de matrice
  param2: tableau des positions des pions (en une dimensions ex: 0, 15, 16 et 55)
  param3: le nombre de joueurs dans la partie 
  cette fonction va afficher le plateau et va afficher chaque joueur avec une couleur différente
*/

void afficherPlateau(char plateau[15][15], int positionDesPions[], int nombreDejoueurs) {

    _Position2D *positionPions2D = (_Position2D*) malloc(sizeof(_Position2D) * nombreDejoueurs);

    for (int i = 0; i < nombreDejoueurs; i++) {
        //on recupere chaque cordonnées de pion en une dimension et on les traduits une par une en cordonnées a deux dimensions 
        //on stock tous les resultats dans un tableau
        positionPions2D[i] = positionPlateau1Dto2D(positionDesPions[i], numFilsToJoueur(i));
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            
            bool checkOccupeeParJoueur = false;//vérifie si la case courante est occupée par un joueur 

            for (int k = 0; k < nombreDejoueurs; k++) {
                if (positionPions2D[k].x == i && positionPions2D[k].y == j) {//la case est occupé par un joeur
                    checkOccupeeParJoueur = true; 
                    // => on va l'afficher en couleur
                    switch (k) {
                        case 0: //joueur 1
                            printf("\033[31m"); //rouge
                            printf("%c ", plateau[i][j]);
                            printf("\033[00m");
                        break;
                        case 1://joueur 2
                            printf("\033[34m" ); //bleu
                            printf("%c ", plateau[i][j]);
                            printf("\033[00m");
                        break;
                        case 2://joueur 3
                            printf("\033[36m" );//cyan
                            printf("%c ", plateau[i][j]);
                            printf("\033[00m");
                        break;
                        case 3://joueur 4
                            printf("\033[35m" );//mauve
                            printf("%c ", plateau[i][j]);
                            printf("\033[00m");
                        break;
                    }
                }
            }

            if (!checkOccupeeParJoueur) {
                printf("%c ", plateau[i][j]);
            }

        }
        printf("\n");
    }

    free(positionPions2D);
}




/* 
  param1 : cordonée d'un pion en une dimensions (de 0 à 80)
  param2: numéro du joueur associé au pion
  
  retour : cette fonction va traduire la cordonnée en une dimension en cordonnée a deux dimensions et la retourner 

  exemple, la case centrale est située à la cordonnée 80 en une dimension
  sur le plateau elle correspond a la case (7,7)
*/



_Position2D positionPlateau1Dto2D(int p1D, int numJoueur) { // !!! attention 0 <= p1d <= 55 
    _Position2D p2D;

    // ---------------------------------- cases où les joueurs sont dans leurs écuries respectives ----------------------------------

   if (p1D == ECURIE) { //définie arbitrairement
        switch (numJoueur) { //le joueur un est dans son écurie
            case 1: //coin supérieur gauche
                p2D.x = 0; 
                p2D.y = 0;
            break;
            case 2: //le joueur deux est dans son écurie (coin supérieur droit)
                p2D.x = 0;
                p2D.y = 14;
            break;
            case 3: //le joueur trois est dans son écurie (coin inférieur droit)
                p2D.x = 14;
                p2D.y = 14;
            break;
            case 4:  //le joueur quatre est dans son écurie (coin inférieur gauche)
                p2D.x = 14;
                p2D.y = 0;
            break;
        }
   }
   // ---------------------------------- toutes les par lesquels les joueurs parcours le plateau  ----------------------------------
   else if (p1D >= 0 && p1D <= 6) {
        p2D.x = 6;
        p2D.y = p1D;
    } else if (p1D >= 7 && p1D <= 11) {
        p2D.x = 5 - (p1D - 7);
        p2D.y = 6;
    } else if (p1D >= 12 && p1D <= 14) {
        p2D.x = 0;
        p2D.y = 6 + (p1D - 12);
    } else if (p1D >= 15 && p1D <= 19) {
        p2D.x = 1 + (p1D - 15);
        p2D.y = 8;
    } else if (p1D >= 20 && p1D <= 26) {
        p2D.x = 6;
        p2D.y = 8 + (p1D - 20);
    } else if (p1D == 27) {
        p2D.x = 7;
        p2D.y = 14;
    } else if (p1D >= 28 && p1D <= 34) {
        p2D.x = 8;
        p2D.y = 14 - (p1D - 28);
    }
    else if (p1D >= 35 && p1D <= 39) {
        p2D.x = 9 + (p1D - 35);
        p2D.y = 8;
    } else if (p1D >= 40 && p1D <= 42) {
        p2D.x = 14;
        p2D.y = 8 - (p1D - 40);
    } else if (p1D >= 43 && p1D <= 47) {
        p2D.x = 13 - (p1D - 43);
        p2D.y = 6;
    } else if (p1D >= 48 && p1D <= 54) {
        p2D.x = 8;
        p2D.y = 6 - (p1D - 48);
    }
    else if (p1D == 55) {
        p2D.x = 7;
        p2D.y = 0;

    // ---------------------------------- cas des escaliers des joueurs ----------------------------------
    } else if (p1D >= 56 && p1D <= 61) { //escalier joueur 1
        p2D.x = 7;
        p2D.y = 1 + (p1D - 56);
    }
    else if (p1D >= 62 && p1D <= 67) { //escalier joueur 2
        p2D.x = 1 + (p1D - 62);
        p2D.y = 7;
    }
    else if (p1D >= 68 && p1D <= 73) { //escalier joueur 3
        p2D.x = 7;
        p2D.y = 13 - (p1D - 68);
    }
    else if (p1D >= 74 && p1D <= 79) { //escalier joueur 4
        p2D.x = 13 - (p1D - 74);
        p2D.y = 7;
    }
    else if (p1D == 80) { //case centrale du plateau (cas où le joueur a gagné)
        p2D.x = 7;
        p2D.y = 7;
    }

    return p2D;
}


/*
  param1  numéro du joueur
  param2 : cordonnée  actuelle du joueur en une dimension sur le plateau (de 0 à 80)
  param3: la valeur du dé qu'il vient de jouer
  
  retour : cette fonction retourne la nouvelle cordonnées

  ex: le joueur est a la case 55 (devant son escalier) et lance le dé et obtient un 
  => il est maintenant en position 56 (premiere case de son escalier)
*/



int getNewPosition(int idJoueur, int position1DPlateauJoueur, int valeurDuDe) {
    int newPosition = -1;

	if (position1DPlateauJoueur == ECURIE && valeurDuDe == 6){ //cas où le joueur doit sortir de son écurie

        switch (idJoueur) {
			case 1:
			    newPosition = 0;
			    break;
			case 2:
			    newPosition = 14;
			    break;
			case 3:
			    newPosition = 28;
			    break;
			case 4:
			    newPosition = 42;
			    break;
		}
    }
    else if (position1DPlateauJoueur != ECURIE) {
        switch (idJoueur) {
            case 1://cas joueur 1
                if (position1DPlateauJoueur == 55) {//devant escalier 
                    if (valeurDuDe == 1) //si la valeur du de = 1 on monte dans l'escalier
                        newPosition = 56;
                    else
                        newPosition = 55;//sinon on reste a 55 ...   
                }
                else if (position1DPlateauJoueur >= 56 && position1DPlateauJoueur <= 61) {//joueur 1 dans son escalier

                    if (position1DPlateauJoueur == 56 && valeurDuDe == 2) {
                        newPosition = 57;
                    } else if (position1DPlateauJoueur == 57 && valeurDuDe == 3) {
                        newPosition = 58;
                    }
                    else if (position1DPlateauJoueur == 58 && valeurDuDe == 4) {
                        newPosition = 59;
                    } else if (position1DPlateauJoueur == 59 && valeurDuDe == 5) {
                        newPosition = 60;
                    } else if (position1DPlateauJoueur == 60 && valeurDuDe == 6) {
                        newPosition = 61;
                    }
                    else if (position1DPlateauJoueur == 61 && valeurDuDe == 6) {
                        newPosition = 80; //case centrale => joueur 1 a gagné
                    } else {
                        //sinon, le joueur avance de valeurDuDe cases
                        newPosition = position1DPlateauJoueur;
                    }

                }
                else if((valeurDuDe + position1DPlateauJoueur) > 55) {
                    //ici on sort de la limite
                    //donc on recule du nombre de cases que l'on a dépassé la limite
                    newPosition = 55 - (position1DPlateauJoueur + valeurDuDe - 55);
                }
                else { //ici on parcours le plateau sans depasser la limite
                    newPosition = position1DPlateauJoueur + valeurDuDe;
                }
            break;
            case 2://cas joueur 2
                 if (position1DPlateauJoueur == 13) {//joueur deux est devant son escalier
                    if (valeurDuDe == 1) //si valeur du de = 1 on monte dans l'escalier
                        newPosition = 62;
                    else
                        newPosition = 13;//sinon on reste a 13 .
                 }
                 else if (position1DPlateauJoueur >= 62 && position1DPlateauJoueur <= 67) {//joueur 2 est dans son escalier
                    //cas on est dans l'escalier 2
                    if (position1DPlateauJoueur == 62 && valeurDuDe == 2) {
                        newPosition = 63;
                    } else if (position1DPlateauJoueur == 63 && valeurDuDe == 3) {
                        newPosition = 64;
                    }
                    else if (position1DPlateauJoueur == 64 && valeurDuDe == 4) {
                        newPosition = 65;
                    } else if (position1DPlateauJoueur == 65 && valeurDuDe == 5) {
                        newPosition = 66;
                    } else if (position1DPlateauJoueur == 66 && valeurDuDe == 6) {
                        newPosition = 67;
                    }
                    else if (position1DPlateauJoueur == 67 && valeurDuDe == 6) {
                        newPosition = 80; //case centrale => joueur 2 a gagné
                    } else {
                        //sinon, le joueur avance de valeurDuDe cases
                        newPosition = position1DPlateauJoueur;
                    }
                 }
                 else if (position1DPlateauJoueur + valeurDuDe > 55) { //le joueur 2 repasse par la case de depart
                    newPosition = (position1DPlateauJoueur + valeurDuDe) % 56;
                 }
                 else {
                    if (position1DPlateauJoueur <= 13 && (position1DPlateauJoueur + valeurDuDe > 13)) {
                        //cas ou on recule du nombre de cases en trop
                        newPosition = 13 - (position1DPlateauJoueur + valeurDuDe - 13); 
                    }
                    else {
                        newPosition = position1DPlateauJoueur + valeurDuDe;
                        ///sinon, le joueur avance de valeurDuDe cases
                    }
                 }
            break;
            case 3://cas joueur 3
                if (position1DPlateauJoueur == 27) {
                    if (valeurDuDe == 1) //si  valeur du de = 1 on monte dans l'escalier
                        newPosition = 68;
                    else
                        newPosition = 27; //sinon le joueur 3 reste devant son escalier
                 }
                 else if (position1DPlateauJoueur >= 68 && position1DPlateauJoueur <= 73) {
                    //cas on est dans l'escalier 3
                    if (position1DPlateauJoueur == 68 && valeurDuDe == 2) {
                        newPosition = 69;
                    } else if (position1DPlateauJoueur == 69 && valeurDuDe == 3) {
                        newPosition = 70;
                    }
                    else if (position1DPlateauJoueur == 70 && valeurDuDe == 4) {
                        newPosition = 71;
                    } else if (position1DPlateauJoueur == 71 && valeurDuDe == 5) {
                        newPosition = 72;
                    } else if (position1DPlateauJoueur == 72 && valeurDuDe == 6) {
                        newPosition = 73;
                    }
                    else if (position1DPlateauJoueur == 73 && valeurDuDe == 6) {
                        newPosition = 80; //case centrale => joueur 3 a gagné
                    } else {
                        //sinon le joueur avance de valeurDuDe cases
                        newPosition = position1DPlateauJoueur;
                    }
                 }
                 //cas ou le joueur 3 repasse par la case de depart
                 else if (position1DPlateauJoueur + valeurDuDe > 55) {
                    newPosition = (position1DPlateauJoueur + valeurDuDe) % 56;
                 }
                 else {
                    if (position1DPlateauJoueur <= 27 && (position1DPlateauJoueur + valeurDuDe > 27)) {
                        //cas ou on recule du nombre de cases en trop
                        newPosition = 27 - (position1DPlateauJoueur + valeurDuDe - 27); 
                    }
                    else {
                        //sinon le joueur 3 avance de valeurDuDe cases
                        newPosition = position1DPlateauJoueur + valeurDuDe;
                    }
                 }
                break;
            case 4://cas joueur 4
                if (position1DPlateauJoueur == 41) { //le joueur 4 est devant son escalier
                    if (valeurDuDe == 1) //si la valeur du de = 1 on monte dans l'escalier
                        newPosition = 74;
                    else
                        newPosition = 41;//sinon il reste devant son escalier
                 }
                 else if (position1DPlateauJoueur >= 74 && position1DPlateauJoueur <= 79) {
                    //cas on est dans l'escalier 4
                    if (position1DPlateauJoueur == 74 && valeurDuDe == 2) {
                        newPosition = 75;
                    } else if (position1DPlateauJoueur == 75 && valeurDuDe == 3) {
                        newPosition = 76;
                    }
                    else if (position1DPlateauJoueur == 76 && valeurDuDe == 4) {
                        newPosition = 77;
                    } else if (position1DPlateauJoueur == 77 && valeurDuDe == 5) {
                        newPosition = 78;
                    } else if (position1DPlateauJoueur == 78 && valeurDuDe == 6) {
                        newPosition = 79;
                    }
                    else if (position1DPlateauJoueur == 79 && valeurDuDe == 6) {
                        newPosition = 80; //case centrale => joueur 4 a gagné
                    } else {
                        //sinon le joueur 4 avance de valeurDuDe nombre de cases
                        newPosition = position1DPlateauJoueur;
                    }
                 }
                //cas où le joueur 4 repasse par la case de depart
                 else if (position1DPlateauJoueur + valeurDuDe > 55) {
                    newPosition = (position1DPlateauJoueur + valeurDuDe) % 56;
                 }
                 else {
                    if (position1DPlateauJoueur <= 41 && (position1DPlateauJoueur + valeurDuDe > 41)) {
                        //cas ou on recule du nombre de cases en trop
                        newPosition = 41 - (position1DPlateauJoueur + valeurDuDe - 41); 
                    }
                    else {
                        newPosition = position1DPlateauJoueur + valeurDuDe;
                        //Sinon le joueur 4 avance de valeurDuDe nombre de cases
                    }
                 }
                break;
        }
    }

	return newPosition;
}

/*
  param1  positionCouranteDuJoueur (cordonnée en 1 dimension)
  param2 : numéro du joueur
  
  retour : retourne la valeur par defaut de la case à la position actuelle du joueur

  ex: par defaut la case 0 vaut '*', si le joueur est sur cette case et qu'il se deplace, il faut alors remttre la valeur par defaut
     donc * dans ce cas là
*/


char resetCase(int positionCourante, int numJoueur) {
    char valCase;

    if (positionCourante >= 0 && positionCourante <= 55) {
        valCase = '*';
    } 
    else if (positionCourante >= 56 && positionCourante <= 61) { //escalier 1
        valCase = positionCourante - 55 + '0';
    } 
    else if (positionCourante >= 62 && positionCourante <= 67) { //escalier 2
        valCase = positionCourante - 61 + '0';
    } 
    else if (positionCourante >= 68 && positionCourante <= 73) { //escalier 3
        valCase = positionCourante - 67 + '0';
    } 
    else if (positionCourante >= 74 && positionCourante <= 79) { //escalier 4
         valCase = positionCourante - 73 + '0';
    } 
    else if (positionCourante == ECURIE) { //le joueur venait de son ecurie
        valCase = numJoueur + '0';
    }

    return valCase;
}