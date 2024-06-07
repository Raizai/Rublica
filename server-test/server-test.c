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

#define PORT 8080
#define MAX_N_CLIENT 2

void debugWithMessageWithoutParameters(int mode, char * msg){
    if(mode == 0){
        puts(msg);
    }
}

int main(int argc, char *argv[]) {
    // ISTANZIO VARIABILI
    int sockfd, client_connection;
    socklen_t length;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[1024];
    int command;
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
        if(client_connection < 0) exit(1);
        printf("Connessione accettata dall'indirizzo: %s sulla porta :%d\n",inet_ntoa(cli_addr.sin_addr),cli_addr.sin_port);
        if(clientPID = fork() == 0){
            debugWithMessageWithoutParameters(0,"DEBUG : fork eseguito\n");
            do{
                if(read(client_connection, &command, sizeof(command)) > 0) {
                    switch(command){
                        case 1:
                            printf("Eseguo operazione %d richiesta da %s:%d\n",command,inet_ntoa(cli_addr.sin_addr),cli_addr.sin_port);
                            break;
                        case 9:
                            printf("In attesa di input da ... %s:%d\n",inet_ntoa(cli_addr.sin_addr),cli_addr.sin_port);
                            break;
                    }
                }
            }while(command != 9);
        }
    }
    return 0; 
}