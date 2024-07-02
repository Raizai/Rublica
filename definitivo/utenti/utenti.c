#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "utenti.h"

void inizializza(Utenti* utenti) {
    strcpy(utenti->utenti[0].username,"admin");
    strcpy(utenti->utenti[0].password,"1234");
    utenti->totUtenti++;
}

int autorizza(Utenti* lista, Utente* utente){
    // 0 : miss - 1 : hit
    int i;
    for(i = 0;i < lista->totUtenti;i++){
        if(strcmp(lista->utenti[i].username, utente->username) == 0){
            if(strcmp(lista->utenti[i].password, utente->password) == 0){
                return 1;
            }
        }
    }
    return 0;
}

void printUtenti(Utenti* utenti){
    printf("Lista utenti:\n");
    for(int i = 0; i < utenti->totUtenti; i++) {
        printf("Nome Utente: %s\n", utenti->utenti[i].username);
        puts("Parola Segreta: omessa");
    }
}

