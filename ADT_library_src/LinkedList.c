#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

typedef struct nod
{
	struct nod * next;
	void * element;
}nod;

struct LISTA_GEN
{
	nod * first;
	nod * last;
	int nr_elemente;
};

LinkedList CreateLinkedList()
{
	LinkedList new_lista = (LinkedList)malloc(sizeof(struct LISTA_GEN));
	if (new_lista == NULL)
	{
		return NULL;
	}

	new_lista->nr_elemente = 0;
	new_lista->first = NULL;
	new_lista->last = NULL;

	return new_lista;
}


int PrintLinkedList(LinkedList list, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (list->nr_elemente == 0)
	{
		return 0;
	}

	nod * iterator = list->first;
	while (iterator)
	{
		afisare_element(pfile, iterator->element);
		fprintf(pfile, "\n");
		iterator = iterator->next;
	}

	return 1;
}


static inline nod * aloca_nod(void * element)
{
	nod * new_nod = (nod *)malloc(sizeof(nod));
	if (new_nod == NULL)
	{
		return NULL;
	}
	new_nod->next = NULL;
	new_nod->element = element;

	return new_nod;
}

int AddLinkedListItem(LinkedList list, void * new_element)
{
	if (new_element == NULL)
	{
		return 0;
	}

	nod * new_nod = aloca_nod(new_element);
	if (new_nod == NULL)
	{
		return 0;
	}

	++list->nr_elemente;
	if (list->first == NULL)
	{
		list->first = new_nod;
		list->last = new_nod;
	}
	else
	{
		list->last->next = new_nod;
		list->last = new_nod;
	}

	return 1;
}


int PutLinkedListItem(LinkedList list, void * element, int index)
{
	if (element == NULL)
	{
		return 0;
	}

	if (list->nr_elemente < index || index < 0)
	{
		return -1;
	}

	nod * new_nod = aloca_nod(element);
	if (new_nod == NULL)
	{
		return 0;
	}

	if (index == 0)//inceput
	{
		new_nod->next = list->first;
		list->first = new_nod;
	}
	else if (index == list->nr_elemente)//sfarsit
	{
		list->last->next = new_nod;
		list->last = new_nod;
	}
	else
	{//inauntru
		int i = 1;
		nod * iterator = list->first;
		while (iterator->next)
		{
			if (index == i++)
			{
				new_nod->next = iterator->next;
				iterator->next = new_nod;
				break;
			}
			iterator = iterator->next;
		}
	}
	list->nr_elemente++;
	
	return 1;
}


void * GetLinkedListItem(LinkedList list, int index)
{
	if (list->nr_elemente <= index || index < 0)
	{
		return NULL;
	}

	if (index == list->nr_elemente - 1)
	{
		return list->last->element;
	}

	nod * iterator = list->first;
	void * element_cautat;
	int i = 0;
	while (iterator)
	{
		if (index == i++)
		{
			element_cautat = iterator->element;
			break;
		}
		iterator = iterator->next;
	}

	return element_cautat;
}


void * DeleteLinkedListItem(LinkedList list, void * element, int(*comparare_element)(void * element1, void * element2))
{
	if (element == NULL)
	{
		return 0;
	}

	nod * iterator = list->first;
	if (comparare_element(iterator->element, element) == 0)//daca ii primul
	{
		list->first = iterator->next;
		void * gasit = iterator->element;
		free(iterator);
		return gasit;
	}
	
	while (iterator->next)
	{
		if (comparare_element(iterator->next->element, element) == 0)
		{
			if (iterator->next->next == NULL)//ultimul element trebuie sters
			{
				list->last = iterator;
			}
			void * gasit = iterator->next->element;
			nod * aux = iterator->next;
			iterator->next = iterator->next->next;
			free(aux);
			return gasit;
		}
		iterator = iterator->next;
	}

	return NULL;//elementul nu a fost gasit;
}


void * SearchLinkedListItem(LinkedList list, void * element, int(*comparare_element)(void * element1, void * element2))
{
	if (element == NULL)
	{
		return NULL;
	}

	if (element == NULL)
	{
		return NULL;
	}

	nod * iterator = list->first;
	
	while (iterator)
	{
		if (comparare_element(iterator->element, element) == 0)
		{
			return iterator->element;
		}
		iterator = iterator->next;
	}

	return NULL;
}


void SortLinkedList(LinkedList list, int(*comparare_element)(void * element1, void * element2))
{
	int ok;
	
	do
	{
		ok = 0;
		nod * iterator1 = list->first;
		nod * iterator2 = iterator1->next;
		while (iterator2)
		{
			if (comparare_element(iterator1->element, iterator2->element) > 0)
			{
				ok++;
				void * aux = iterator1->element;
				iterator1->element = iterator2->element;
				iterator2->element = aux;
			}
			iterator1 = iterator2;
			iterator2 = iterator2->next;
		}
	} while (ok != 0);
}


static inline int adauga_elemente_sir(LinkedList list, nod ** sir, int * nr_elemente, void * (*copy)(void * element))
{
	nod * iterator = list->first;
	while (iterator)
	{
		void * new_element = copy(iterator->element);
		if (new_element == NULL)
		{
			return 0;
		}
		nod * new_nod = aloca_nod(new_element);
		if (new_nod == NULL)
		{
			return 0;
		}
		sir[(*nr_elemente)++] = new_nod;
		iterator = iterator->next;
	}

	return 1;
}

static inline void sterge_sir(nod ** sir, int nr_elemente, void(*dezaloca_element)(void * element))
{
	for (int i = 0; i < nr_elemente; i++)
	{
		dezaloca_element(sir[i]->element);
		free(sir[i]);
	}
	free(sir);
}

static inline void adauga_noduri_lista(LinkedList list, nod ** sir, int nr_elemente)
{
	for (int i = 0; i < nr_elemente; i++)
	{
		list->last->next = sir[i];
		list->last = sir[i];
		++list->nr_elemente;
	}
}

int MergeLinkedLists(LinkedList list_destinatie, LinkedList list_sursa, void * (*copy)(void * element), void(*dezaloca_element)(void * element))
{
	nod ** sir_elemente = (nod **)malloc(list_sursa->nr_elemente * sizeof(nod *));
	if (sir_elemente == NULL)
	{
		return 0;
	}

	int nr_elemente_sir = 0;
	int ok = adauga_elemente_sir(list_sursa, sir_elemente, &nr_elemente_sir, copy);
	if (ok == 0)
	{
		sterge_sir(sir_elemente, nr_elemente_sir, dezaloca_element);
		return 0;
	}
	
	adauga_noduri_lista(list_destinatie, sir_elemente, nr_elemente_sir);//adauga tot sau nimic

	free(sir_elemente);
	return 1;//succes
}


void DeleteLinkedList(LinkedList list, void(*dezaloca_element)(void * element))
{
	nod * iterator = list->first;

	while (iterator)
	{
		nod * temp = iterator;
		iterator = iterator->next;
		dezaloca_element(temp->element);
		free(temp);
	}
	free(list);
}