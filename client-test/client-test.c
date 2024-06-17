#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "../rubrica/rubrica.h"
#define PORT 8080

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void receiveRubrica(int clientSocket, Rubrica *rubrica) {
    int length;
    if (recv(clientSocket, &length, sizeof(length), 0) <= 0) {
        perror("ERRORE : il totale dei contatti non è stato ricevuto correttamente.");
        return;
    }

    for (int i = 0; i < length; i++) {
        if (recv(clientSocket, &rubrica->contatti[i], sizeof(Contatto), 0) <= 0) {
            perror("ERRORE : Uno dei contatti non è stato ricevuto correttamente.");
            return;
        }
    }
    puts("La rubrica è stata ricevuta correttamente.");
}

int main(int argc, char const *argv[])
{
    int status, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    int num_contact;
    Rubrica rubrica;

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

    printf("Rubrica Condivisa\n");

    while (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        puts("Connessione in corso ...");
        sleep(3);
    }

    int choice;
    do {
        puts("\nBenvenuto nella Rubrica Condivisa");
        puts("Scegliere l'opzione desiderata:");
        puts("1 : Visualizzare i contatti della rubrica");
        puts("2 : Aggiungere un nuovo contatto (Autenticazione richiesta)");
        puts("3 : Modificare un contatto esistente (Autenticazione richiesta)");
        puts("4 : Eliminare un contatto esistente (Autenticazione richiesta)");
        puts("9 : Uscire dal sistema");
        scanf("%d", &choice);
        printf("CHOICCEEE: %d", choice);
        switch (choice) {
        case 1:
            write(client_fd, "1", 1);
            printf("Richiesta inviata al server : %d\n",choice);
            read(client_fd, &rubrica, sizeof( Rubrica));
            printRubrica(&rubrica);
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 9:
            break;
        default:
            printf("Operazione non consentita");
        }
    } while (choice != 9);

    // closing the connected socket
    printf("Grazie per aver utilizzato il nostro servizio!");
    close(client_fd);
    return 0;
}