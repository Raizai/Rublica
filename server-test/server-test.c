#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include "../rubrica/rubrica.h"
#include "../utenti/utenti.h"

#define PORT 8080
#define MAX_N_CLIENT 2

void debugWithMessageWithoutParameters(int mode, char * msg){
    if(mode == 0){
        puts(msg);
    }
}

Contatto* findContatto(Rubrica *rubrica, char nome[], char cognome[]) {
    for (int i = 0; i < rubrica->totContatti; i++) {
        if (strcmp(rubrica->contatti[i].firstname, nome) == 0 && strcmp(rubrica->contatti[i].lastname, cognome) == 0) {
            puts("Trovato");
            return &rubrica->contatti[i];
        }
    }
    return NULL; // Se il contatto non viene trovato
}

int main(int argc, char *argv[]) {
    // INIZIALIZZO LA RUBRICA
    Rubrica rubrica;
    rubrica.totContatti = 0;
    Contatto rossi = setContatto("Rossi", "Luigi", "3335670098");
    Contatto sainz = setContatto("Sainz", "Luca", "3389234123");
    Contatto estathe = setContatto("Estathe", "Leila", "3382987105");
    addContatto(&rubrica, &rossi);
    addContatto(&rubrica, &sainz);
    addContatto(&rubrica, &estathe);
    // ISTANZIO VARIABILI
    int sockfd, client_connection, valread;
    socklen_t length;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[1024];
    char command[10] = {0};
    pid_t clientPID;
    // APRO SOCKET TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERRORE: il socket non si è instaurato\n");
    }

    // DEFINISCO L'INDIRIZZO DA ASSEGNARE AL SOCKET
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERRORE: collegamento non avvenuto correttamente!\n");
    }

    // MI METTO IN ASCOLTO
    if (listen(sockfd, MAX_N_CLIENT) == 0) {
        puts("In ascolto ...");
    }else{
        perror("ERRORE: collegamento non avvenuto correttamente!\n");
        exit(1);
    }

    while(1){
        length = sizeof(cli_addr);
        int client_connection = accept(sockfd, (struct sockaddr *) &cli_addr, &length);
        if(client_connection < 0) {
            exit(1);
        }
        printf("Connessione accettata dall'indirizzo: %s sulla porta :%d\n",inet_ntoa(cli_addr.sin_addr),cli_addr.sin_port);
        if(clientPID = fork() == 0){
                while(read(client_connection, &command, sizeof(command)) > 0) {
                    switch(atoi(command)){
                        case 1:
                            send(client_connection, &rubrica, sizeof(Rubrica), 0);
                            memset(command, 0, sizeof(command));
                            break;
                        case 2:
                            Utente utente;
                            //recv(client_connection, &utente, sizeof(Utente), 0);

                            int totContatti = rubrica.totContatti;
                            send(client_connection, &totContatti, sizeof(int), 0);

                            Contatto newContatto;
                            recv(client_connection, &newContatto, sizeof(Contatto), 0);
                            printContatto(newContatto);
                            addContatto(&rubrica, &newContatto);

                            memset(command, 0, sizeof(command));
                            break;
                        case 3:
                            char newName[50], newLastName[50];
                            Contatto *contatto_modificato;
                            valread = read(client_connection, newName, 50);
                            newName[valread] = '\0';
                            printf("VALREAD: %d\n", valread);
                            printf("newName: %s\n", newName);
                            valread = read(client_connection, newLastName, 50);
                            newLastName[valread] = '\0';
                            printf("VALREAD: %d\n", valread);
                            printf("newLastName: %s\n", newLastName);

                            contatto_modificato = findContatto(&rubrica, newName, newLastName);

                            printf("Nome: %s - Cognome: %s\n", contatto_modificato->firstname, contatto_modificato->lastname);

                            if (contatto_modificato != NULL) {
                                // Invia al client un messaggio di conferma
                                int conferma = 1;
                                send(client_connection, &conferma, sizeof(int), 0);

                                // Ricevi il nuovo numero di telefono dal client
                                valread = read(client_connection, contatto_modificato->cell_number, 20);
                                contatto_modificato->cell_number[valread] = '\0'; // Aggiunge terminatore di stringa

                                printf("Contatto modificato:\n");
                                printf("Nome: %s\n", contatto_modificato->lastname);
                                printf("Cognome: %s\n", contatto_modificato->lastname);
                                printf("Numero di telefono: %s\n", contatto_modificato->cell_number);
                                printf("\n");
                            } else{
                                // Se il contatto non viene trovato, invia un messaggio di errore al client
                                char errore[] = "Contatto non trovato";
                                send(client_connection, errore, sizeof(char), 0);
                            }
                            break;
                        case 9:
                            printf("In attesa di input da ... %s:%d\n",inet_ntoa(cli_addr.sin_addr),cli_addr.sin_port);
                            break;
                    }
                }
        }
    }
    return 0; 
}