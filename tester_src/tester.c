#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "VECTOR.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "MinHeap.h"
#include "BinarySearchTree.h"
#include "BalancedBST.h"
#include "intreg.h"
#include "Persoana.h"
#define DIM 100

//acestea sunt necesare aproape la fiecare functie
FILE * p_in;
FILE * p_out;
void ** sir_structuri;// retine pointeri la fiecare structura
char ** sir_tip_data;// ritine tipul de data pentru fiecare structura
char * argumente;// argumentele functiilor din fisierul de intrare

inline litera_mare(char litera)
{
	if (litera >= 'A' && litera <= 'Z')
	{
		return 1;
	}
	return 0;
}

inline int check_one_argument()
{
	if (argumente == NULL)
	{
		fprintf(p_out, "Error: Instance unknown\n");
		return 0;
	}
	if (strlen(argumente) != 1 || !litera_mare(argumente[0]) )
	{
		fprintf(p_out, "Error: Argument mismatch\n");
		return 0;
	}

	if (sir_structuri[argumente[0] - 'A'] == NULL)
	{
		fprintf(p_out, "Error: Illegal operation. Data structure does not exist\n");
		return 0;
	}
	return 1;//ii ok
}

inline int check_two_arguments()
{
	if (argumente == NULL)
	{
		fprintf(p_out, "Error: Instance unknown\n");
		return 0;
	}
	if (strlen(argumente) != 3 || argumente[1] != ' ' ||
		!litera_mare(argumente[0]) || !litera_mare(argumente[2]))
	{
		fprintf(p_out, "Error: Argument mismatch\n");
		return 0;
	}

	if (sir_structuri[argumente[0] - 'A'] == NULL || sir_structuri[argumente[2] - 'A'] == NULL)
	{
		fprintf(p_out, "Error: Illegal operation. Data structure does not exist\n");
		return 0;
	}
	return 1;//ii ok
}

inline int verifica_tip_data(char * tip)
{
	if (strcmp(tip, "INTREG") == 0)
	{
		return 1;
	}
	else if (strcmp(tip, "PERSOANA") == 0)
	{
		return 1;
	}
	else
	{
		fprintf(p_out, "ERROR: Unknown data type\n");
		return 0;
	}
}

inline void delete_new_line_character(char * sir_structuri)
{
	int lungime_sir = strlen(sir_structuri);
	if (sir_structuri[lungime_sir - 1] == '\n')
	{
		sir_structuri[lungime_sir - 1] = '\0';
	}
}

inline char * copy_sir(char * sir_structuri)
{
	int lungime_sir = strlen(sir_structuri) + 1;
	char * new_sir = (char *)malloc(lungime_sir);
	if (new_sir == NULL)
	{
		return NULL;
	}
	memcpy(new_sir, sir_structuri, lungime_sir);
	return new_sir;
}

inline FILE * deschide_fisier(char * cale, char * mod)
{
	FILE * pfile = fopen(cale, mod);
	if (pfile == NULL)
	{
		fprintf(stderr, "Nu s-a putut deschide fisierul %s\n", cale);
		exit(1);
	}

	return pfile;
}


/*
	functie pentru afisarea unei structuri, nu stim ce structura
	folosita pentru PrintVector, PrintLinkedList, PrintHashTable, PrintHeap, PrintBST, PreorderBST, InorderBST,
	        PostorderBST, PrintBalancedBST, PreorderBalancedBST, InorderBalancedBST, PostorderBalancedBST
*/
void afisare_struct(int (*afisare_structura)(void * structura, FILE * pfile, void (*afisare_element)(FILE * pfile, void * element)))
{
	if (check_one_argument() == 0)
	{
		return;
	}
	
	int ok;
	int index_struct = argumente[0] - 'A';
	
	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		ok = afisare_structura(sir_structuri[index_struct], p_out, afisare_intreg);
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		ok = afisare_structura(sir_structuri[index_struct], p_out, afisare_persoana);
	}//tot timpul se afla un tip valid in sir_tip_data
	
	if (ok == 0)
	{
		fprintf(p_out, "Printing: Structure is empty\n");
	}
}


/*
functie pentru stergerea unei structuri
folosit pentru DeleteVector, DeleteLinkedList, DeleteHashTable, DeleteHeap, DeleteBST, DeleteBalancedBST
*/
void delete_struct(void(*destroy_struct)(void * sir, void(*dezaloca_element)(void * element)))
{
	if (check_one_argument() == 0)
	{
		return;
	}

	int index_struct = argumente[0] - 'A';
	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		destroy_struct(sir_structuri[index_struct], &dezaloca_intreg);
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		destroy_struct(sir_structuri[index_struct], &dezaloca_persoana);
	}

	sir_structuri[index_struct] = NULL;
	free(sir_tip_data[index_struct]);
	sir_tip_data[index_struct] = NULL;
}


