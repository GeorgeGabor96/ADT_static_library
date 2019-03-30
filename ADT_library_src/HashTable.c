#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"

static const int DIM = 5;

typedef struct nod
{
	struct nod * next;
	void * content;
}nod;

struct htable
{
	nod ** content;
	int nr_elemente;
	int nr_max_elemente;
	int (*hash_funtion)(void * element, int numar_elemente);
	int nr_octeti;//numarul de octeti pentru un element
};

inline static int generic_hash_function(void * p, int nr_octeti, int numar_elemente)
{
	int suma = 0;
	for (int i = 0; i < nr_octeti; i++)
	{
		unsigned char c = *((unsigned char *)p);
		suma += suma ^ c;
		p = (void *)((uintptr_t)p + 1);
	}
	return (suma % numar_elemente);
}


HashTable CreateHashTable(int(*hash_function)(void * element, int numar_elemente), int numar_octeti)
{
	if (numar_octeti < 1)
	{
		return NULL;
	}
	HashTable new_table = (HashTable)malloc(sizeof(struct htable));
	if (new_table == NULL)
	{
		return NULL;
	}

	new_table->content = (nod **)calloc(DIM, sizeof(nod *));
	if (new_table->content == NULL)
	{
		free(new_table);
		return NULL;
	}

	new_table->nr_elemente = 0;
	new_table->nr_max_elemente = DIM;
	new_table->nr_octeti = numar_octeti;
	new_table->hash_funtion = hash_function;

	return new_table;
}


int PrintHashTable(HashTable table, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (table->nr_elemente == 0)
	{
		return 0;
	}

	fprintf(pfile, "Printing:\n");
	for (int i = 0; i < table->nr_max_elemente; i++)
	{
		nod * iterator = table->content[i];
		while (iterator)
		{
			afisare_element(pfile, iterator->content);
			iterator = iterator->next;
			if (iterator)
			{
				fprintf(pfile, " -> ");
			}
		}
		fprintf(pfile, "\n");
	}

	return 1;
}


static nod * aloca_nod(void * content)
{
	nod * new_nod = (nod *)malloc(sizeof(nod));
	if (new_nod == NULL)
	{
		return NULL;
	}
	new_nod->content = content;
	new_nod->next = NULL;

	return new_nod;
}

inline static nod * insert_list(nod * lista, nod * element)
{
	if (lista != NULL)
	{
		element->next = lista;
	}

	return element;
}

inline static float load_factor(HashTable table)
{
	return table ->nr_elemente / (float)table->nr_max_elemente;
}

inline static int is_prime(int n)
{
	for (int i = 2; i < n / 2; i++)
	{
		if (n % i == 0) return 0;
	}
	return 1;
}

static int next_prime(int n)
{
	int dublul = 2 * n;
	if (is_prime(dublul))
	{
		return dublul;
	}

	int raza = 1;
	while (1)
	{
		int numar = dublul + raza;
		if (is_prime(numar))
		{
			return numar;
		}
		numar = dublul - raza++;
		if (is_prime(numar))
		{
			return numar;
		}
	}
}

inline static int get_index(HashTable table, void * p, int dimensiune)
{
	int new_index;
	if (table->hash_funtion == NULL)
	{
		new_index = generic_hash_function(p, table->nr_octeti, dimensiune);
	}
	else
	{
		new_index = table->hash_funtion(p, dimensiune);
	}
	return new_index;
}

static void copy_content(HashTable table, nod ** new_content, int dimensiune)
{
	for (int i = 0; i < table->nr_max_elemente; i++)
	{
		nod * iterator = table->content[i];

		while (iterator)
		{
			int new_index = get_index(table, iterator->content, dimensiune);	
			nod * auxiliar = iterator;
			iterator = iterator->next;
			auxiliar->next = NULL;
			new_content[new_index] = insert_list(new_content[new_index], auxiliar);
		}
	}
}

static int redimensionare_tabela(HashTable table)
{
	int new_dimension = next_prime(table->nr_max_elemente);
	nod ** new_content = (nod **)calloc(new_dimension, sizeof(nod));
	if (new_content == NULL)
	{
		return 0;//failure
	}

	copy_content(table, new_content, new_dimension);
	free(table->content);
	table->content = new_content;
	table->nr_max_elemente = new_dimension;

	return 1; //succes
}

