#include <stdio.h>
#include <string.h>
#include "Persoana.h"

static int baza = 33;
static int multiplier = 2;

struct stud_struct
{
	char nume[20];
	char prenume[20];
	int varsta;
};

static int hash_function(char * sir, int dimensiune)
{
	int index_hash = 0;

	unsigned char * c = sir;

	while (*c != '\0')
	{
		index_hash = index_hash * multiplier + *c++;
	}

	return (index_hash % baza) % dimensiune;
}

persoana aloca_persoana(char * nume, char * prenume, int varsta)
{
	if (varsta < 0)
	{
		return NULL;
	}
	persoana stud = (persoana)malloc(sizeof(struct stud_struct));
	if (stud == NULL)
	{
		return NULL;
	}
	int lungime_nume = strlen(nume) + 1;
	if (lungime_nume > sizeof(stud->nume))
	{
		free(stud);
		return NULL;
	}
	memcpy(stud->nume, nume, lungime_nume);

	int lungime_prenume = strlen(prenume) + 1;
	if (lungime_prenume > sizeof(stud->prenume))
	{
		free(stud);
		return NULL;
	}
	memcpy(stud->prenume, prenume, lungime_prenume);

	stud->varsta = varsta;

	return stud;
}

void afisare_persoana(FILE * pfile, const persoana pers)
{
	fprintf(pfile, "NUME: %s, PRENUME: %s, VARSTA: %d", pers->nume, pers->prenume, pers->varsta);
}

void dezaloca_persoana(persoana pers)
{
	free(pers);
}

int compare_persoana(const persoana pers1, const persoana pers2)
{
	int ok = strcmp(pers1->nume, pers2->nume);
	if (ok > 0)
	{
		return 1;
	}
	else if (ok < 0)
	{
		return -1;
	}
	else
	{
		ok = strcmp(pers1->prenume, pers2->prenume);
		if (ok > 0)
		{
			return 1;
		}
		else if (ok < -1)
		{
			return -1;
		}
		else
		{
			ok = pers1->varsta - pers2->varsta;
			if (ok > 0)
			{
				return 1;
			}
			else if (ok < 0)
			{
				return -1;
			}
			else
			{ 
				return 0;
			}
		}
	}
}

persoana citeste_persoana(FILE * p_in, FILE * p_out)
{
	char linie[100];
	fgets(linie, 100, p_in);
	int ok = 1;
	char * nume = strtok(linie, ";");
	if (nume == NULL)
	{
		ok = 0;
	}
	char * prenume = strtok(NULL, ";");
	if (prenume == NULL)
	{
		ok = 0;
	}
	char * field = strtok(NULL, ";");
	if (field == NULL || field[0] == '\n')
	{
		ok = 0;
	}
	if (ok == 0)
	{
		fprintf(p_out, "Error: Missing value\n");
		return NULL;
	}

	int varsta;
	ok = sscanf(field, "%d ", &varsta);
	if (ok == 0)
	{
		fprintf(p_out, "Error: Type mismatch\n");
		return NULL;
	}

	persoana new_persoana = aloca_persoana(nume, prenume, varsta);
	if (new_persoana == NULL)
	{
		fprintf(p_out, "Error: Memory allocation failed\n");
	}

	return new_persoana;
}

persoana copy_persoana(const persoana p)
{
	return aloca_persoana(p->nume, p->prenume, p->varsta);
}

int hash_persoana(const persoana pers, int dimensiune)
{
	int hash = hash_function(pers->nume, dimensiune);
	hash += hash_function(pers->prenume, dimensiune);
	hash += pers->varsta;

	return hash % dimensiune;
}
