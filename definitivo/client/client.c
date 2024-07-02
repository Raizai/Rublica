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
#include "../utenti/utente.h"
#define PORT 8080

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

int isNumero(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

void checkNumero(char *number) {
    do {
        puts("Cellulare: ");
        scanf("%s", number);

        if (!isNumero(number)) {
            printf("Numero non valido. Inserisci di nuovo il numero\n");
        }
    } while (!isNumero(number));
}

int inoltroCredenziali(int client_fd){
    Utente utente;
    int conferma = 0;
    puts("Autenticazione richiesta");
    printf("Nome utente: ");
    scanf("%s", utente.username);
    printf("Password: ");
    scanf("%s", utente.password);
    send(client_fd, &utente, sizeof(Utente), 0);
    
    recv(client_fd, &conferma, sizeof(int),0);
    printf("CONFERMA: %d\n", conferma);
    return conferma;
}

int main()
{
    int client_fd, response;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    Rubrica rubrica;
    int conferma;

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

    puts("\nBenvenuto nella sua Rubrica Online");

    int choice;
    do {
        puts("");
        puts("Scegliere l'opzione desiderata:");
        puts("1 : Visualizzare i contatti della rubrica");
        puts("2 : Aggiungere un nuovo contatto (Autenticazione richiesta)");
        puts("3 : Modificare un contatto esistente (Autenticazione richiesta)");
        puts("4 : Eliminare un contatto esistente (Autenticazione richiesta)");
        puts("9 : Uscire dal sistema");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            write(client_fd, "1", 1);
            puts("");
            recv(client_fd, &rubrica, sizeof( Rubrica),0);
            printRubrica(&rubrica);
            break;
        case 2: 
            write(client_fd, "2", 1);
            conferma = inoltroCredenziali(client_fd);
            if(conferma){
                Contatto nuovoContatto;
                int totContatti;
                recv(client_fd, &totContatti, sizeof(int),0);
                if (totContatti < MAX_CONTATTO) {
                    puts("Nome: ");
                    scanf("%s", nuovoContatto.firstname);
                    puts("Cognome: ");
                    scanf("%s", nuovoContatto.lastname);
                    checkNumero(nuovoContatto.cell_number);
                    send(client_fd, &nuovoContatto, sizeof(Contatto), 0);
                }
                recv(client_fd, &conferma, sizeof(int),0);
                if(conferma){
                    printf("Elemento inserito in rubrica correttamente.\n");
                }else{
                    puts("La tua Rubrica è piena,non è possibile aggiungere altri contatti.");
                    printf("Operazione fallita, si prega di ritentare.\n");
                }
            }else{
                printf("Autenticazione fallita, la tua Rubrica è piena,non è possibile aggiungere altri contatti.\n");
            }
            break;
        case 3:
            write(client_fd, "3", 1);
            conferma = inoltroCredenziali(client_fd);
            if(conferma){
                char newName[50], newLastName[50];
                Contatto contatto_modificato;
                puts("Inserisci il nome:");
                scanf("%s", newName);
                send(client_fd, newName, strlen(newName), 0);
                puts("Inserisci il cognome:");
                scanf("%s", newLastName);
                send(client_fd, newLastName, strlen(newLastName), 0);
                recv(client_fd, &response, sizeof(int),0);
                if (response == 1) {
                    do {
                    printf("Inserisci il nuovo numero di telefono per il contatto: ");
                    scanf("%s", contatto_modificato.cell_number);
                        if (!isNumero(contatto_modificato.cell_number)) {
                            printf("Numero non valido. Inserisci di nuovo il numero\n");
                        }
                    } while (!isNumero(contatto_modificato.cell_number));
                    // Invia il nuovo numero al server
                    send(client_fd, contatto_modificato.cell_number, strlen(contatto_modificato.cell_number), 0);
                    printf("Numero modificato inviato al server.\n");
                } else {
                    printf("Contatto non esiste nella Rubrica.\n");
                }
            }else{
                printf("Autenticazione fallita, si prega di ritentare.\n");
            }
            break;
        case 4:
            write(client_fd, "4", 1);
            conferma = inoltroCredenziali(client_fd);
            if(conferma){
                char nome_elim[50], cognome_elim[50]; 
                send(client_fd, buffer, strlen(buffer), 0);

                printf("Inserisci il nome del contatto da eliminare: ");
                scanf("%s", nome_elim);
                send(client_fd, nome_elim, strlen(nome_elim), 0);

                printf("Inserisci il cognome del contatto da eliminare: ");
                scanf("%s", cognome_elim);
                send(client_fd, cognome_elim, strlen(cognome_elim), 0);

                char response[1024] = {0};
                recv(client_fd, response, 1024, 0);
                printf("%s\n", response);
            }else{
                printf("Autenticazione fallita, si prega di ritentare.\n");
            }
            break;
        case 9:
            write(client_fd, "9", 1);
            close(client_fd);
            break;
        default:
            printf("Operazione non consentita");
        }
    } while (choice != 9);

    // closing the connected socket
    printf("Grazie per aver utilizzato il nostro servizio!\n");

    return 0;
}