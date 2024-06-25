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

void debugWithMessageWithoutParameters(int mode, char *msg)
{
    if (mode == 0)
    {
        puts(msg);
    }
}

void elimina_contatto(Rubrica *rubrica, int indice) {
    if (indice >= 0 && indice < rubrica->totContatti) {
        for (int i = indice; i < rubrica->totContatti - 1; i++) {
            rubrica->contatti[i] = rubrica->contatti[i + 1];
        }
        rubrica->totContatti--;
    }
}

int main(int argc, char *argv[])
{
    // INIZIALIZZO LA RUBRICA
    Rubrica rubrica;
    rubrica.totContatti = 0;
    Contatto rossi = setContatto("Rossi", "Luigi", "3335670098");
    Contatto sainz = setContatto("Sainz", "Luca", "3389234123");
    Contatto estathe = setContatto("Estathe", "Leila", "3382987105");
    addContatto(&rubrica, &rossi);
    addContatto(&rubrica, &sainz);
    addContatto(&rubrica, &estathe);
    // INIZIALIZZO UTENTI
    Utenti utenti;
    utenti.totUtenti = 0;
    inizializza(&utenti);
    //printUtenti(&utenti);
    // ISTANZIO VARIABILI
    int sockfd, client_connection, valread;
    socklen_t length;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[1024];
    char command[10] = {0};
    pid_t clientPID;
    // APRO SOCKET TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERRORE: il socket non si è instaurato\n");
    }

    // DEFINISCO L'INDIRIZZO DA ASSEGNARE AL SOCKET
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERRORE: collegamento non avvenuto correttamente!\n");
    }

    // MI METTO IN ASCOLTO
    if (listen(sockfd, MAX_N_CLIENT) == 0)
    {
        puts("In ascolto ...");
    }
    else
    {
        perror("ERRORE: collegamento non avvenuto correttamente!\n");
        exit(1);
    }

    while (1)
    {
        length = sizeof(cli_addr);
        int client_connection = accept(sockfd, (struct sockaddr *)&cli_addr, &length);
        if (client_connection < 0)
        {
            exit(1);
        }
        printf("Connessione accettata dall'indirizzo: %s sulla porta :%d\n", inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);
        if (clientPID = fork() == 0)
        {
            while (read(client_connection, &command, sizeof(command)) > 0)
            {
                switch (atoi(command))
                {
                case 1:
                    send(client_connection, &rubrica, sizeof(Rubrica), 0);
                    memset(command, 0, sizeof(command));
                    break;
                case 2:
                    puts("Scelta effettuata dall'utente : 2");
                    puts("In attesa di autenticazione ...");
                    Utente utente;
                    int conferma;
                    recv(client_connection, &utente, sizeof(Utente), 0);
                    puts("Dati ricevuti con successo");
                    if(conferma = autorizza(&utenti, &utente)){
                        puts("Autenticazione avvenuta con successo");
                        //Esito positivo
                        send(client_connection, "1", sizeof(int),0);
                        send(client_connection, &rubrica.totContatti, sizeof(int),0);
                        if(rubrica.totContatti < MAX_CONTATTO){
                            Contatto newContatto;
                            recv(client_connection, &newContatto, sizeof(Contatto), 0);
                            printContatto(newContatto);
                            addContatto(&rubrica, &newContatto);
                            send(client_connection, "1", sizeof(int),0);
                        }else{
                            send(client_connection, "0", sizeof(int),0);
                        }
                    }else{
                        puts("Autenticazione fallita, segnalo al client l'errore.");
                        send(client_connection, "0", sizeof(int),0);
                        puts("Errore recapitato con successo");
                    }
                    memset(command, 0, sizeof(command));
                    break;
                case 3:
                    char newName[50], newLastName[50], newNumber[50];
                    Contatto *contatto_modificato;
                    int indice;
                    valread = read(client_connection, newName, 50);
                    newName[valread] = '\0';
                    printf("VALREAD: %d\n", valread);
                    printf("newName: %s\n", newName);
                    valread = read(client_connection, newLastName, 50);
                    newLastName[valread] = '\0';
                    printf("VALREAD: %d\n", valread);
                    printf("newLastName: %s\n", newLastName);

                    indice = getContatto(&rubrica, newName, newLastName);
                    // printf("QUINDI: %s",contatto_modificato );

                    // printf("Nome: %s - Cognome: %s\n", contatto_modificato->firstname, contatto_modificato->lastname);

                    if (indice != -1)
                    {
                        // Invia al client un messaggio di conferma
                        int conferma = 1;
                        send(client_connection, &conferma, sizeof(int), 0);

                        memset(newNumber, 0, sizeof(newNumber));
                        // Ricevi il nuovo numero di telefono dal client
                        read(client_connection, newNumber, sizeof(newNumber));
                        newNumber[strcspn(newNumber, "\n")] = 0; // Aggiunge terminatore di stringa

                        strcpy(rubrica.contatti[indice].cell_number, newNumber);
                        printf("Numero di telefono del contatto modificato.\n");

                        printf("Contatto modificato:\n");
                        printf("Nome: %s\n", rubrica.contatti[indice].firstname);
                        printf("Cognome: %s\n", rubrica.contatti[indice].lastname);
                        printf("Numero di telefono: %s\n", rubrica.contatti[indice].cell_number);
                        printf("\n");
                    }
                    else
                    {
                        // Se il contatto non viene trovato, invia un messaggio di errore al client
                        char errore[] = "Contatto non trovato";
                        send(client_connection, errore, sizeof(char), 0);
                    }
                    break;
                case 4:
                    char nome_elim[50], cognome_elim[50];
                    int index;
                    memset(nome_elim, 0, sizeof(nome_elim));
                    memset(cognome_elim, 0, sizeof(cognome_elim));

                    read(client_connection, nome_elim, sizeof(nome_elim));
                    nome_elim[strcspn(nome_elim, "\n")] = '\0';       // Rimuove il carattere newline

                    read(client_connection, cognome_elim, sizeof(cognome_elim));
                    cognome_elim[strcspn(cognome_elim, "\n")] = '\0'; // Rimuove il carattere newline

                    printf("Richiesta di eliminazione per il contatto: %s %s\n", nome_elim, cognome_elim);

                    index = getContatto(&rubrica, nome_elim, cognome_elim);

                    if (index != -1) {
                        // Contatto trovato, elimina il contatto
                        elimina_contatto(&rubrica, indice);
                        printf("Contatto eliminato: %s %s\n", nome_elim, cognome_elim);

                        // Invia conferma al client
                        char conferma[] = "Contatto eliminato con successo.";
                        send(client_connection, conferma, strlen(conferma), 0);
                    } else {
                        // Contatto non trovato
                        char conferma[] = "Contatto non trovato.";
                        send(client_connection, conferma, strlen(conferma), 0);
                    }
                    break;
                case 9:
                    printf("In attesa di input da ... %s:%d\n", inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);
                    break;
                }
            }
        }
    }
    return 0;
}