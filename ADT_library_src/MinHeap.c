#include <stdio.h>
#include <stdlib.h>
#include "MinHeap.h"

static int DIM_MAX_INIT = 15;//dimensiunea initiala pentru heap

struct Min_Heap_GEN
{
	void ** content;
	int numar_elemente;
	int numar_maxim_elemente;
};

Heap CreateHeap()
{
	Heap new_heap = (Heap)malloc(sizeof(struct Min_Heap_GEN));
	if (new_heap == NULL)
	{
		return NULL;
	}

	new_heap->content = (void **)malloc(DIM_MAX_INIT * sizeof(void *));
	if (new_heap->content == NULL)
	{
		free(new_heap);
		return NULL;
	}

	new_heap->numar_elemente = 0;
	new_heap->numar_maxim_elemente = DIM_MAX_INIT;

	return new_heap;
}


static void print_tree(Heap heap, int level, int index, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (index >= heap->numar_elemente)
	{
		return;
	}
	print_tree(heap, level + 1, 2 * index + 2, pfile, afisare_element);//afisare copil drept
	if (2 * index + 2 < heap->numar_elemente)
	{
		for (int i = 0; i < (level + 1) * 10 - 3; i++)
		{
			fprintf(pfile, " ");
		}
		fprintf(pfile, "/\n");
	}
	for (int i = 0; i < level * 10; i++)
	{
		fprintf(pfile, " ");
	}
	afisare_element(pfile, heap->content[index]);
	fprintf(pfile, "\n");
	if (2 * index + 1 < heap->numar_elemente)
	{
		for (int i = 0; i < (level + 1) * 10 - 3; i++)
		{
			fprintf(pfile, " ");
		}
		fprintf(pfile, "\\\n");
	}
	print_tree(heap, level + 1, 2 * index + 1, pfile, afisare_element);//afisare copil stang
}

int PrintHeap(Heap heap, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (heap->numar_elemente == 0)
	{
		return 0;
	}
	print_tree(heap, 0, 0, pfile, afisare_element);
	fprintf(pfile, "\n");
	return 1;
}


inline int redimensionare_heap(Heap heap, int numar_maxim_nou_elemente)
{
	void ** dummy = (void **)realloc(heap->content, numar_maxim_nou_elemente * sizeof(void *));
	if (dummy == NULL)
	{
		return 0;//redimensionare esuata
	}

	heap->content = dummy;
	heap->numar_maxim_elemente = numar_maxim_nou_elemente;
	return 1;//redimensionare reusita
}

inline void swap(void ** content, int i, int j)
{
	void * aux = content[i];
	content[i] = content[j];
	content[j] = aux;
}

static void fix_heap_up(void ** content, int index_child, int(*comparare_elemente)(void *, void *))
{
	//am ajuns la radacina
	if (index_child == 0)
	{
		return;
	}
	int index_parent = (index_child - 1) / 2;
	//parintele este mai mic sau egal cu copilul
	if (comparare_elemente(content[index_child], content[index_parent]) >= 1)
	{
		return;
	}
	//schimbam parinte si copil
	swap(content, index_child, index_parent);
	//continuam in sus
	fix_heap_up(content, index_parent, comparare_elemente);
}

int AddHeapItem(Heap heap, void * element, int(*comparare_elemente)(void *element1, void *element2))
{
	if (element == NULL)
	{
		return 0;
	}

	if (heap->numar_elemente == heap->numar_maxim_elemente)
	{
		if (redimensionare_heap(heap, heap->numar_maxim_elemente << 1) == 0)
		{
			return 0;
		}
	}
	heap->content[heap->numar_elemente] = element;
	fix_heap_up(heap->content, heap->numar_elemente++, comparare_elemente);
	return 1;
}


void * GetHeapMin(Heap heap)
{
	if (heap->numar_elemente == 0)
	{
		return NULL;
	}

	return heap->content[0];
}


static void fix_heap_down(Heap heap, int index_parent, int(*comparare_elemente)(void *, void *))
{
	int index_child_left = 2 * index_parent + 1;
	int index_child_right = 2 * index_parent + 2;
	if (index_child_left >= heap->numar_elemente && index_child_right >= heap->numar_elemente)//nodul nu mai are copii
	{
		return;
	}

	if (index_child_right >= heap->numar_elemente)//nodul nu are copil drept(stang are mereu)
	{
		if (comparare_elemente(heap->content[index_parent], heap->content[index_child_left]) > 0)
		{
			swap(heap->content, index_parent, index_child_left);
		}
		return;
	}

	//are 2 copii
	int ok = comparare_elemente(heap->content[index_child_left], heap->content[index_child_right]);//vedem care copil e mai mic
	if (ok >= 0)//copilul drept e mai mic
	{
		if (comparare_elemente(heap->content[index_parent], heap->content[index_child_right]) > 0)
		{
			swap(heap->content, index_parent, index_child_right);
			fix_heap_down(heap, index_child_right, comparare_elemente);
		}
	}
	else//copilul stang e mai mic
	{
		if (comparare_elemente(heap->content[index_parent], heap->content[index_child_left]) > 0)
		{
			swap(heap->content, index_parent, index_child_left);
			fix_heap_down(heap, index_child_left, comparare_elemente);
		}
	}
}

