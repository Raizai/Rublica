#include "contatto.h"

Contatto setContatto(char* lastname, char* firstname, char* cell_number) {
    Contatto contatto;
    strcpy(contatto.lastname,lastname);
    strcpy(contatto.firstname,firstname);
    strcpy(contatto.cell_number,cell_number);
    return contatto;
}

void printContatto(Contatto contatto) {
    printf("Name: %s\n", contatto.firstname);
    printf("Cognome: %s\n", contatto.lastname);
    printf("Number: %s\n", contatto.cell_number);
    puts("-----------------");
}
