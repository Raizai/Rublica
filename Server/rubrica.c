#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "contatto.c"
int lenght = 0;

void stampaRubrica(Record records[]){
    int index = lenght;
    while(index-- > 0){
        printf(records[index].lastname);
        printf("\t");
        printf(records[index].firstname);
        printf("\t");
        puts(records[index].cell_number);
    }
}