void * DeleteHeapMin(Heap heap, int (*comparare_elemente)(void *element1, void *element2))
{
	if (heap->numar_elemente == 0)
	{
		return NULL;
	}
	void * minim = heap->content[0];
	heap->content[0] = heap->content[heap->numar_elemente - 1];//schimbam radacina cu ultimul element
	heap->numar_elemente--;
	heap->content[heap->numar_elemente] = NULL;//stergem ultimul element
	
	fix_heap_down(heap, 0, comparare_elemente);//refacem invariant
	
	return minim;
}


inline int determina_pozitite_heap(Heap heap, void * element, int(*comparare_elemente)(void * element1, void * element2))
{
	for (int i = 0; i < heap->numar_elemente; i++)
	{
		if (comparare_elemente(heap->content[i], element) == 0)
		{
			return i;
		}
	}
	return -1;
}

int DeleteHeapItem(Heap heap, void * element, void(*dezaloca_element)(void * element), int(*comparare_elemente)(void *element1, void *element2))
{
	if (element == NULL)
	{
		return 0;
	}

	int index = determina_pozitite_heap(heap, element, comparare_elemente);
	if (index < 0)
	{
		return 0;//elementul nu exista
	}

	dezaloca_element(heap->content[index]);
	heap->content[index] = heap->content[heap->numar_elemente - 1];
	heap->numar_elemente--;
	heap->content[heap->numar_elemente] = NULL;
	
	fix_heap_down(heap, index, comparare_elemente);//noul element de pe poz index va fi mereu mai mare decat parintele, ne uitam doar la fii lui
	return 1;
}


static inline int adauga_elemente_sir(Heap heap_sursa, void ** sir, int * nr_elemente, void * (*copy)(void * element))
{
	for (int i = 0; i < heap_sursa->numar_elemente; i++)
	{
		void * new_element = copy(heap_sursa->content[i]);
		if (new_element == NULL)
		{
			return 0;
		}

		sir[(*nr_elemente)++] = new_element;
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

static inline void adauga_items_heap(Heap heap, void ** sir, int nr_elemente, int(*comparare_element)(void * element1, void * element2))
{
	for (int i = 0; i < nr_elemente; i++)
	{
		heap->content[heap->numar_elemente] = sir[i];
		fix_heap_up(heap->content, heap->numar_elemente++, comparare_element);
	}
}

int MergeMinHeaps(Heap heap_destinatie, Heap heap_sursa, void * (*copy_element)(void * element), int (*comparare_elemente)(void *element1, void *element2), void (*dezaloca_element)(void *element))
{
	//copiem elemente din sursa intr-un vector intermediar
	void ** sir_elemente = (void **)malloc(heap_sursa->numar_elemente * sizeof(void *));
	if (sir_elemente == NULL)
	{
		return 0;
	}

	int nr_elemente_sir = 0;
	int ok = adauga_elemente_sir(heap_sursa, sir_elemente, &nr_elemente_sir, copy_element);
	if (ok == 0)
	{
		sterge_sir(sir_elemente, nr_elemente_sir, dezaloca_element);
		return 0;
	}
	//redimensionam destinatia 
	if (heap_destinatie->numar_maxim_elemente < heap_destinatie->numar_elemente + heap_sursa->numar_elemente)
	{
		ok = redimensionare_heap(heap_destinatie, heap_destinatie->numar_elemente + heap_sursa->numar_elemente);
		if (ok == 0)
		{
			sterge_sir(sir_elemente, nr_elemente_sir, dezaloca_element);
			return 0;
		}
	}

	adauga_items_heap(heap_destinatie, sir_elemente, nr_elemente_sir, comparare_elemente);//adauga tot sau nimic
	free(sir_elemente);

	return 1;//succes

}


void DeleteHeap(Heap heap, void(*dezaloca_element)(void * element))
{
	for (int i = 0; i < heap->numar_elemente; i++)
	{
		dezaloca_element(heap->content[i]);
	}
	free(heap->content);
	free(heap);
}