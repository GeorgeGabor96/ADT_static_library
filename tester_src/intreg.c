#include <stdio.h>
#include <stdlib.h>
#include "intreg.h"


intreg aloca_intreg(int numar)
{
	intreg numar_alocat = (intreg)malloc(sizeof(int));
	if (numar_alocat == NULL)
	{
		return NULL;
	}
	*numar_alocat = numar;

	return numar_alocat;
}

void afisare_intreg(FILE * pfile, const intreg numar)
{
	fprintf(pfile, "VALOAREA: %d", *numar);
}

void dezaloca_intreg(intreg i)
{
	free(i);
}

int compare_intreg(const intreg i1, const intreg i2)
{
	if (*i1 == *i2)
	{
		return 0;
	}
	else if(*i1 < *i2)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

intreg citeste_intreg(FILE * p_in, FILE * p_out)
{
	int valoare;
	int ok = fscanf(p_in, "%d\n", &valoare);
	if (ok == 0)
	{
		fprintf(p_out, "Error: Type mismatch\n");
		return NULL;
	}
	intreg new_intreg = aloca_intreg(valoare);
	if (new_intreg == NULL)
	{
		fprintf(p_out, "Error: Memory allocation failed\n");
	}
	return new_intreg;
}

intreg copy_intreg(const intreg i)
{
	return aloca_intreg(*i);
}

int hash_intreg(const intreg i, int numar_elemente)
{
	if (*i < 0)
	{
		return -*i % numar_elemente;
	}
	return *i % numar_elemente;
}