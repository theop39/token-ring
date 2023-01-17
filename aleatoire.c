#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include "aleatoire.h"
------------------------------------------- */

/* 
  param1 : borneInf (borne inférieur de l'intervalle) 
  param2 : borneSup (borne supérieur de l'intervalle)
  contrainte(s) : borneInf < borneSup
  retour : un entier compris dans l'intervalle [borneInf;borneSup] 
*/

int random_intervalle(int borneInf, int borneSup) {

  int longueurIntervalle = (borneSup - borneInf) + 1;//nb éléments dans l'intervalle

  return (rand() % longueurIntervalle) + borneInf;
}


/* 
  retour : un nombre compris entre 1 et 6
*/

int joueurLanceDe(void) {
  return random_intervalle(VALEUR_DE_MIN, VALEUR_DE_MAX);
}

/* 
  initialise le générateur de nombre aléatoire 
*/

void initialiseGenerateurDeNombre(void) {
  time_t t;
  srand((int) time( & t) % getpid()); //pour éviter que chaque fils genere le même nombre
}