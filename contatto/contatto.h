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

Contatto setContatto(char* lastname, char* firstname, char* cell_number);

void printContatto(Contatto contatto);

#endif //CONTATTO_H