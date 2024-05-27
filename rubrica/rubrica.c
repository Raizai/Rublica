#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "rubrica.h"

int lenght = 0;

void addContatto(Rubrica *rubrica, Contatto *contatto) {
    if(rubrica->totContatti >= 100) {
        printf("La Rubrica è piena, non è possibile aggiungere altri contatti.\n");
        return;
    }

    rubrica->contatti[rubrica->totContatti] = *contatto;
    rubrica->contatti[rubrica->totContatti];
    rubrica->totContatti++;
}
