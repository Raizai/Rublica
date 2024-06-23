#include "utente.h"

#define MAX_UTENTI 100

typedef struct {
    Utente utenti[MAX_UTENTI];
    int totUtenti;
} Utenti;

Utenti inizializza();
int autorizza(Utenti* lista, Utente *utente);