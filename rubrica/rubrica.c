#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "rubrica.h"

void addContatto(Rubrica *rubrica, Contatto *contatto) {
    rubrica->contatti[rubrica->totContatti] = *contatto;
    rubrica->contatti[rubrica->totContatti];
    rubrica->totContatti++;
}

int getContatto(Rubrica *rubrica, char nome[], char cognome[]) {
    for (int i = 0; i < rubrica->totContatti; i++) {
        if (strcmp(rubrica->contatti[i].firstname, nome) == 0 && strcmp(rubrica->contatti[i].lastname, cognome) == 0) {
            return i;  //se il contatto viene trovato viene ritornato il suo indice
        }
    }
    return -1; // Se il contatto non viene trovato
}

void printRubrica(Rubrica* rubrica){
    for(int i = 0; i < rubrica->totContatti; i++) {
        printContatto(rubrica->contatti[i]);
    }
}
