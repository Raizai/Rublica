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

void receiveContacts(int client_socket, Rubrica *rubrica) {
    recv(client_socket, &rubrica->totContatti, sizeof(Rubrica), 0);

    for(int i = 0; i < rubrica->totContatti; i++) {
        recv(client_socket, &rubrica->totContatti, sizeof(Rubrica), 0);
    }
}

void riceviRubrica(int clientSocket, Rubrica *rubrica) {
    // Receive the total number of contacts
    int totContatti;
    if (recv(clientSocket, &totContatti, sizeof(totContatti), 0) <= 0) {
        perror("Error receiving total contacts");
        return;
    }
    rubrica->totContatti = totContatti;
    printf("TOTCONTATTO: %d\n", totContatti);

    // Receive each contact individually
    for (int i = 0; i < totContatti; i++) {
        if (recv(clientSocket, &rubrica->contatti[i], sizeof(Contatto), 0) <= 0) {
            perror("Error receiving contact");
            return;
        }
        printf("Received contact %d: %s, %s\n", i, rubrica->contatti[i].firstname, rubrica->contatti[i].cell_number);
    }
}

void inviaNuovoContatto(int clientSocket, Contatto *contatto) {
    send(clientSocket, contatto, sizeof(Contatto), 0);
}

int main(int argc, char const *argv[])
{
    int status, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    Rubrica rubrica;

    int num_contact;

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

        printf("Messaggio inviato al Server\n");

        switch (choice) {
        case 1:
            puts("Elenco della Rubrica: \n");
            write(client_fd, "1", 1);
            printf("Messaggio inviato al Server\n");

            riceviRubrica(client_fd, &rubrica);
            printf("Ricevuto \n");
            
            printf("TOT: %d\n", rubrica.totContatti);
            
            for(int i = 0; i < rubrica.totContatti; i++) {
                printf("Contatto %d\n", i+1);
                printf("Name: %s\n", rubrica.contatti[i].firstname);
                printf("Cognome: %s\n", rubrica.contatti[i].lastname);
                printf("Number: %s\n", rubrica.contatti[i].cell_number);
            }

            // printf("qui\n");

            break;
        case 2:
            Contatto nuovoContatto;
            write(client_fd, "2", 1);
            puts("Nome: ");
            scanf("%s",nuovoContatto.firstname);
            puts("Cognome: ");
            scanf("%s",nuovoContatto.lastname);
            puts("Cellulare: ");
            scanf("%s",nuovoContatto.cell_number);
            //write(client_fd, "2", 1);
            inviaNuovoContatto(client_fd, &nuovoContatto);
            printContatto(nuovoContatto);
            puts("INVIATO CONTATTO");
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