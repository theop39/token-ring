#include <stdio.h>
#include <stdlib.h>
#include "entree_sortie.h"
#include "jeu.h"


/* ------------------------------------------------ */
// Fichier source qui regroupe toutes les fonctions 
// pour gérer les entrées sorties
/* ------------------------------------------------ */

/* 
  param1 : argc (nombre d'argument(s) de la ligne de commande + 1, transmis au main) 
  param2 : argv (Liste des argument(s) de la ligne de commande + le nom de la ligne de commande, transmis au main)
  retour : nombre de joueurs entre 2 et 4 
*/


int recupererNombreDeJoueurs(int argc, char *argv[]) {
    int nombreDejoueurs;

    if (argc == 2) {
        nombreDejoueurs = atoi(argv[1]);

        if (nombreDejoueurs < 2 || nombreDejoueurs > 4) {
            fprintf(stderr, "Erreur: le nombre de joueurs doit être compris entre 2 et 4\n");
            exit(EXIT_FAILURE);
        }
        
    } else {
        fprintf(stderr, "(Erreur) Usage: petitChevaux nombreDejoueurs\n");
        exit(EXIT_FAILURE);
    }

    return nombreDejoueurs;
}

/* 
  param1 : va prendre en paramètre le nombre de joueur
  retour : retourne un entier compris entre 1 et le nombre de joueurs qui correspond au numéro du joueur qui va commencer la partie 
*/

int readPremierJoueur(int nombreDejoueurs) {
    char saisie[255];
    int num;

    while (1)
    {
        printf("Saisir le numéro du joueur qui joue en premier\n");

        displayPrompt();

        fgets(saisie, 255, stdin);
        
        if (sscanf(saisie, "%d", &num) == 1 && (num >= 1 && num <= nombreDejoueurs)) {
            break;
        }
        
        printf("Erreur, la saisie est incorrecte\n"
               "Le numéro doit être compris entre 1 et %d\n", nombreDejoueurs); 
    }

    return numJoueurToFils(num);
}

void displayPrompt(void) {
    printf(">>");
}