/*
	functie pentru crearea unei structuri generice
	folosita pentru CreateVector, CreateLinkedList, CreateHeap, CreateBST, CreateBalancedBST
*/
void create_new_struct(void * (CreateADT)(), void (*delete_ADT)(void * structura, void (*dezaloca_element)(void * element)))
{
	//testam argumentul 
	if (argumente == NULL)
	{
		fprintf(p_out, "Error: Instance unknown\n");
		return;
	}
	
	if (strlen(argumente) != 1 || !litera_mare(argumente[0]))
	{
		fprintf(p_out, "Error: Argument mismatch\n");
		return;
	}
	int index_struct = argumente[0] - 'A';
	if (sir_structuri[index_struct] != NULL)
	{
		fprintf(p_out, "Error: attempt to recreate an existing structure\n");
		return;
	}
	//verificam sa fie un tip de data cunoscut
	char tip_data[DIM];
	fgets(tip_data, DIM, p_in);
	delete_new_line_character(tip_data);
	if (verifica_tip_data(tip_data) == 0)
	{
		return;
	}

	void * new_struct = CreateADT();//creem structura nu stim care
	if (new_struct == NULL)
	{
		fprintf(p_out, "Error: Memory allocation failed\n");
		return;
	}

	char * new_sir = copy_sir(tip_data);
	if (new_sir == NULL)
	{
		fprintf(p_out, "Error: Memory allocation failed\n");
		delete_ADT(new_struct, NULL);//nu are elemente, al doilea argument nu va fii folosit
		return;
	}
	//salvam in siruri
	sir_structuri[index_struct] = new_struct;
	sir_tip_data[index_struct] = new_sir;
}


/*
	fucntie pentru crearea unei structuri generica ce foloseste o functie de hash
	folosita pentru CreateHashTable
*/
void create_new_struct_with_hash_function(void * (*create_struct_with_hash_function)(int(*hash_function)(void * element, int numar_elemente), int numar_octeti), void(*delete_ADT)(void * structura, void(*dezaloca_element)(void * element)))
{
	if (argumente == NULL)
	{
		fprintf(p_out, "Error: Instance unknown\n");
		return;
	}

	if (strlen(argumente) != 1 || !litera_mare(argumente[0]))
	{
		fprintf(p_out, "Error: Argument mismatch\n");
		return;
	}

	int index_struct = argumente[0] - 'A';
	if (sir_structuri[index_struct] != NULL)
	{
		fprintf(p_out, "Error: attempt to recreate an existing structure\n");
		return;
	}
	
	char linie[DIM];
	fgets(linie, DIM, p_in);
	delete_new_line_character(linie);
	char * tip_data = strtok(linie, " ");
	if (verifica_tip_data(tip_data) == 0)
	{
		return;
	}
	char * functie_hash = strtok(NULL, " \0");
	
	char * temp = strtok(NULL, " \0");
	if (temp != NULL)
	{
		fprintf(p_out, "Error: Argument mismatch\n");
	}

	void * new_struct = NULL;
	if (functie_hash == NULL)
	{
		new_struct = create_struct_with_hash_function(NULL, sizeof(intreg));
	}
	else if (strcmp(functie_hash, "hi") == 0)
	{
		new_struct = create_struct_with_hash_function(hash_intreg, sizeof(intreg));
	}
	else if (strcmp(functie_hash, "hp") == 0)
	{
		new_struct = create_struct_with_hash_function(hash_persoana, sizeof(persoana));
	}
	else
	{
		fprintf(p_out, "Error: Unrecognized hash function\n");
		return;
	}

	if (new_struct == NULL)
	{
		fprintf(p_out, "Error: Memory allocation failed\n");
		return;
	}

	char * new_sir = copy_sir(tip_data);
	if (new_sir == NULL)
	{
		fprintf(p_out, "Error: Memory allocation failed\n");
		delete_ADT(new_struct, NULL);//nu are elemente, al doilea argument nu va fii folosit
		return;
	}
	//salvam in siruri
	sir_structuri[index_struct] = new_struct;
	sir_tip_data[index_struct] = new_sir;
}


void ** aloca_elemente(int numar_elemente, void * (*citeste_element)(FILE * p_in, FILE * p_out), void (*dezaloca_element)(void *element))
{
	void ** sir_elemente = (void **)malloc(numar_elemente * sizeof(void *));
	if (sir_elemente == NULL)
	{
		return NULL;
	}
	int pozitie_sir = 0;//pozitia la care se adauga la un moment dat
	for (int i = 0; i < numar_elemente; i++)//citirea
	{
		void * new_inregistrare = citeste_element(p_in, p_out);
		if (new_inregistrare == NULL)
		{
			for (int j = 0; j < pozitie_sir; j++)//eliminam sirul
			{
				dezaloca_element(sir_elemente[j]);
			}
			free(sir_elemente);
			return NULL;
		}
		sir_elemente[pozitie_sir++] = new_inregistrare;
	}

	return sir_elemente;
}

/*
	functie pentru adaugarea intr-o sturctura a unui numar de elemente
	folosita pentru AddVectorItems
*/
void adaugare_items_struct(int(*ADD_items)(void * structura, void ** sir_elemente, int numar_elemente))
{
	if (check_one_argument() == 0)
	{
		return;
	}
	int index_struct = argumente[0] - 'A';
	int numar_elemente;

	int ok = fscanf(p_in, "%d ", &numar_elemente);
	if (ok == 0)
	{
		fprintf(p_out, "Error: Type mismatch\n");
		return;
	}

	void ** sir_elemente;
	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		sir_elemente = aloca_elemente(numar_elemente, citeste_intreg, dezaloca_intreg);
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		sir_elemente = aloca_elemente(numar_elemente, citeste_persoana, dezaloca_persoana);
	}

	if (sir_elemente == NULL)
	{
		return;
	}

	ok = ADD_items(sir_structuri[index_struct], sir_elemente, numar_elemente);
	if (ok == 0)
	{
		fprintf(p_out, "Error: Memory allocation failed\n");
	}
}


/*
functie pentru adaugarea de elemente intr-o structura
folosita pentru AddLinkedListItem
*/
void adaugare_item_at_end_struct(int (*ADD_item)(void *, void *))
{
	if (check_one_argument() == 0)
	{
		return;
	}

	int index_struct = argumente[0] - 'A';
	void * new_inregistrare;
	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		new_inregistrare = citeste_intreg(p_in, p_out);
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		new_inregistrare = citeste_persoana(p_in, p_out);
	}

	if (new_inregistrare == NULL)
	{
		return;
	}

	int ok = ADD_item(sir_structuri[index_struct], new_inregistrare);
	if (ok == 0)
	{
		fprintf(p_out, "Error: Memory allocation failed\n");
	}
}


