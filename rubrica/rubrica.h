#include "../contatto/contatto.h"

#define MAX_CONTATTO 100

typedef struct {
    Contatto contatti[MAX_CONTATTO];
    int totContatti;
} Rubrica;

int length(Rubrica rubrica[]);

void addContatto(Rubrica *rubrica, Contatto *contatto);

void stampaRubrica(Rubrica rubrica[]);

