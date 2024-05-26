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

void addContatto(Rubrica *rubrica, Contatto *contatto);