/*
	functie pentru adaugarea unei inregistrari intr-o structura la un index
	folosita pentru PutVectorItem, PutLinkedListItem
*/
void add_inregistrare_at_index_struct(int (*add_item_at_index_struct)(void * strutura, void * element, int index))
{
	if (check_one_argument() == 0)
	{
		return;
	}
	int index;
	int ok = fscanf(p_in, "%d ", &index);
	if (ok == 0)
	{
		fprintf(p_out, "Error: Missing value\n");
		return;
	}

	int index_struct = argumente[0] - 'A';
	void * new_inregistrare;
	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		new_inregistrare = citeste_intreg(p_in, p_out);	
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		new_inregistrare = citeste_persoana(p_in, p_out);
	}
	if (new_inregistrare == NULL)
	{
		return;
	}

	ok = add_item_at_index_struct(sir_structuri[index_struct], new_inregistrare, index);
	if (ok == 0)
	{
		fprintf(p_out, "Error: Memory allocation failed\n");
	}
	else if (ok == -1)
	{
		fprintf(p_out, "Error: Index out of bounds \n");
	}		
}


/*
	functie pentru adaugarea unui element intr-o structura ce foloseste o functie de dispersie, sau nu conteaza ordinea elementelor
	folosita la AddHashTableItem
*/
void add_item(int(*adauga_element)(void * structura, void * element))
{
	if (check_one_argument() == 0)
	{
		return;
	}
	int index_struct = argumente[0] - 'A';
	void * new_inregistrare;
	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		new_inregistrare = citeste_intreg(p_in, p_out);
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		new_inregistrare = citeste_persoana(p_in, p_out);
	}
	if (new_inregistrare == NULL)
	{
		return;
	}

	int ok = adauga_element(sir_structuri[index_struct], new_inregistrare);
	if (ok == 0)
	{
		fprintf(p_out, "Error: Memory allocation failed\n");
	}
}


/*
functie pentru stergerea unei inregistrari intr-o structura la un index
folosita pentru DeleteVectorItem
*/
void delete_inregistrare_at_index_struct(int (*delete_item_at_index_struct)(void *, int, void (*dezaloca_element)(void * element)))
{
	if (check_one_argument() == 0)
	{
		return;
	}

	int index;
	int ok = fscanf(p_in, "%d ", &index);
	if (ok == 0)
	{
		fprintf(p_out, "Error: Missing value\n");
		return;
	}
	int index_struct = argumente[0] - 'A';
  

	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		ok = delete_item_at_index_struct(sir_structuri[index_struct], index, &dezaloca_intreg);
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		ok = delete_item_at_index_struct(sir_structuri[index_struct], index, &dezaloca_persoana);
	}

	if (ok == 0)
	{
		fprintf(p_out, "Error: Index out of bounds\n");
	}
}

/*
functie pentru cautarea unei inregistrari intr-o structura
folosita pentru SearchVectorItem, SearchHashTableItem
*/
void cauta_inregistrare_return_index_struct(int (*search_item_struct)(void *, void *, int(*comparare_elemente)(void * element1, void * element2)))
{
	if (check_one_argument() == 0)
	{
		return;
	}

	void * search_item;
	int index;
	int index_struct = argumente[0] - 'A';

	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		search_item = citeste_intreg(p_in, p_out);
		if (search_item == NULL)
		{
			return;
		}
		index = search_item_struct(sir_structuri[index_struct], search_item, &compare_intreg);
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		search_item = citeste_persoana(p_in, p_out);
		if (search_item == NULL)
		{
			return;
		}
		index = search_item_struct(sir_structuri[index_struct], search_item, &compare_persoana);
	}

	if (index == -1)
	{
		fprintf(p_out, "Error: Item not found\n");
	}
	else
	{
		fprintf(p_out, "Found at index %d\n", index);
	}
}


/*
functie pentru returnarea unei inregistrari intr-o structura de la un index
folosita pentru GetVectorItem, GetLinkListItem
*/
void get_inregistrare_at_index_struct(void * (*get_item_struct)(void * structura, int index))
{
	if (check_one_argument() == 0)
	{
		return;
	}

	int index;
	int ok = fscanf(p_in, "%d ", &index);
	if (ok == 0)
	{
		fprintf(p_out, "Error: Missing value\n");
		return;
	}
	int index_struct = argumente[0] - 'A';

	void * item_found = get_item_struct(sir_structuri[index_struct], index);
	if (item_found == NULL)
	{
		fprintf(p_out, "Error: Index out of bounds\n");
	}
	else
	{
		fprintf(p_out, "Found : ");
		if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
		{
			afisare_intreg(p_out, item_found);
		}
		else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
		{
			afisare_persoana(p_out, item_found);
		}
		fprintf(p_out, "\n");
	}
}


/*
functie pentru cautarea unei inregistrari intr - o structura
folosita pentru SortVector, SortLinkedList
*/
void sort_struct(void (*sort_items_struct)(void *, int(*comparare_elemente)(void * element1, void * element2)))
{
	if (check_one_argument() == 0)
	{
		return;
	}
	int index_struct = argumente[0] - 'A';
	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		sort_items_struct(sir_structuri[index_struct], &compare_intreg);
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		sort_items_struct(sir_structuri[index_struct], &compare_persoana);
	}
}


