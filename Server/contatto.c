#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char lastname[50];
    char firstname[50];
    char cell_number[50];
} Record;

Record setContatto(char* lastname, char* firstname, char* cell_number){
    Record contatto;
    strcpy(contatto.lastname,lastname);
    strcpy(contatto.firstname,firstname);
    strcpy(contatto.cell_number,cell_number);
    return contatto;
}

Record getRecord(Record records[],char * lastname, char * firstname){
    int length = sizeof(records);
    int i = 0;
    while(i < length){
        if(strcmp(records[i].lastname,lastname) == 0){
            if(strcmp(records[i].lastname,firstname) == 0){
                return records[i];
            }
        }
        i++;
    }
}
