#ifndef STUDENT
#define STUDENT

typedef struct stud_struct * persoana;

persoana aloca_persoana(char *, char *, int);

void afisare_persoana(FILE *, const persoana);

void dezaloca_persoana(persoana);

int compare_persoana(const persoana, const persoana);

persoana citeste_persoana(FILE * p_in, FILE * p_out);

persoana copy_persoana(const persoana );

int hash_persoana(const persoana pers, int dimensiune);

#endif