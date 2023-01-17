#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------ */
// Fichier source qui regroupe toutes les fonctions 
// qui permettent de manipuler les tubes
/* ------------------------------------------------ */

/* 
  param1 : tableaux contenant les tubes
  param2 : nombre de tubes

  détails:
  soit n nombre de joueurs

  les tubes tubes[0] à tubes[n - 1] représentent les tubes qui forment l'anneau dans lequel tous 
  les processus fils/joueurs y communiquent à travers
  le tube tubes[n] est le tube ou le processus pere recoit le code de retour
  les tubes tubes[n + 1] à tubes[2n] sont les tubes où le pere envoie simultanément le numéro du prochain joueur qui doit joue
*/


void creerTubes(int tubes[][2], int nombreTubes) {
     for (int i = 0; i < nombreTubes; i++) {
        if (pipe(tubes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
}



/* 
  param1 : numéro du processus fils dans l'anneau
  param2 : nombre de processus fils
  retour: retourne l'index du tube où le processus doit lire dans l'anneau

*/



int ouJeLisDansAnneau(int numProcessus, int nbProcessus) {
  return (numProcessus == 0 ? (nbProcessus - 1) : (numProcessus - 1));
}


/* 
  param1 : numéro du processus fils qui a joué 
  param2 : nombre de processus fils
  retour: retourne le numéro du dernier processus dans l'anneau (celui qui va communiquer le code de retour au pere)

*/


int numDernier(int numPremier, int nombreProcessus) {
  if (numPremier == 0) 
    return nombreProcessus - 1;
  else 
    return numPremier - 1;
}


/* 
  param1 : numéro du processus fils
  param2 : nombre de processus fils
  param3: tableau de l'ensemble des tubes
  
  la fonction va fermer toutes les extrémitées des tubes non utilisées par le processus fils dont le numéro est donné en paramètre
*/



void fermerExtermiteesNonUtiliseesFils(int numFils, int nombreJoueurs, int tubes[][2]) {
  int nombreTubes = (2 * nombreJoueurs) + 1;

  for (int i = 0; i < nombreTubes; i++) {
    if (i >= 0 && i <= (nombreJoueurs - 1)) {//ici i = index des tubes qui composent l'anneau
      if (i == numFils) {
        if (close(tubes[i][0]) == -1) {
          perror("close");
          exit(EXIT_FAILURE);
        }
      } else if (i == ouJeLisDansAnneau(numFils, nombreJoueurs - 1)) {
        if(close(tubes[i][1]) == -1) {
          perror("close");
          exit(EXIT_FAILURE);
        }
      } else {
        if(close(tubes[i][1]) == -1) {
          perror("close");
          exit(EXIT_FAILURE);
        }
        //close(tubes[i][0]);
      }
    } else if (i == nombreJoueurs) {//ici i = index du tube partagée
      if (close(tubes[i][0]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
    } else if (i > nombreJoueurs && i < nombreTubes) {//ici i = index des tubes où les processus fils recoivent le numéro du prochain joueur
      if (i == (nombreJoueurs + 1 + numFils)) {
        if (close(tubes[i][1]) == -1) {
          perror("close");
          exit(EXIT_FAILURE);
        }
      } else {
        if(close(tubes[i][1]) == -1) {
          perror("close");
          exit(EXIT_FAILURE);
        }
        if(close(tubes[i][0]) == -1){
          perror("close");
          exit(EXIT_FAILURE);
        }
      }
    }
  }
}

/* 
  param1 : numéro du processus fils
  param2 : nombre de processus fils
  param3: tableau de l'ensemble des tubes
  
  la fonction va fermer toutes les extrémitées des tubes utilisées par le processus fils dont le numéro est donné en paramètre
*/



void fermerExtermiteesUtiliseesFils(int numFils, int nombreJoueurs, int tubes[][2]) {
  if (close(tubes[numFils][1]) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
  if (close(tubes[ouJeLisDansAnneau(numFils, nombreJoueurs - 1)][0]) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
  if (close(tubes[nombreJoueurs][1]) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
  if (close(tubes[nombreJoueurs + 1 + numFils][0]) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
}


/* 
  param1 : numéro nombre de joueurs
  param2: tableau de l'ensemble des tubes
  
  la fonction va fermer toutes les extrémitées des tubes non utilisées par le processus pere
*/


void fermerExtermiteesNonUtiliseesPere(int nombreJoueurs, int tubes[][2]) {
  int nombreTubes = (nombreJoueurs * 2) + 1;

  for (int i = 0; i < nombreJoueurs; i++) {
    if (close(tubes[i][0]) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }
    if (close(tubes[i][1]) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }
  }

  if (close(tubes[nombreJoueurs][1]) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  for (int i = nombreJoueurs + 1; i < nombreTubes; i++) {
    if (close(tubes[i][0]) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }
  }
}



/* 
  param1 : numéro nombre de joueurs
  param2: tableau de l'ensemble des tubes
  
  la fonction va fermer toutes les extrémitées des tubes utilisées par le processus pere
*/


void fermerExtermiteesUtiliseesPere(int nombreJoueurs, int tubes[][2]) {
  int nombreTubes = (nombreJoueurs * 2) + 1;
  
  if (close(tubes[nombreJoueurs][0]) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  for (int i = nombreJoueurs + 1; i < nombreTubes; i++) {
    if (close(tubes[i][1]) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }
  }
}

