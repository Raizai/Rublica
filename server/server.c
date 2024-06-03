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

#include "../contatto/contatto.h"

#define PORT 8080
#define MAX_N_CLIENT 1


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void riceviNuovoContatto(int clientSocket, Contatto *contatto) {
    recv(clientSocket, contatto, sizeof(Contatto), 0);
}

int main(int argc, char *argv[]) {
    int sockfd;
    int client_sockets[MAX_N_CLIENT];
    socklen_t socket_lenght;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;

    char command[10] = {0};

    Rubrica rubrica;
    rubrica.totContatti = 0;
    Contatto rossi = setContatto("Rossi", "Luigi", "3335670098");
    Contatto sainz = setContatto("Sainz", "Luca", "3389234123");
    Contatto estathe = setContatto("Estathe", "Leila", "3382987105");
    addContatto(&rubrica, &rossi);
    addContatto(&rubrica, &sainz);
    addContatto(&rubrica, &estathe);

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
        if(fork() == 0){
            client_sockets[socket_counter] = accept(sockfd, (struct sockaddr *) &cli_addr, &socket_lenght);
            if (client_sockets[socket_counter] < 0) {
                error("ERRORE: accettazione fallita\n");
                bzero(buffer,256);
            }
            printf("Qui\n");
            while (read(client_sockets[socket_counter], command, sizeof(command)) > 0) {
                printf("Letto \n");
                printf("%s\n", command);
                switch (atoi(command)) {
                case 1:
                    printf("true\n");
                    inviaRubrica(client_sockets[socket_counter], &rubrica);
                    printf("inviato\n");
                    break;
                case 2:
                    Contatto newContatto;
                    riceviNuovoContatto(client_sockets[socket_counter], &newContatto);
                    printContatto(newContatto);
                    addContatto(&rubrica, &newContatto);
                    printf("Contatto Aggiunto\n");
                    break;
                default:
                    break;
                }
            }
        }
        sleep(1);
    }

    return 0; 
}