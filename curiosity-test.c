
#include <stdio.h>
#include <stdlib.h>
#include "environnement.h"
#include "interprete.h"   
#include "programme.h" 


void  gestion_erreur_terrain( erreur_terrain errt){
    switch (errt)
    {
    case OK :
        break;

    case ERREUR_FICHIER :
        printf("ERREUR_FICHIER\n");
        exit(1);

    case ERREUR_LECTURE_LARGEUR :
        printf("ERREUR_LECTURE_LARGEUR\n");
        exit(1);

    case ERREUR_LECTURE_HAUTEUR :
        printf("ERREUR_LECTURE_HAUTEUR\n");
        exit(1);

    case ERREUR_LARGEUR_INCORRECTE :
        printf("ERREUR_LARGEUR_INCORRECTE\n");
        exit(1);

    case ERREUR_HAUTEUR_INCORRECTE :
        printf("ERREUR_HAUTEUR_INCORRECTE\n");
        exit(1);

    case ERREUR_CARACTERE_INCORRECT :
        printf("ERREUR_CARACTERE_INCORRECT\n");
        exit(1);

    case ERREUR_LIGNE_TROP_LONGUE :
        printf("ERREUR_LIGNE_TROP_LONGUE\n");
        exit(1);

    case ERREUR_LIGNE_TROP_COURTE :
        printf("ERREUR_LIGNE_TROP_COURTE\n");
        exit(1);

    case ERREUR_LIGNES_MANQUANTES :
        printf("ERREUR_LIGNES_MANQUANTES\n");
        exit(1);

    case  ERREUR_POSITION_ROBOT_MANQUANTE:
        printf("ERREUR_POSITION_ROBOT_MANQUANTE\n");
        exit(1);

    default:
        exit(1);
    }

}


void gestion_erreur_programme(erreur_programme errp){
    switch (errp.type_err)
    {
    case OK_PROGRAMME:
        break;

    case  ERREUR_FICHIER_PROGRAMME:
        printf("ERREUR_FICHIER_PROGRAMME\n");
        exit(2);

    case  ERREUR_BLOC_NON_FERME:
        printf("ERREUR_BLOC_NON_FERME\n");
        exit(2);

    case  ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
        printf("ERREUR_FERMETURE_BLOC_EXCEDENTAIRE\n");
        exit(2);

    case  ERREUR_COMMANDE_INCORRECTE:
        printf("ERREUR_FICHIER_PROGRAMME\n");
        exit(2);
    
    default:
        exit(2);
    }



}


/* Affiche "Ok." et renvoie 0 si la position du robot correspond
aux coordonnées (ax, ay) avec l’orientation ao.
Sinon affiche les coordonnées obtenues et les coordonnées attendues,
et renvoie 1 */
int etat_attendu(Robot r, int ax, int ay, char ao) {
    int rx, ry;
    char ro;

    rx = abscisse(&r);
    ry = ordonnee(&r);
    switch (orient(&r)) {
        case Nord:
            ro = 'N';
            break;
        case Sud:
            ro = 'S';
            break;
        case Est:
            ro = 'E';
            break;
        case Ouest:
            ro = 'O';
            break;
    }

    if ((rx == ax) && (ry == ay) && (ro == ao)) {
        printf("Ok.\n");
        return 0;
    } else {
        printf("Position attendue : (%d, %d), %c\n", ax, ay, ao);
        printf("Position obtenue : (%d, %d), %c\n", rx, ry, ro);
        printf("Test échoué.\n");
        return 1;
    }
}

int main(int argc, char **argv) {
    FILE *ftest;
    char nom_fenvt[1000];
    char nom_fprog[1000];
    Environnement envt;
    Programme prog;
    erreur_terrain errt;
    erreur_programme errp;
    int nbstepmax;
    char cevent;
    int ax, ay;
    char ao;
    int nbstep;
    etat_inter etat;
    resultat_inter res;

    if (argc < 2) {
        printf("Usage: %s <fichier test>\n", argv[0]);
        return 1;
    }

    ftest = fopen(argv[1], "r");

    fscanf(ftest, "%s\n", nom_fenvt);
    errt = initialise_environnement(&envt, nom_fenvt);
    gestion_erreur_terrain(errt);

    fscanf(ftest, "%s\n", nom_fprog);
    errp = lire_programme(&prog, nom_fprog);
    gestion_erreur_programme(errp);

    fscanf(ftest, "%d\n", &nbstepmax);
    fscanf(ftest, "%c\n", &cevent);
    if ((cevent == 'N') || (cevent == 'F')) {
        fscanf(ftest, "%d %d\n", &ax, &ay);
        fscanf(ftest, "%c", &ao);
    }

    init_etat(&etat);
    res = OK_ROBOT;
    for (nbstep = 0; (nbstep < nbstepmax) && (res == OK_ROBOT); nbstep++) {
        res = exec_pas(&prog, &envt, &etat);
        afficher_envt(&envt);
    }

    switch (res) {
        case OK_ROBOT:
            printf("Robot sur une case libre, programme non terminé\n");
            if (cevent == 'N') {
                return etat_attendu(envt.r, ax, ay, ao);
            } else {
                printf("Test échoué, état attendu %c au lieu de N\n", cevent);
                return 1;
            }
        case SORTIE_ROBOT:
            printf("Le robot est sorti :-)\n");
            if (cevent == 'S') {
                printf("Ok.\n");
                return 0;
            } else {
                printf("Test échoué, état attendu %c au lieu de S\n", cevent);
                return 1;
            }
        case ARRET_ROBOT:
            printf("Robot sur une case libre, programme terminé \n");
            if (cevent == 'F') {
                return etat_attendu(envt.r, ax, ay, ao);
            } else {
                printf("Test échoué, état attendu %c au lieu de F\n", cevent);
                return 1;
            }
        case PLOUF_ROBOT:
            printf("Le robot est tombé dans l’eau :-(\n");
            if (cevent == 'P') {
                printf("Ok.\n");
                return 0;
            } else {
                printf("Test échoué, état attendu %c au lieu de P\n", cevent);
                return 1;
            }
        case CRASH_ROBOT:
            printf("Le robot s’est écrasé sur un rocher X-(\n");
            if (cevent == 'O') {
                printf("Ok.\n");
                return 0;
            } else {
                printf("Test échoué, état attendu %c au lieu de O\n", cevent);
                return 1;
            }
        case ERREUR_PILE_VIDE:
            printf("ERREUR : pile vide\n");
            return 1;
        case ERREUR_ADRESSAGE:
            printf("ERREUR : erreur d’adressage\n");
            return 1;
        case ERREUR_DIVISION_PAR_ZERO:
            printf("ERREUR : division par 0\n");
            return 1;
    }
    
}
