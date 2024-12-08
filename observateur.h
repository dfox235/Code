typedef enum {Init, Mes, Puit}Etat;

typedef enum {M, A, G, D}Alphabet;

Etat etat_initale();

Etat transition(Etat e, Alphabet a);

int est_accepteur(Etat e);
