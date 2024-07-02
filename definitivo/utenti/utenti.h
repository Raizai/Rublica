#include "utente.h"

#define MAX_UTENTI 100

typedef struct {
    Utente utenti[MAX_UTENTI];
    int totUtenti;
} Utenti;

void inizializza(Utenti* utenti);
int autorizza(Utenti* lista, Utente *utente);
void printUtenti(Utenti* utenti);