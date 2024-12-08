#include "generation_terrains.h"
#include "terrain.h"
#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf(
        "Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for (i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch (e.type_err) {
  case OK_PROGRAMME:
    break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}


int main (int argc, char* argv[]){
    Terrain T;
    Environnement envt;
    Programme prog;
    erreur_terrain errt;
    erreur_programme errp;
    etat_inter etat;
    resultat_inter res;
    int N,l,h, nb_step_max, nb_pas;
    unsigned int graine;
    float pas_total=0;
    FILE *resFile;
    FILE *file;
    float dObst=0;
    N = strtol(argv[2], NULL, 10);
    l = strtol(argv[3], NULL, 10);
    h = strtol(argv[4], NULL, 10);
    dObst = strtof(argv[5], NULL);
    if (l > DIM_MAX || l % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
    }
    if (h > DIM_MAX || h % 2 == 0) {
        printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
        return 1;
    }
    if ((dObst > 1) || (dObst < 0)) {
        printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
        return 1;
    }
    graine = strtol(argv[6], NULL, 10);
    nb_step_max = strtol(argv[7], NULL, 10);
    resFile = fopen(argv[8], "w");
    if (resFile==NULL){
        printf("Erreur lors de l'ouverture de %s\n", argv[8]);
    }
    
    fprintf(resFile, "Nombre de terrains testés:%d\n", N);
    srand(graine);
    int x=(int)l/2;
    int y=(int)h/2;
    int nb_sorties=0;
    int nb_bloqués=0;
    int nb_obstacles=0;
    float pourc_sorties, pourc_bloqués, pourc_obstacles;
    for (int i=0;i<N;i++){
        nb_pas=0;
        file=fopen("fichier_ecriture_terrain.txt","w");
        if (file==NULL){
          printf("Erreur lors de l'ouverture du fichier d'écriture des terrains");
        }
        do {
          generation_aleatoire(&T,l,h,dObst);
        } while (!existe_chemin_vers_sortie(&T));
        ecrire_terrain(file,&T,x,y);
        fclose(file);
         /* Initialisation de l'environnement : lecture du terrain,
            initialisation de la position du robot */
        errt = initialise_environnement(&envt, "fichier_ecriture_terrain.txt");
        gestion_erreur_terrain(errt);

        /* Lecture du programme */
        errp = lire_programme(&prog, argv[1]);
        gestion_erreur_programme(errp);

        /* Initialisation de l'état */
        init_etat(&etat);
        res=OK_ROBOT;
        // do {
        //     res = exec_pas(&prog, &envt, &etat);
        //     /* Affichage du terrain et du robot */
            
        // } while (res == OK_ROBOT);
        for(int j=0;j<nb_step_max && res==OK_ROBOT;j++){
          res=exec_pas(&prog,&envt,&etat);
          nb_pas++;
        }

        /* Affichage du résultat */
        switch (res) {
        case OK_ROBOT:
            fprintf(resFile,"%d\n",-1);
            nb_bloqués++;
            break;
        case PLOUF_ROBOT:
            fprintf(resFile,"%d\n",-2);
            nb_obstacles++;
            break;
        case CRASH_ROBOT:
            fprintf(resFile,"%d\n",-3);
            nb_obstacles++;
            break;
        case SORTIE_ROBOT:
           fprintf(resFile,"%d\n",nb_pas+1);
           pas_total=pas_total+nb_pas+1;
           nb_sorties++;
           break;
        default:
          break;
        }
    }
    fclose(resFile);
    pourc_bloqués=((float)nb_bloqués/N)*100;
    pourc_obstacles = ((float)nb_obstacles / N) * 100;
    pourc_sorties=((float)nb_sorties/N)*100;
    pas_total=((float)pas_total/N);
    printf("Nombre de terrains où le robot est bloqué: %d et pourcentage: %.2f\n", nb_bloqués,pourc_bloqués);
    printf("Nombre de terrains où le robot a rencontré un obstacle: %d et pourcentage: %.2f\n", nb_obstacles, pourc_obstacles);
    printf("Nombre de terrains où le robot est sorti:%d et pourcentage: %.2f\n", nb_sorties, pourc_sorties);
    printf("Nombre moyen de pas effectué pour les sorties:%.2f\n", pas_total);
    return 0;
}
