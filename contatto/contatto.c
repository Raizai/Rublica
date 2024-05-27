#include "contatto.h"

Contatto setContatto(char* lastname, char* firstname, char* cell_number) {
    Contatto contatto;
    strcpy(contatto.lastname,lastname);
    strcpy(contatto.firstname,firstname);
    strcpy(contatto.cell_number,cell_number);
    return contatto;
}

int lenght = 0;

void addContatto(Rubrica *rubrica, const Contatto *contatto) {
    if(rubrica->totContatti >= 100) {
        printf("La Rubrica è piena, non è possibile aggiungere altri contatti.\n");
        return;
    }

    rubrica->contatti[rubrica->totContatti] = *contatto;
    rubrica->contatti[rubrica->totContatti];
    rubrica->totContatti++;
}

void inviaRubrica(int clientSocket, Rubrica *rubrica) {
    int totContatti = rubrica->totContatti;
    send(clientSocket, &totContatti, sizeof(totContatti), 0);
    printf("TOTCONTATTO: %d\n",totContatti); 
    for(int i = 0; i < totContatti; i++) {
        send(clientSocket, &rubrica->contatti[i], sizeof(Rubrica), 0);
    }
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