int AddHashTableItem(HashTable table, void * element)
{
	if (element == NULL)
	{
		return 0;
	}
	if (load_factor(table) > 0.75f)
	{
		int ok = redimensionare_tabela(table);
		if (ok == 0)
		{
			return 0;
		}
	}
	int index = get_index(table, element, table->nr_max_elemente);

	nod * new_nod = aloca_nod(element);
	if (new_nod == 0)
	{
		return 0;
	}
	table->content[index] = insert_list(table->content[index], new_nod);
	table->nr_elemente++;

	return 1;
}


static void * search_and_delete_item(nod * lista, void * element, void ** element_cautat, int(*comparare_element)(void * element1, void * element2))
{
	if (lista == NULL)
	{
		return NULL;
	}

	if (comparare_element(lista->content, element) == 0)
	{
		
		*element_cautat = lista->content;
		nod * aux = lista->next;
		free(lista);
		return aux;
	}
	
	nod * iterator = lista;
	while (iterator->next)
	{
		if (comparare_element(iterator->next->content, element) == 0)
		{
			*element_cautat = iterator->next->content;
			nod * aux = iterator->next;
			iterator->next = iterator->next->next;
			free(aux);
			break;
		}
		iterator = iterator->next;
	}
	return lista;
}

int DeleteHashTableItem(HashTable table, void * element, void (*dezaloca_element)(void * element), int(*comparare_element)(void * element1, void * element2))
{
	if (element == NULL)
	{
		return 0;
	}
	
	int index = get_index(table, element, table->nr_max_elemente);
	void * element_cautat = NULL;
	table->content[index] = search_and_delete_item(table->content[index], element, &element_cautat, comparare_element);
    
	if (element_cautat == NULL)
	{
		return 0;
	}
	else
	{
		dezaloca_element(element_cautat);
		table->nr_elemente--;
		return 1;
	}

}


static int search_item(nod * lista, void * element, int(*comparare_elemente)(void * element1, void * element2))
{
	if (lista == NULL)
	{
		return 0;
	}

	nod * iterator = lista;
	while (iterator)
	{
		if (comparare_elemente(iterator->content, element) == 0)
		{
			return 1;
		}
		iterator = iterator->next;
	}
	return 0;
}

int SearchHashTableItem(HashTable table, void * element, int(*comparare_elemente)(void * element1, void * element2))
{
	if (element == NULL)
	{
		return -1;
	}
	int index = get_index(table, element, table->nr_max_elemente);
	int ok = search_item(table->content[index], element, comparare_elemente);
	if (ok == 0)
	{
		return -1;//elementul nu a fost gasit
	}
	else
	{
		return index;
	}
}


static void redistribuire_elemente(HashTable table_sursa, HashTable table_destinatie)
{
	for (int i = 0; i < table_sursa->nr_max_elemente; i++)
	{
		nod * iterator = table_sursa->content[i];
		while (iterator)
		{
			int new_index = get_index(table_destinatie, iterator->content, table_destinatie->nr_max_elemente);
			nod * aux = iterator;
			iterator = iterator->next;
			aux->next = NULL;
			table_destinatie->content[new_index] = insert_list(table_destinatie->content[new_index], aux);
			table_destinatie->nr_elemente++;
		}
	}
}

static HashTable create_table_with_nr(HashTable table, int(*hash_function)(void * element, int numar_elemente))
{
	HashTable new_table = (HashTable)malloc(sizeof(struct htable));
	if (new_table == NULL)
	{
		return NULL;
	}

	new_table->content = (nod **)calloc(table->nr_max_elemente, sizeof(nod *));
	if (new_table->content == NULL)
	{
		free(new_table);
		return NULL;
	}

	if (hash_function == NULL)
	{
		new_table->hash_funtion = NULL;
	}
	else
	{
		new_table->hash_funtion = hash_function;
	}

	new_table->nr_elemente = 0;
	new_table->nr_max_elemente = table->nr_max_elemente;
	new_table->nr_octeti = table->nr_octeti;

	return new_table;
}

void * ReHashTable(HashTable table, int(*hash_function)(void * element, int numar_elemente))
{
	if (table == NULL)
	{
		return NULL;
	}
	HashTable new_table = create_table_with_nr(table, hash_function);
	if (new_table == NULL)
	{
		return table;
	}

	redistribuire_elemente(table, new_table);
	free(table->content);
	free(table);

	return new_table;
}


void DeleteHashTable(HashTable table, void(*dezaloca_element)(void * element))
{
	for (int i = 0; i < table->nr_elemente; i++)
	{
		nod * iterator = table->content[i];
		while (iterator)
		{
			nod * aux = iterator;
			iterator = iterator->next;
			dezaloca_element(aux->content);
			free(aux);
		}
	}

	free(table->content);
	free(table);
}