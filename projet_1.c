#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include "entree_sortie.h"
#include "tubes.h"
#include "jeu.h"
#include "aleatoire.h"



int main(int argc, char **argv) {
    int nombreDejoueurs = recupererNombreDeJoueurs(argc, argv);

    int numFils;//pour stocker les numéros des fils
    pid_t pid;//pour stocker le retour de fork
    int numProchain = 0; //numéro du prochain fils dans l'anneau
    int nombreTubes = nombreDejoueurs * 2 + 1;//nombres de tubes du programme
    int tubes[nombreTubes][2];//tubes anonymes utilisés par les procesus
    bool jeuTermine = false;//indique si le jeu est terminé ou non

    numProchain = readPremierJoueur(nombreDejoueurs);//demande a l'utilisateur d'entrer le numéro du prochain joueur
    creerTubes(tubes, nombreTubes); //création des tubes


    for (numFils = 0; numFils < nombreDejoueurs; numFils++) {//boucle de création des processus fils
        pid = fork();

        if (pid == 0){ //si on est un processus fils on sort de la boucle
            initialiseGenerateurDeNombre(); //on initialise le générateur de nombre aléatoire pour
            break;
        }
    }

    if (pid == 0) {//
             //declarations variables des fils ici 
            _Joueur joueur; //informations relatives a chaque joueur
            _InformationsDuTour infosTour; //a transmettre au fils suivant dans l'anneau
            _CodeRetour codeRetour; //a transmettre au processus pere

            fermerExtermiteesNonUtiliseesFils(numFils, nombreDejoueurs, tubes);//ferme les extrémités des tubes non utilisées

            joueur.positionsDesPions = (int *) malloc(sizeof(int) * nombreDejoueurs);
            memset(joueur.positionsDesPions, ECURIE, sizeof(int) * nombreDejoueurs); //on itialiase la postion  de chaque pion
            initPlateau(joueur.plateau); //on initialise le plateau de chaque processus fils

            infosTour.numJoueur = numFilsToJoueur(numFils); //on stock le numéro du joueur correspond au fils

            while (!jeuTermine) { //les fils boucle tant que le jeu n'est pas terminé
                int quiJoue;//recupere le numéro du fils qui doit joueur
                
                //on recupere le numéro envoyé par le processus pere
                if (read(tubes[nombreDejoueurs + 1 + numFils][0], &quiJoue, sizeof(quiJoue)) == -1) {
                    perror("read");
                    exit(EXIT_FAILURE);
                }

                if (numFils == quiJoue) { //si mon numéro de processus vaut celui qui doit joueur => je joue

                    // ---- traitement de celui qui joue ----

                    int valeurDuDe = joueurLanceDe(); //le fils qui joue lance le de 

                    printf("Joueur#%d lance le dé et obtient %d\n", numFilsToJoueur(numFils), valeurDuDe);
                    
                    //calcul de la nouvelle position
                    //on recupere la position du tour precedent

                    int anciennePosition = joueur.positionsDesPions[numFils];

                    //on traduit l'ancienne position en coordonnées en 2D
                    _Position2D anciennePosition2D = positionPlateau1Dto2D(anciennePosition, numFilsToJoueur(numFils));
                    
                    //on remet la valeur initiale dans le plateau (pour la case liée à l'ancienne position)

                    joueur.plateau[anciennePosition2D.x][anciennePosition2D.y] = resetCase(anciennePosition, numFilsToJoueur(numFils));

                    //on calcul la nouvelle position 

                    joueur.positionsDesPions[numFils] = getNewPosition(numFilsToJoueur(numFils), anciennePosition, valeurDuDe);

                    //on met a jour le plateau a la case correspondant à la nouvelle position

                    _Position2D nouvellePosition2D = positionPlateau1Dto2D(joueur.positionsDesPions[numFils], numFilsToJoueur(numFils));

                    joueur.plateau[nouvellePosition2D.x][nouvellePosition2D.y] = numFilsToJoueur(numFils) + '0';

                    // ----- fin traitement de celui qui joue -----


                    // ----- envoie des informations du tour au joueur suivant dans l'anneau ------- 

                    infosTour.valDe = valeurDuDe; //valeur du dé lancé
                    infosTour.numJoueur = numFilsToJoueur(numFils); //numéro du joueur qui a joué
                    infosTour.newPosition = joueur.positionsDesPions[numFils];//nouvelle position du joueur

                    //on transmet les informations au suivant dans l'anneau
                    if (write(tubes[numFils][1], &infosTour, sizeof(infosTour)) == -1) {
                        perror("write");
                        exit(EXIT_FAILURE);
                    }

                    if (infosTour.newPosition == 80) { //si sa position vaut 80 (case centrale, on va arreter le jeu)
                        jeuTermine = true;
                    }
                        
                    // ----- fin envoie des informations au joueur suivant dans l'anneau ------- 
                }
                else {//les autres fils dans l'anneau 
                    int ouDoisJeLire = ouJeLisDansAnneau(numFils, nombreDejoueurs); //on recupere l'index du tube où on doit lire
                    
                    //je recupere les informations de celui qui vient de jouer

                    //on recupere les informations depuis ce tube 
                    if (read(tubes[ouDoisJeLire][0], &infosTour, sizeof(infosTour)) == -1) {
                        perror("read");
                        exit(EXIT_FAILURE);
                    }
                   

                    if (infosTour.newPosition == 80) //si celui qui a joué est à la position 80 on arrette le jeu
                        jeuTermine = true;

                    int numDeCeluiQuiAjoue = infosTour.numJoueur; //je recupere son numéro de joueur
                    int nouvellePositionDeCeluiQuiAjoue = infosTour.newPosition; //je recupere la nouvelle position de son pion

                    //j'actualise mon plateau

                    //je reset la case où celui qui a joué été avant ce tour 

                    int anciennePositionDeCeluiQuiAjoue = joueur.positionsDesPions[numJoueurToFils(numDeCeluiQuiAjoue)];
                    _Position2D anciennePosition2D = positionPlateau1Dto2D(anciennePositionDeCeluiQuiAjoue, numDeCeluiQuiAjoue);
                    joueur.plateau[anciennePosition2D.x][anciennePosition2D.y] = resetCase(anciennePositionDeCeluiQuiAjoue, numDeCeluiQuiAjoue);

                    //j'actualise mon plateau avec sa nouvelle postion

                    _Position2D nouvellePosition2D = positionPlateau1Dto2D(nouvellePositionDeCeluiQuiAjoue, numDeCeluiQuiAjoue);
                    joueur.plateau[nouvellePosition2D.x][nouvellePosition2D.y] = numDeCeluiQuiAjoue + '0';
                    

                    //j'actualise ma liste des postions des pions

                    if (joueur.positionsDesPions[numFils] == nouvellePositionDeCeluiQuiAjoue) {
                        joueur.positionsDesPions[numFils] = ECURIE;//retour ecurie (colision)
                    } else {
                        joueur.positionsDesPions[numJoueurToFils(numDeCeluiQuiAjoue)] = nouvellePositionDeCeluiQuiAjoue;
                    }

                    if (numFils == numDernier(quiJoue, nombreDejoueurs)) { //processus qui envoie le code de retour au pere
                        //le dernier dans l'anneau affiche son plateau
                        usleep(100000); //enlever cette ligne pour enlever les latences a l'affichage
                        afficherPlateau(joueur.plateau, joueur.positionsDesPions, nombreDejoueurs);
                        
                        //informations a envoyer au processus pere
                        codeRetour.valDe = infosTour.valDe;
                        codeRetour.gagneLaPartie = (infosTour.newPosition == 80) ? (true) : (false);

                        write(tubes[nombreDejoueurs][1], &codeRetour, sizeof(codeRetour));
                    }
                    else {
                        //on envoie les informations au processus fils suivant dans l'anneau
                        if(write(tubes[numFils][1], &infosTour, sizeof(infosTour)) == -1) {//j'envoie au suivant
                            perror("write");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
            }

            free(joueur.positionsDesPions);
            fermerExtermiteesUtiliseesFils(numFils, nombreDejoueurs, tubes);

            exit(0);
        
    } else { //pere
            _CodeRetour codeRetour;

            fermerExtermiteesNonUtiliseesPere(nombreDejoueurs, tubes);

            while (!jeuTermine) {
               
                printf("Pere écrit aux fils le numéro du prochain joueur (joueur #%d)\n", numFilsToJoueur(numProchain));


                for (int i = 0; i < nombreDejoueurs; i++) {
                    //j'écris à tous les fils le numéo de quel joueur va joueur
                    if (write(tubes[nombreDejoueurs + 1 + i][1], &numProchain, sizeof(numProchain)) == -1) {
                        perror("write");
                        exit(EXIT_FAILURE);
                    }
                }

                //le pere recupere le code de retour du dernier fils dans l'anneau
                
                if(read(tubes[nombreDejoueurs][0], &codeRetour, sizeof(codeRetour)) == -1) {
                    perror("read");
                    exit(EXIT_FAILURE);
                }

                printf("resultat du dé lancé depuis le pere = %d\n", codeRetour.valDe);
               
                if (codeRetour.gagneLaPartie == true) { //si dans le code de retour le joueur qui vient de jouer a gagné 
                    break; // => on sort de la boucle infinie
                }

                if (codeRetour.valDe != 6) { //si resultat différent de 6 joueur suivant
                    numProchain = getNumeroDuProchain(numProchain, nombreDejoueurs);
                }
            }
    }

    printf("Le joueur %d a gagné !!!\n", numFilsToJoueur(numProchain));

    while (wait(NULL) != -1); //on attend la terminaison des processus fils

    fermerExtermiteesUtiliseesPere(nombreDejoueurs, tubes);   

    return 0;
}
