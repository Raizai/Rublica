#include "../contatto/contatto.h"

#define MAX_CONTATTO 100

typedef struct {
    Contatto contatti[MAX_CONTATTO];
    int totContatti;
} Rubrica;

void addContatto(Rubrica *rubrica, Contatto *contatto);

int getContatto(Rubrica *rubrica, char* lastname, char* firstname);

void printRubrica(Rubrica *rubrica);

void sendRubrica(int clientSocket, Rubrica *rubrica);