/*
functie pentru cautarea unei inregistrari intr - o structura
folosita pentru MergeVectors, MergeLinkedLists
*/
void merge_structs(int (*merge_structs)(void * struct1, void * struct2, void * (*copy)(void * element), void (*dezaloca_element)(void *element)))
{
	if (check_two_arguments() == 0)
	{
		return;
	}
	int index_struct1 = argumente[0] - 'A';
	int index_struct2 = argumente[2] - 'A';
	int ok;

	if (strcmp(sir_tip_data[index_struct1], sir_tip_data[index_struct2]) != 0)
	{
		fprintf(p_out, "Error: Data structures contain different data types\n");
	}
	else if (strcmp(sir_tip_data[index_struct1], "INTREG") == 0)
	{
		ok = merge_structs(sir_structuri[index_struct1], sir_structuri[index_struct2], copy_intreg, dezaloca_intreg);
	}
	else if (strcmp(sir_tip_data[index_struct1], "PERSOANA") == 0)
	{
		ok = merge_structs(sir_structuri[index_struct1], sir_structuri[index_struct2], copy_persoana, dezaloca_persoana);
	}

	if (ok == 0)
	{
		fprintf(p_out, "Error: Memory allocation failed\n");
	}
}


/*
functie pentru stergerea unei inregistrari intr-o structura dupa valoare
folosita la DeleteLinkListItem, DeleteBSTItem, DeleteBalancedBSTItem
*/
void delete_and_return_inregistrare_with_value_struct(void * delete_item_with_value_struct(void * structura, void *element, int(*compare)(void * element1, void * element2)))
{
	void * element;
	void * element_returnat;
	int index_struct = argumente[0] - 'A';

	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		element = citeste_intreg(p_in, p_out);
		if (element == NULL)
		{
			return;
		}
		element_returnat = delete_item_with_value_struct(sir_structuri[index_struct], element, &compare_intreg);
		if (element_returnat == NULL)
		{
			fprintf(p_out, "Cannot delete: Item not found\n");
		}
		else
		{
			fprintf(p_out, "Item deleted : ");
			afisare_intreg(p_out, element_returnat);
			fprintf(p_out, "\n");
			dezaloca_intreg(element_returnat);
		}
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		element = citeste_persoana(p_in, p_out);
		if (element == NULL)
		{
			return;
		}
		element_returnat = delete_item_with_value_struct(sir_structuri[index_struct], element, &compare_persoana);
		if (element == NULL)
		{
			fprintf(p_out, "Cannot delete: Item not found\n");
		}
		else
		{
			fprintf(p_out, "Item deleted : ");
			afisare_persoana(p_out, element_returnat);
			fprintf(p_out, "\n");
			dezaloca_persoana(element_returnat);
		}
	}
}


/*
functie pentru cautarea unei inregistrari intr-o structura
folosita la SearchLinkListItem, SearchBSTItem, SearchBalancedBSTItem
*/
void cauta_inregistrare_return_value_struct(void *(*search_item_struct)(void * structura, void * element, int(*comparare_elemente)(void * element1, void * element2)))
{
	if (check_one_argument() == 0)
	{
		return;
	}

	void * search_item;
	void * element_returnat;
	int index_struct = argumente[0] - 'A';

	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		search_item = citeste_intreg(p_in, p_out);
		if (search_item == NULL)
		{
			return;
		}
		element_returnat = search_item_struct(sir_structuri[index_struct], search_item, &compare_intreg);
		if (element_returnat == NULL)
		{
			fprintf(p_out, "Error: Item not found\n");
		}
		else
		{
			fprintf(p_out, "Item found : ");
			afisare_intreg(p_out, element_returnat);
			fprintf(p_out, "\n");
		}
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		search_item = citeste_persoana(p_in, p_out);
		if (search_item == NULL)
		{
			return;
		}
		element_returnat = search_item_struct(sir_structuri[index_struct], search_item, &compare_persoana);
		if (element_returnat == NULL)
		{
			fprintf(p_out, "Error: Item not found\n");
		}
		else
		{
			fprintf(p_out, "Item found : ");
			afisare_persoana(p_out, element_returnat);
			fprintf(p_out, "\n");
		}
	}
}

/*
functie pentru adaugarea unui element intr-o structura ordonata
folosita pentru AddHeapItem, AddBSTItem, AddBalancedBstItem
*/
void adauga_inregistrare_comparare_struct(int (*adauga_item)(void * structura, void * element, int(*comparare_elemente)(void * element1, void * element2)))
{
	if (check_one_argument() == 0)
	{
		return;
	}
	int index_struct = argumente[0] - 'A';
	void * new_inregistrare;
	int ok;
	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
			new_inregistrare = citeste_intreg(p_in, p_out);
			if (new_inregistrare == NULL)
			{
				return;
			}
			ok = adauga_item(sir_structuri[index_struct], new_inregistrare, &compare_intreg);
			if (ok == 0)
			{
				fprintf(p_out, "Error: Memory allocation failed\n");
			}
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
			new_inregistrare = citeste_persoana(p_in, p_out);
			if (new_inregistrare == NULL)
			{
				return;
			}
			ok = adauga_item(sir_structuri[index_struct], new_inregistrare, &compare_persoana);
			if (ok == 0)
			{
				fprintf(p_out, "Error: Memory allocation failed\n");
			}
	}
}

/*
functie pentru returnarea minimului dintr-o structura
folosita pentru GetHeapMin	
*/
void get_min_inregistrare_struct(void * (*get_min)(void * structura))
{
	if (check_one_argument() == 0)
	{
		return;
	}
	int index_struct = argumente[0] - 'A';
	void * min = get_min(sir_structuri[index_struct]);
	if (min == NULL)
	{
		fprintf(p_out, "ERROR : Structure is empty\n");
		return;
	}

	fprintf(p_out, "Minim : ");
	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		afisare_intreg(p_out, min);
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		afisare_persoana(p_out, min);
	}
	fprintf(p_out, "\n");
}

