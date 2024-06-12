#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "rubrica.h"

void addContatto(Rubrica *rubrica, Contatto *contatto) {
    if(rubrica->totContatti >= 100) {
        printf("La Rubrica è piena, non è possibile aggiungere altri contatti.\n");
        return;
    }

    rubrica->contatti[rubrica->totContatti] = *contatto;
    rubrica->contatti[rubrica->totContatti];
    rubrica->totContatti++;
}

// Nome e cognome per identificare il numero, maledetto chi ha persone omonime in entrambi i campi nella propria rubrica
Contatto getContatto(Rubrica *rubrica, char* lastname, char* firstname) {
    int length = sizeof(rubrica);
    int i = 0;
    while(i < length){
        if(strcmp(rubrica->contatti[i].lastname,lastname) == 0){
            if(strcmp(rubrica->contatti[i].firstname,firstname) == 0){
                return rubrica->contatti[i];
            }
        }
        i++;
    }
    // pensare a cosa ritornare nel caso in cui non trova il contatto
    // ritorno contatto nullo, ci penserà il client a gestirlo
    return setContatto("","","");
}

void printRubrica(Rubrica* rubrica){
    int length = rubrica->totContatti;
    for(int i = 0; i < length; i++) {
        printContatto(rubrica->contatti[i]);
    }
}

void sendRubrica(int clientSocket, Rubrica *rubrica) {
    int totContatti = rubrica->totContatti;
    send(clientSocket, &totContatti, sizeof(totContatti), 0);
    printf("Numero di contatti inviati: %d\n",totContatti); 
    for(int i = 0; i < totContatti; i++) {
        send(clientSocket, &rubrica->contatti[i], sizeof(Rubrica), 0);
    }
    puts("Invio dei contatti avvenuto con successo.");
}
