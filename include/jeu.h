#ifndef JEU_H
#define JEU_H


#include <stdbool.h>

#define ECURIE  -1

struct Position2D {//coordonnées en deux dimensions (pour l'affichage du plateau sous forme de matrice 15*15)
	int x;
	int y;
};
typedef struct Position2D _Position2D;

struct Joueur {
    char plateau[15][15]; //chaque joueur a en mémoire son propre plateau
    int *positionsDesPions; //contient toutes les coordonnées des pions sous forme d'un tableau à 1 dimensions
};
typedef struct Joueur _Joueur;

struct InformationsDuTour { //informations relatives au tour courant (transite dans l'anneau de processus en processus) 
    int numJoueur; //numéro du joueur qui a joué 
    int newPosition; //nouvelle position du joueur après avoir lancé le dé                  
    int valDe; //valeur du dé obtenu
};
typedef struct InformationsDuTour _InformationsDuTour;

struct CodeRetour { //code de retour envoyé au processus père en sortie de l'anneau
    bool gagneLaPartie; //vaut true si le joueur qui a joué a sa position à 80 (case centrale)
    int valDe; //valeur du dé que le joueur a joué
};
typedef struct CodeRetour _CodeRetour;

int numFilsToJoueur(int numFils);
int numJoueurToFils(int numJoueur);
int getNumeroDuProchain(int numQuiVientDeJouer, int nombreJoueurs);
void initPlateau(char plateau[15][15]);
void afficherPlateau(char plateau[15][15], int positionDesPions[], int nombreDejoueurs);
_Position2D positionPlateau1Dto2D(int p1D, int numJoueur);
int getNewPosition(int idJoueur, int position1DPlateauJoueur, int valeurDuDe);
char resetCase(int positionCourante, int numJoueur);

#endif