/*
functie pentru stergerea si returnarea elementului minim dintr-o structura
folosita la DeleteHeapMin
*/
void delete_and_return_min_inregistrare_struct(void * (*delete_min)(void * structura, int(*compara_elemente)(void * element1, void * element2)))
{
	if (check_one_argument() == 0)
	{
		return;
	}

	int index_struct = argumente[0] - 'A';
	void * minim;
	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		minim = delete_min(sir_structuri[index_struct], &compare_intreg);
		if (minim == NULL)
		{
			fprintf(p_out, "ERROR : Structure is empty\n");
			return;
		}
		fprintf(p_out, "Deleted :");
		afisare_intreg(p_out, minim);
		fprintf(p_out, "\n");
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		minim = delete_min(sir_structuri[index_struct], &compare_persoana);
		if (minim == NULL)
		{
			fprintf(p_out, "ERROR : Structure is empty\n");
			return;
		}
		fprintf(p_out, "Deleted :");
		afisare_persoana(p_out, minim);
		fprintf(p_out, "\n");
	}
}


/*
functie pentru sergerea unui element, dupa valoare, dintr-o structura
        folosita la DeleteHeapItem, DeleteHashTableItem
*/
void delete_inregistrare_with_value_struct(int(*sterge_item)(void *, void *, void(*dezaloca)(void *), int(*comparare)(void *, void *)))
{
	if (check_one_argument() == 0)
	{
		return;
	}

	void * element_cautat;
	int index_struct = argumente[0] - 'A';
	int ok;
	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		element_cautat = citeste_intreg(p_in, p_out);
		if (element_cautat == NULL)
		{
			return;
		}
		ok = sterge_item(sir_structuri[index_struct], element_cautat, &dezaloca_intreg, &compare_intreg);
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		element_cautat = citeste_persoana(p_in, p_out);
		if (element_cautat == NULL)
		{
			return;
		}
		ok = sterge_item(sir_structuri[index_struct], element_cautat, &dezaloca_persoana, &compare_persoana);
	}
	if (ok == 0)
	{
		fprintf(p_out, "Cannot delete: Item not found\n");
	}
}


/*
functie pentru lipirea a doua structuri respectand invariant
folosit la MergeMinHeaps, MergeBSTs, MergeBalancedBSTs
*/
void merge_with_compare_struct(int(*merge_struct)(void * structura_destinatie, void * structura_sursa , void * (*copy_element)(void * element), int(*comparare_elemente)(void * element1, void * element2), void(*dezaloca_element)(void * element)))
{
	if (check_two_arguments() == 0)
	{
		return;
	}

	int index_struct1 = argumente[0] - 'A';
	int index_struct2 = argumente[2] - 'A';
	int ok;

	if (strcmp(sir_tip_data[index_struct1], sir_tip_data[index_struct2]) != 0)
	{
		fprintf(p_out, "Error: Data structures contain different data types\n");
	}
	if (strcmp(sir_tip_data[index_struct1], "INTREG") == 0)
	{
		ok = merge_struct(sir_structuri[index_struct1], sir_structuri[index_struct2], &copy_intreg, &compare_intreg, &dezaloca_intreg);
	}
	else if (strcmp(sir_tip_data[index_struct1], "PERSOANA") == 0)
	{
		ok = merge_struct(sir_structuri[index_struct1], sir_structuri[index_struct2], &copy_persoana, &compare_persoana, &dezaloca_persoana);
	}

	if (ok == 0)
	{
		fprintf(p_out, "Error: Memory allocation failed ");
	}
}


/*
functie pentru gasirea inaltimii unui element intr-o structura
folosita pentru HightBST, HightBanlancedBST
*/
void find_height_of_element_struct(int(*find_height)(void * structura, void * element, int (*comparare_elemente)(void * element1, void * element2)))
{
	if (check_one_argument() == 0)
	{
		return;
	}
	int index_struct = argumente[0] - 'A';
	void * element;
	int height;
	if (strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		element = citeste_intreg(p_in, p_out);
		if (element == NULL)
		{
			return;
		}
		height = find_height(sir_structuri[index_struct], element, compare_intreg);
	}
	else if (strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		element = citeste_persoana(p_in, p_out);
		if (element == NULL)
		{
			return;
		}
		height = find_height(sir_structuri[index_struct], element, compare_persoana);
	}

	if (height == -1)
	{
		fprintf(p_out, "Error : Item not found\n");
	}
	else
	{
		fprintf(p_out, "Height equals %d\n", height);
	}
}

/*
	functie pentru schimbarea si redistribuirea elementelor intr-o structura ce foloseste o functie de hash
	folosita pentru ReHashTable
*/
void rehash_struct(void * rehash(void * table, int(*hash_function)(void * element, int dimensiune)))
{
	if (argumente == NULL)
	{
		fprintf(p_out, "Error: Instance unknown\n");
		return;
	}
	//verificare argument structura
	char * entitate = strtok(argumente, " \0");
	if (entitate == NULL)
	{
		fprintf(p_out, "Error: Instance unknown\n");
		return;
	}
	if (strlen(entitate) != 1)
	{
		fprintf(p_out, "Error: Argument mismatch\n");
		return;
	}
	int index_struct = entitate[0] - 'A';
	if (sir_structuri[index_struct] == NULL)
	{
		fprintf(p_out, "Error: Illegal operation. Data structure does not exist\n");
		return;
	}
	//verificare argument functie hash 
	char * hash = strtok(NULL, " \0");
	if (hash == NULL)
	{
		sir_structuri[index_struct] = ReHashTable(sir_structuri[index_struct], NULL);
	}
	else if (strcmp(hash, "hi") == 0 && strcmp(sir_tip_data[index_struct], "INTREG") == 0)
	{
		sir_structuri[index_struct] = ReHashTable(sir_structuri[index_struct], hash_intreg);
	}
	else if (strcmp(hash, "hp") == 0 && strcmp(sir_tip_data[index_struct], "PERSOANA") == 0)
	{
		sir_structuri[index_struct] = ReHashTable(sir_structuri[index_struct], hash_persoana);
	}
	else
	{
		fprintf(p_out, "Error: Unrecognized hash function\n");
	}

}

