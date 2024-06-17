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

#define PORT 8080
#define MAX_N_CLIENT 2

void debugWithMessageWithoutParameters(int mode, char * msg){
    if(mode == 0){
        puts(msg);
    }
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
    int sockfd, client_connection;
    socklen_t length;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[1024];
    int command[10] = {0};
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
                            printf("Eseguo operazione %d richiesta da %s:%d\n",command,inet_ntoa(cli_addr.sin_addr),cli_addr.sin_port);
                            send(client_connection, &rubrica, sizeof(Rubrica), 0);
                            memset(command, 0, sizeof(command));
                            break;
                        case 2:
                            Contatto newContatto;
                            recv(client_connection, &newContatto, sizeof(Contatto), 0);
                            printContatto(newContatto);
                            addContatto(&rubrica, &newContatto);
                            printf("Contatto Aggiunto\n");

                            memset(command, 0, sizeof(command));
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