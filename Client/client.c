#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define PORT 8080
#define MAX_N_CLIENT 5

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
 
int main(int argc, char const* argv[])
{
    int status, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("ERRORE: Creazione socket fallita");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("ERRORE: Indirizzo non supportato");
        return -1;
    }
    
    printf("Rublica Condivisa");

    while(connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        puts("Connessione in corso ...");
        sleep(3);
    }

    int choice;
    do{
        puts("Benvenuto nella Rublica Condivisa");
        puts("Scegliere l'opzione desiderata:");
        puts("1 : Visualizzare i contatti della rubrica");
        puts("2 : Aggiungere un nuovo contatto (Autenticazione richiesta)");
        puts("3 : Modificare un contatto esistente (Autenticazione richiesta)");
        puts("4 : Eliminare un contatto esistente (Autenticazione richiesta)");
        puts("9 : Uscire dal sistema");
        scanf("%d",&choice);
        switch(choice){
            case 1 :
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 9:
                break;
        }
    }while(choice != 9);

    // closing the connected socket
    printf("Grazie per aver utilizzato il nostro servizio!");
    close(client_fd);
    return 0;
}