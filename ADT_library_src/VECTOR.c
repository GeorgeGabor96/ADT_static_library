#include <stdio.h>
#include <stdlib.h>
#include "VECTOR.h"

struct VECTOR_GEN
{
	void ** content;
	int numar_elemente;
	int numar_maxim_elemente;
};

Vector CreateVector()
{
	Vector new_vector = (Vector)malloc(sizeof(struct VECTOR_GEN));
	if (new_vector == NULL)
	{
		return NULL;
	}

	new_vector->numar_elemente = 0;
	new_vector->numar_maxim_elemente = 5;

	new_vector->content = (void **)malloc(new_vector->numar_maxim_elemente * sizeof(void *));
	if (new_vector->content == NULL)
	{
		free(new_vector);
		return NULL;
	}
	
	return new_vector;
}


int PrintVector(Vector table, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (table->numar_elemente == 0)
	{
		return 0;
	}
	for (int i = 0; i < table->numar_elemente; i++)
	{
		afisare_element(pfile, table->content[i]);
		fprintf(pfile, "\n");
	}
	return 1;
}


static inline int redimensionare_vector(Vector table, int numar_maxim_nou_elemente)
{
	void ** dummy = (void **)realloc(table->content, numar_maxim_nou_elemente * sizeof(void *));
	if (dummy == NULL)
	{
		return 0;//redimensionare esuata
	}

	table->content = dummy;
	table->numar_maxim_elemente = numar_maxim_nou_elemente;
	return 1;//redimensionare reusita
}

static inline void adauga_elemente(void ** content, int start, void ** new_elements, int nr_elements)
{
	for (int i = 0; i < nr_elements; i++)
	{
		content[start++] = new_elements[i];
	}
}

int AddVectorItems(Vector table, void ** new_elements, int nr_elements)
{
	//adauga tot sau nimic
	if (table->numar_maxim_elemente < table->numar_elemente + nr_elements)//daca nu e loc
	{
		if (redimensionare_vector(table, table->numar_elemente + nr_elements) == 0)
		{
			return 0;//adaugare esuata
		}
	}
	adauga_elemente(table->content, table->numar_elemente, new_elements, nr_elements);
	table->numar_elemente += nr_elements;
	return 1;//adaugare reusita
}


static inline void mutare_cu_o_pozitie_dreapta(Vector table, int index)
{
	for (int i = table->numar_elemente; i > index; i--)
	{
		table->content[i] = table->content[i - 1];
	}
}

int PutVectorItem(Vector table, void * new_element, int index)
{
	if (new_element == NULL)
	{
		return 0;
	}

	if (table->numar_elemente < index || index < 0)//poate sa fie egal, ar insemna pe ultima pozitie
	{
		return -1;//index invalid
	}
	if (table->numar_elemente == table->numar_maxim_elemente)//daca e plin se dubleaza spatiul
	{
		if (redimensionare_vector(table, table->numar_maxim_elemente << 1) == 0)
		{
			return 0;//adaugare esuata
		}
	}

	mutare_cu_o_pozitie_dreapta(table, index);
	table->content[index] = new_element;
	table->numar_elemente++;

	return 1;//adaugare reusita
}


void * GetVectorItem(Vector table, int index)
{
	if (table->numar_elemente <= index || index < 0)
	{
		return NULL;//index invalid
	}

	return table->content[index];
}


static inline void mutare_cu_o_pozitie_stanga(Vector table, int index)
{
	for (int i = index; i < table->numar_elemente - 1; i++)
	{
		table->content[i] = table->content[i + 1];
	}
	table->numar_elemente--;
}

int DeleteVectorItem(Vector table, int index, void (* dezaloca_element)(void * element))
{
	if (table->numar_elemente <= index || index < 0)
	{
		return 0;//index invalid
	}

	dezaloca_element(table->content[index]);
	mutare_cu_o_pozitie_stanga(table, index);
	return 1;//succes
}


int SearchVectorItem(Vector table, void * element, int (* comparare_elemente)(void * element1, void * element2))
{
	if (element == NULL)
	{
		return -1;
	}

	for (int i = 0; i < table->numar_elemente; i++)
	{
		if (comparare_elemente(table->content[i], element) == 0)
		{
			return i;
		}
	}

	return -1;//elementul nu a fost gasit
}


void SortVector(Vector table, int(* comparare_elemente)(void * element1, void * element2))
{
	//am folosit insertion sort
	for (int i = 1; i < table->numar_elemente; i++)
	{
		int index = i;
		while (index > 0 && comparare_elemente(table->content[index], table->content[index - 1]) == -1)
		{
			table->content[index] = (void **) ((uintptr_t)table->content[index] ^ (uintptr_t)table->content[index - 1]);
			table->content[index - 1] = (void **) ((uintptr_t)table->content[index - 1] ^ (uintptr_t)table->content[index]);
			table->content[index] = (void **) ((uintptr_t)table->content[index] ^ (uintptr_t)table->content[index - 1]);
			index--;
		}
	}
}


static inline int aduaga_elemente_sir(Vector table, void ** sir_elemente, int * nr_elemente, void * (*copy)(void * element))
{
	for (int i = 0; i < table->numar_elemente; i++)
	{
		void * new_element = copy(table->content[i]);
		if (new_element == NULL)
		{
			return 0;//nu s-a putut aloca memorie
		}
		sir_elemente[(*nr_elemente)++] = new_element;
	}

	return 1;
}

static inline void sterge_sir(void ** sir, int nr_elemente, void(*dezaloca_element)(void * element))
{
	for (int i = 0; i < nr_elemente; i++)
	{
		dezaloca_element(sir[i]);
	}
	free(sir);
}

int MergeVectors(Vector table_destinatie, Vector table_sursa, void * (*copy)(void * element), void(*dezaloca_element)(void * element))
{
	void ** sir_elemente = (void **)malloc(table_sursa->numar_elemente * sizeof(void *));
	if (sir_elemente == NULL)
	{
		return 0;
	}

	int nr_elemente_sir = 0;
	int ok = aduaga_elemente_sir(table_sursa, sir_elemente, &nr_elemente_sir, copy);
	if (ok == 0)
	{
		sterge_sir(sir_elemente, nr_elemente_sir, dezaloca_element);
		return 0;
	}

	ok = AddVectorItems(table_destinatie, sir_elemente, nr_elemente_sir);//adauga tot sau nimic
	
	if (ok == 0)
	{
		sterge_sir(sir_elemente, nr_elemente_sir, dezaloca_element);//nu s-au introdus , trebuie eliminate
		return 0;
	}
	else
	{
		free(sir_elemente);
		return 1;//succes
	}
}


void DeleteVector(Vector table, void(*dezaloca_element)(void * element))
{
	for (int i = 0; i < table->numar_elemente; i++)
	{
		dezaloca_element(table->content[i]);
	}
	free(table->content);
	free(table);
}