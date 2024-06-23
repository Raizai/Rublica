#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "utenti.h"

Utenti inizializza() {
    Utenti utenti;
    Utente utente;
    strcpy(utente.username,"admin");
    strcpy(utente.password,"1234");
    utenti.totUtenti++;
}

int autorizza(Utenti* lista, Utente* utente){
    // 0 : hit - 1 : miss
    int i;
    for(i = 0;i<lista->totUtenti;i++){
        if(strcmp(lista->utenti[i].username, utente->username) == 0){
            if(strcmp(lista->utenti[i].password, utente->password) == 0){
                return 0;
            }
        }
    }
    return 1;
}

