#ifndef INT
#define INT

typedef int * intreg;

intreg aloca_intreg(int);

void afisare_intreg(FILE *, intreg);

int compare_intreg(intreg, intreg);

void dezaloca_intreg(intreg);

intreg copy_intreg(intreg);

intreg citeste_intreg(FILE * p_in, FILE * p_out);

int hash_intreg(intreg i, int numar_elemente);

#endif