//aici verific fiecare instructiune din fisier
void process_file(const char * path_in, const char * path_out)
{
	p_in = fopen(path_in, "r");
	if (p_in == NULL)
	{
		fprintf(stderr, "Nu s-a putut deschide fisierul %s\n", path_in);
		return;
	}
	
	//char * fisier_out = generare_cale_fisier_iesire(path);
	p_out = fopen(path_out, "w");
	if (p_out == NULL)
	{
		fprintf(stderr, "Nu s-a putut deschide fisierul %s\n", path_out);
		return;
	}

	sir_structuri = (void **)calloc(26, sizeof(void *));//retine structurile de date
	sir_tip_data = (char **)calloc(26, sizeof(char *));//retine tipul de data stocat de fiecare structura
	if (sir_structuri == NULL || sir_tip_data == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed\n");
		fclose(p_in);
		fclose(p_out);
		exit(1);
	}

	char linie[DIM];
	while (fgets(linie, DIM, p_in) != NULL)
	{
		delete_new_line_character(linie);
		char * comanda = strtok(linie, " ");
		argumente = strtok(NULL, "\0");
		//printf("%s %s\n", comanda, argumente);

		if (comanda == NULL)
		{
			continue;
		}

		//operatii pe Vector
		if (strcmp(comanda, "CreateVector") == 0)
		{
			create_new_struct(&CreateVector, &DeleteVector);
			continue;
		}
		if (strcmp(comanda, "PrintVector") == 0)
		{
			afisare_struct(&PrintVector);
			continue;
		}
		if (strcmp(comanda, "AddVectorItems") == 0)
		{
			adaugare_items_struct(&AddVectorItems);
			continue;
		}
		if (strcmp(comanda, "PutVectorItem") == 0)
		{
			add_inregistrare_at_index_struct(&PutVectorItem);
			continue;
		}
		if (strcmp(comanda, "GetVectorItem") == 0)
		{
			get_inregistrare_at_index_struct(&GetVectorItem);
			continue;
		}
		if (strcmp(comanda, "DeleteVectorItem") == 0)
		{
			delete_inregistrare_at_index_struct(&DeleteVectorItem);
			continue;
		}
		if (strcmp(comanda, "SearchVectorItem") == 0)
		{
			cauta_inregistrare_return_index_struct(&SearchVectorItem);
			continue;
		}
		if (strcmp(comanda, "SortVector") == 0)
		{
			sort_struct(&SortVector);
			continue;
		}
		if (strcmp(comanda, "MergeVectors") == 0)
		{
			merge_structs(&MergeVectors);
			continue;
		}
		if (strcmp(comanda, "DeleteVector") == 0)
		{
			delete_struct(&DeleteVector);
			continue;
		}

		//operatii pe LinkedList
		if (strcmp(comanda, "CreateLinkedList") == 0)
		{
			create_new_struct(&CreateLinkedList, &DeleteLinkedList);
			continue;
		}
		if (strcmp(comanda, "PrintLinkedList") == 0)
		{
			afisare_struct(&PrintLinkedList);
			continue;
		}
		if (strcmp(comanda, "AddLinkedListItem") == 0)
		{
			adaugare_item_at_end_struct(&AddLinkedListItem);
			continue;
		}
		if (strcmp(comanda, "PutLinkedListItem") == 0)
		{
			add_inregistrare_at_index_struct(&PutLinkedListItem);
			continue;
		}
		if (strcmp(comanda, "GetLinkedListItem") == 0)
		{
			get_inregistrare_at_index_struct(&GetLinkedListItem);
			continue;
		}
		if (strcmp(comanda, "DeleteLinkedListItem") == 0)
		{
			delete_and_return_inregistrare_with_value_struct(&DeleteLinkedListItem);
			continue;
		}
		if (strcmp(comanda, "SearchLinkedListItem") == 0)
		{
			cauta_inregistrare_return_value_struct(&SearchLinkedListItem);
			continue;
		}
		if (strcmp(comanda, "SortLinkedList") == 0)
		{
			sort_struct(&SortLinkedList);
			continue;
		}
		if (strcmp(comanda, "MergeLinkedLists") == 0)
		{
			merge_structs(&MergeLinkedLists);
			continue;
		}
		if (strcmp(comanda, "DeleteLinkedList") == 0)
		{
			delete_struct(&DeleteLinkedList);
			continue;
		}

		//operatii pentru HashTable
		if (strcmp(comanda, "CreateHashTable") == 0)
		{
			create_new_struct_with_hash_function(&CreateHashTable, &DeleteHashTable);
			continue;
		}
		if (strcmp(comanda, "PrintHashTable") == 0)
		{
			afisare_struct(&PrintHashTable);
			continue;
		}
		if (strcmp(comanda, "AddHashTableItem") == 0)
		{
			add_item(&AddHashTableItem);
			continue;
		}
		if (strcmp(comanda, "DeleteHashTableItem") == 0)
		{
			delete_inregistrare_with_value_struct(&DeleteHashTableItem);
			continue;
		}
		if (strcmp(comanda, "SearchHashTableItem") == 0)
		{
			cauta_inregistrare_return_index_struct(&SearchHashTableItem);
			continue;
		}
		if (strcmp(comanda, "ReHashTable") == 0)
		{
			rehash_struct(&ReHashTable);
			continue;
		}
		if (strcmp(comanda, "DeleteHashTable") == 0)
		{
			delete_struct(&DeleteHashTable);
			continue;
		}

		//operatii pentru Heap
		if (strcmp(comanda, "CreateHeap") == 0)
		{
			create_new_struct(&CreateHeap, &DeleteHeap);
			continue;
		}
		if (strcmp(comanda, "PrintHeap") == 0)
		{
			afisare_struct(&PrintHeap);
			continue;
		}
		if (strcmp(comanda, "AddHeapItem") == 0)
		{
			adauga_inregistrare_comparare_struct(&AddHeapItem);
			continue;
		}
		if (strcmp(comanda, "GetHeapMin") == 0)
		{
			get_min_inregistrare_struct(&GetHeapMin);
			continue;
		}
		if (strcmp(comanda, "DeleteHeapMin") == 0)
		{
			delete_and_return_min_inregistrare_struct(&DeleteHeapMin);
			continue;
		}
		if (strcmp(comanda, "DeleteHeapItem") == 0)
		{
			delete_inregistrare_with_value_struct(&DeleteHeapItem);
			continue;
		}
		if (strcmp(comanda, "MergeMinHeaps") == 0)
		{
			merge_with_compare_struct(&MergeMinHeaps);
			continue;
		}
		if (strcmp(comanda, "DeleteHeap") == 0)
		{
			delete_struct(&DeleteHeap);
			continue;
		}

		//operatii pentru BinarySearchTree
		if (strcmp(comanda, "CreateBST") == 0)
		{
			create_new_struct(&CreateBST, &DeleteBST);
			continue;
		}
		if (strcmp(comanda, "PrintBST") == 0)
		{
			afisare_struct(&PrintBST);
			continue;
		}
		if (strcmp(comanda, "PreorderBST") == 0)
		{
			afisare_struct(&PreorderBST);
			continue;
		}
		if (strcmp(comanda, "InorderBST") == 0)
		{
			afisare_struct(&InorderBST);
			continue;
		}
		if (strcmp(comanda, "PostorderBST") == 0)
		{
			afisare_struct(&PostorderBST);
			continue;
		}
		if (strcmp(comanda, "AddBSTItem") == 0)
		{
			adauga_inregistrare_comparare_struct(&AddBSTItem);
			continue;
		}
		if (strcmp(comanda, "SearchBSTItem") == 0)
		{
			cauta_inregistrare_return_value_struct(&SearchBSTItem);
			continue;
		}
		if (strcmp(comanda, "DeleteBSTItem") == 0)
		{
			delete_and_return_inregistrare_with_value_struct(&DeleteBSTItem);
			continue;
		}
		if (strcmp(comanda, "MergeBSTs") == 0)
		{
			merge_with_compare_struct(&MergeBSTs);
			continue;
		}
		if (strcmp(comanda, "HightBST") == 0)
		{
			find_height_of_element_struct(&HightBST);
			continue;
		}
		if (strcmp(comanda, "DeleteBST") == 0)
		{
			delete_struct(&DeleteBST);
			continue;
		}

		//operatii pe BalancedBST
		if (strcmp(comanda, "CreateBalancedBST") == 0)
		{
			create_new_struct(&CreateBalancedBST, DeleteBalancedBST);
			continue;
		}
		if (strcmp(comanda, "PrintBalancedBST") == 0)
		{
			afisare_struct(&PrintBalancedBST);
			continue;
		}
		if (strcmp(comanda, "PreorderBalancedBST") == 0)
		{
			afisare_struct(&PreorderBalancedBST);
			continue;
		}
		if (strcmp(comanda, "InorderBalancedBST") == 0)
		{
			afisare_struct(&InorderBalancedBST);
			continue;
		}
		if (strcmp(comanda, "PostorderBalancedBST") == 0)
		{
			afisare_struct(&PostorderBalancedBST);
			continue;
		}
		if (strcmp(comanda, "AddBalancedBSTItem") == 0)
		{
			adauga_inregistrare_comparare_struct(&AddBalancedBSTItem);
			continue;
		}
		if (strcmp(comanda, "SearchBalancedBSTItem") == 0)
		{
			cauta_inregistrare_return_value_struct(&SearchBalancedBSTItem);
			continue;
		}
		if (strcmp(comanda, "DeleteBalancedBSTItem") == 0)
		{
			delete_and_return_inregistrare_with_value_struct(&DeleteBalancedBSTItem);
			continue;
		}
		if (strcmp(comanda, "MergeBalancedBSTs") == 0)
		{
			merge_with_compare_struct(&MergeBalancedBSTs);
			continue;
		}
		if (strcmp(comanda, "HightBalancedBST") == 0)
		{
			find_height_of_element_struct(&HightBalancedBST);
			continue;
		}
		if (strcmp(comanda, "DeleteBalancedBST") == 0)
		{
			delete_struct(&DeleteBalancedBST);
			continue;
		}
		fprintf(p_out, "Error: Unrecognized command: %s\n", comanda);
	}

	fclose(p_in);
	fclose(p_out);
}

