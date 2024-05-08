#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#define PORT 8080
#define MAX_N_CLIENT 5


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd;
     int client_sockets[MAX_N_CLIENT];
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

    // APRO SOCKET TCP
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERRORE: il socket non si è instaurato"); 

    // DEFINISCO L'INDIRIZZO DA ASSEGNARE AL SOCKET
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(PORT);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              error("ERRORE: l'indirizzo non è stato assegnato correttamente");
    
    // MI METTO IN ASCOLTO
    while(1){
        listen(sockfd,MAX_N_CLIENT);
        clilen = sizeof(cli_addr);
        int socket_counter = 0;
        if(fork() == 0){
            client_sockets[socket_counter++] = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, 
                    &clilen);
            if (client_sockets[socket_counter] < 0) {
                error("ERRORE: accettazione fallita");
                bzero(buffer,256);
            }
        }
        sleep(1);
    }

    return 0; 
}