#ifndef TUBES_H
#define TUBES_H

void creerTubes(int tubes[][2], int nombreTubes);
int ouJeLisDansAnneau(int numProcessus, int nbProcessus);
int numDernier(int numPremier, int nombreProcessus);
void fermerExtermiteesNonUtiliseesFils(int numFils, int nombreJoueurs, int tubes[][2]);
void fermerExtermiteesNonUtiliseesPere(int nombreJoueurs, int tubes[][2]);
void fermerExtermiteesUtiliseesFils(int numFils, int nombreJoueurs, int tubes[][2]);
void fermerExtermiteesUtiliseesPere(int nombreJoueurs, int tubes[][2]);


#endif