char * create_path_in(char * numar)
{
	char * path = (char *)calloc(20, sizeof(char));
	if (path == NULL)
	{
		return NULL;
	}
	strcat(path, "input\\");
	strcat(path, numar);
	int ok = 0;
	int i = 0;
	while (numar[i + 2] != '\0')//pentru situatile cand numar = ceva.in
	{
		if (numar[i] == '.' && numar[i + 1] == 'i' && numar[i + 2] == 'n')
		{
			ok = 1;
			break;
		}
		i++;
	}
	if (ok == 0)
	{
		strcat(path, ".in");
	}
	return path;
}

char * create_path_out(char * numar)
{
	char * path = (char *)calloc(20, sizeof(char));
	if (path == NULL)
	{
		return NULL;
	}
	strcat(path, "result\\");
	int i = 0;
	while (numar[i + 2] != '\0')//pentru situatii cand numar = ceva.in
	{
		if (numar[i] == '.' && numar[i + 1] == 'i' && numar[i + 2] == 'n')
		{
			numar[i] = '\0';
			break;
		}
		i++;
	}
	strcat(path, numar);
	strcat(path, ".res");

	return path;
}

void increase_path(char * numar)
{
	int lungime = strlen(numar);
	numar[lungime - 1]++;
	for (int i = lungime - 1; i > 0 ; i--)
	{
		if (numar[lungime - i] == 58)//57 = '9';
		{
			numar[lungime - i] = '0';
			if (i != lungime)
			{
				numar[lungime - i - 1]++;
			}
		}
	}
}

