#include "generation_terrains.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nb_cases_occupees(Terrain *T) {
  int occupee = 0;
  int x, y;
  for (x = 0; x < largeur(T); x++) {
    for (y = 0; y < hauteur(T); y++) {
      if (T->tab[x][y] != LIBRE)
        occupee++;
    }
  }
  return occupee;
}

// Test de generation aléatoire de terrains
// Le programme génère n terrains de largeur et hauteur fixes
// avec largeur et hauteur impaires et inférieures a dimension_max de terrain.h
// avec densité d'obstacle dObst
// autre que la case centrale soit occupee
// l'appel du programme se fait avec 5 arguments :
// generation_terrains N largeur hauteur dObstacle fichier_res
// la sortie se fait dans le fichier resultat

int main(int argc, char **argv) {
  int N, l, h;
  float dObst=0;
  // int decim=0;
  // int inter;
  // int compt=0;
  FILE *resFile;
  Terrain T;
  //int c;

  if (argc < 6) {
    printf(
        "Usage: %s <N> <largeur> <hauteur> <densite_obstacle> <fichier_res> \n",
        argv[0]);
    return 1;
  }

  N = strtol(argv[1], NULL, 10);
  l = strtol(argv[2], NULL, 10);
  h = strtol(argv[3], NULL, 10);
  // for (int z=0;argv[4][z]!='\0';z++){
  //   inter=argv[4][z]-'0';
  //   printf("%d\n", inter);
  //   if ((argv[4][z]=='.') || (argv[4][z]==',')){
  //     c=1;
  //   }
  //   else if (c==1){
  //     decim=decim*10+inter;
  //     compt=compt+10;
  //   }
  //   else {
  //     dObst=dObst*10+inter;
  //   }
  // }
  // dObst=dObst+((float)decim/compt);
  dObst = strtof(argv[4], NULL);
  // sscanf(argv[5], "%f", &dObst);
  // test de l et h
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
  // Ouverture du fichier résultat
  resFile = fopen(argv[5], "w");
  if (resFile==NULL){
    printf("Problème lors de l'ouverture du fichier\n");
    return 0;
  }
  // Écriture du nombre de terrains
  

  // Initialisation de la fonction random
  // A compléter
  srand(time(NULL));
  int x=(int)l/2;
  int y=(int)h/2;
  int z=0;
  float g;
  float nb;
  float g1;
  float pourc=0;
  float pourcentage_final;
  fprintf(resFile, "%d\n", N);
  // A compléter
  // Génération aléatoire des terrains
  // Écriture des terrains générés dans le fichier resFile
  // Écriture/Affichage des statistiques
  for (int i=0;i<N;i++){

    do {
      generation_aleatoire(&T,l,h,dObst);
    } while (!existe_chemin_vers_sortie(&T));
    ecrire_terrain(resFile,&T,x,y);
    nb=(float)nb_cases_occupees(&T);
    g1=nb/(float)(l*h);

    fprintf(resFile,"Densité d'obstacles dans ce terrain:%f\n",g1);
    fprintf(resFile,"\n");
    if ((g1<=dObst+0.02) && (g1>=dObst-0.02)){
      pourc=pourc+1.0;
    }
    g=g+g1;
    z++;
  }
  g=(float)g/z;
  pourcentage_final=(pourc/(float)N)*100;
  fprintf(resFile,"Densité d'obstacles moyenne:%f\n",g);
  fprintf(resFile,"Densité d'obstacles attendue:%f\n",dObst);
  fprintf(resFile,"Pourcentage de densités d'obstacles valides à la densité attendue à 0.02 près:%f\n",pourcentage_final);
  // fermeture des fichiers
  fclose(resFile);
  return 0;
}

