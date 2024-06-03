#ifndef CONTATTO_H
#define CONTATTO_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#define MAX_CONTATTO 100

typedef struct {
    char lastname[50];
    char firstname[50];
    char cell_number[50];
} Contatto;

typedef struct {
    Contatto contatti[MAX_CONTATTO];
    int totContatti;
} Rubrica;

Contatto setContatto(char* lastname, char* firstname, char* cell_number);

void addContatto(Rubrica *rubrica, const Contatto *contatto);

void inviaRubrica(int clientSocket, Rubrica *rubrica);

Contatto getContatto(Rubrica *rubrica, char* lastname, char* firstname);

void printContatto(Contatto contatto);

#endif //CONTATTO_H