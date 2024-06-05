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

#define PORT 8080
#define MAX_N_CLIENT 2


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd;
    int client_sockets[MAX_N_CLIENT];
    socklen_t socket_lenght;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    char command[10] = {0};

        // APRO SOCKET TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERRORE: il socket non si è instaurato\n");
    }

    // DEFINISCO L'INDIRIZZO DA ASSEGNARE AL SOCKET
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERRORE: l'indirizzo non è stato assegnato correttamente\n");
    }

    if (listen(sockfd, MAX_N_CLIENT) < 0) {
        perror("Errore in listen\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    // MI METTO IN ASCOLTO
    while(1){

        socket_lenght = sizeof(cli_addr);
        int socket_counter = 0;
        pid_t pid;
        pid = fork();
        if(pid == 0){
            printf("Processo Figlio pre connessione : %d\n",getpid());
            int client_connection = accept(sockfd, (struct sockaddr *) &cli_addr, &socket_lenght);
            printf("Client socket : %d\n",client_connection);
            printf("Processo Figlio post connessione : %d\n",getpid());
            if (client_connection < 0) {
                error("ERRORE: accettazione fallita\n");
                bzero(buffer,256);
            }
            client_sockets[socket_counter] = client_connection;
            printf("Invio socket counter : %d\n",socket_counter);
            if(send(sockfd,&socket_counter,sizeof(socket_counter),0) > 0){
                printf("Inviato\n");
                socket_counter++;
            }else{
                printf("Maremma maiala\n");
            }
        }else if(pid > 0){
            printf("Processo Padre : %d\n",getpid());
        }
        sleep(1);
    }

    return 0; 
}