int ListDirectoryContents(const wchar_t *sDir, char ** sir, int * numar)
{
	HANDLE hFind = NULL;
	WIN32_FIND_DATA fdFile;

	wchar_t sPath[2048];

	//Specify a file mask. *.* = We want everything!
	wsprintf(sPath, L"%s\\*.*", sDir);

	// Find the first file
	if ((hFind = FindFirstFile((LPCWSTR)sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		wprintf(L"Path not found: [%s]\n", sPath);
		return 0;
	}

	//Find the next file
	int ok = 0;
	while (FindNextFile(hFind, &fdFile))
	{
		/*if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
		{
			wprintf(L"Directory: %s\n", fdFile.cFileName);
		}
		else
		{
			wprintf(L"File: %s\n", fdFile.cFileName);
		}*/
		if (ok != 0)
		{
			int i = 0;
			while (fdFile.cFileName[i] != '\0')
			{
				sir[*numar][i] = fdFile.cFileName[i] & 255;
				i++;
			}
			sir[*numar][i] = '\0';
			(*numar)++;
		}
		else
		{
			ok = 1;
		}
	}

	// Close the file handle
	FindClose(hFind);
	return 1;
}

//am presupus ca se pot introduce si 001 si 1
void creare_numar_string(char * sir, int numar)
{
	if (numar < 10)
	{
		sir[0] = '0';
		sir[1] = '0';
		sir[2] = '0' + numar;
	}
	else if (numar < 100)
	{
		sir[0] = '0';
		int p = numar / 10;
		sir[1] = '0' + p;
		numar -= p * 10;
		sir[2] = '0' + numar;
	}
	else if (numar < 1000)
	{
		int p = numar / 100;
		sir[0] = '0' + p;
		numar -= p * 100;
		p = numar / 10;
		sir[1] = '0' + p;
		numar -= p * 10;
		sir[2] = '0' + numar;
	}
	sir[3] = '\0';
}

void test_argument_main(char * arg, int *numar)
{
	int lungime = strlen(arg);
	int ok = 0;
	for (int i = 0; i < lungime; i++)
	{
		if (arg[i] < '0' || arg[i] > '9')
		{
			ok = 1;
			break;
		}
	}
	if (ok == 1)
	{
		fprintf(stderr, "'%s' invalid argument\n", arg);
		exit(2);
	}
	sscanf(arg, "%d", numar);
}

int main(int argc, char * argv[])
{
	if (argc > 3)
	{
		fprintf(stderr, "To many arguments\n");
		exit(1);
	}
	else if (strcmp(argv[1], "runall") == 0)
	{
		if (argc != 2)
		{
			fprintf(stderr, "To many arguments\n");
			exit(1);
		}
		char **sir = (char **)malloc(100 * sizeof(char *));
		if (sir == NULL)
		{
			fprintf(stderr, "ERROR: memory allocation fail\n");
			exit(1);
		}
		for (int i = 0; i < 100; i++)
		{
			sir[i] = (char *)malloc(100);
			if (sir[i] == NULL)
			{
				fprintf(stderr, "ERROR: memory allocation fail\n");
				exit(1);
			}
		}

		int numar = 0;
		ListDirectoryContents(L"input", sir, &numar);

		for (int i = 0; i < numar; i++)
		{
			char * path_in = create_path_in(sir[i]);
			char * path_out = create_path_out(sir[i]);
			printf("%s %s", path_in, path_out);
			process_file(path_in, path_out);
			free(path_in);
			free(path_out);
		}
	}
	else
	{
		int n1;
		int n2;
		test_argument_main(argv[1], &n1);
		n2 = n1;
		if (argc == 3)
		{
			test_argument_main(argv[2], &n2);
		}
		if (n1 > n2)
		{
			int aux = n1;
			n1 = n2;
			n2 = aux;
		}
		char arg1[4];
		char arg2[4];
		creare_numar_string(arg1, n1);
		creare_numar_string(arg2, n2);
		puts(arg1);
		puts(arg2);
		for (int i = n1; i <= n2; i++)
		{
			char * path_in = create_path_in(arg1);
			char * path_out = create_path_out(arg1);
		    process_file(path_in, path_out);
			free(path_in);
			free(path_out);
			increase_path(arg1);
		}
	}
	
	return 0;
}