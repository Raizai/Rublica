#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "contatto.h"

Contatto setContatto(char* lastname, char* firstname, char* cell_number) {
    Contatto contatto;
    strcpy(contatto.lastname,lastname);
    strcpy(contatto.firstname,firstname);
    strcpy(contatto.cell_number,cell_number);
    return contatto;
}

int lenght = 0;

void addContatto(Rubrica *rubrica, Contatto *contatto) {
    if(rubrica->totContatti >= 100) {
        printf("La Rubrica è piena, non è possibile aggiungere altri contatti.\n");
        return;
    }

    rubrica->contatti[rubrica->totContatti] = *contatto;
    rubrica->contatti[rubrica->totContatti];
    rubrica->totContatti++;
}

// Contatto getRecord(Contatto records[],char +number) {
//     int length = sizeof(records);
//     int i = 0;
//     while(i < length){
//         if(strcmp(records[i].lastname,lastname) == 0){
//             if(strcmp(records[i].lastname,firstname) == 0){
//                 return records[i];
//             }
//         }
//         i++;
//     }
// }
