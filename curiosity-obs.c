#include <stdio.h>
#include <stdlib.h>
#include "environnement.h"
#include "programme.h"
#include "interprete.h" 




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

void gestion_erreur_robot(resultat_inter res){
    switch (res)
    {
    case OK_ROBOT:
    printf("OK_ROBOT\n");
        break;

    case  SORTIE_ROBOT:
        printf("SORTIE_ROBOT\n");
        exit(3);

    case  ARRET_ROBOT:
        printf("ARRET_ROBOT\n");
        exit(3);

    case  PLOUF_ROBOT:
        printf("PLOUF_ROBOT\n");
        exit(3);

    case  CRASH_ROBOT:
        printf("CRASH_ROBOT\n");
        exit(3);

    case  ERREUR_PILE_VIDE:
        printf("ERREUR_PILE_VIDE\n");
        exit(3);
    
    case  ERREUR_ADRESSAGE:
        printf("ERREUR_ADRESSAGE\n");
        exit(3);
    
    case  ERREUR_DIVISION_PAR_ZERO:
        printf("ERREUR_DIVISION_PAR_ZERO\n");
        exit(3);
    
    default:
        exit(3);
    }
}



void etat_observateur(Environnement envt){
    switch (envt.etat_courant)
        {
        case Puit :
            printf("etat courant : Puit\n");
            break;

        case Init :
            printf("etat courant : Init\n");
            break;
        

        case Mes :
            printf("etat courant : Mes\n");
            break;
        
        default:
            break;
        }
}

int main(int argc, char **argv) {
    
    Environnement envt;
    Programme prog;
    erreur_terrain errt;
    erreur_programme errp;
    etat_inter etat;
    resultat_inter res;

    int a;

    if (argc != 3) {
        printf("Pas le bon nobre d'arguments, il faut le [nom d'un fichier terrain] et le [nom d'un programme-robot] \n");
        return 1;
    }

    errt = initialise_environnement(&envt, argv[1]);
    gestion_erreur_terrain(errt);

    errp = lire_programme(&prog, argv[2]);
    gestion_erreur_programme(errp);

    init_etat(&etat);
    res = OK_ROBOT;
    while (res == OK_ROBOT) {
        res = exec_pas(&prog, &envt, &etat);
        afficher_envt(&envt);
        etat_observateur(envt);
    }
    a = est_accepteur(envt.etat_courant);
    switch (a) {
        case 1:
            printf("accepté par l'observateur, donc la propriété est valide\n");
            gestion_erreur_robot(res);
            return 1;

        case 0:          
            printf("rejeté par l'observateur, donc la propriété n'est pas valide\n");
            gestion_erreur_robot(res);
            return 1;  
    }
    
    
}
