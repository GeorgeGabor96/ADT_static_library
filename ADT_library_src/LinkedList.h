#ifndef LIS
#define LIS

typedef struct LISTA_GEN * LinkedList;

/*
functie pentru alocarea unei liste . 
se returneaza : pointer la lista creata ;
				NULL daca nu s-a putut aloca memorie
*/
LinkedList CreateLinkedList();

/*
functie pentru scierea unue liste intr-un fisier.
parametrii : pointer la lista, 
			 pointer la fisierul unde se va scrie 
			 pointer la o functie ce stie sa tipareasca un element din vector in fisier, parametrii : un pointer la fisier
																									  pointer la un element din vector
returneaza: 1 in caz de succes
            0 daca lista nu are nici un element
*/
int PrintLinkedList(LinkedList list, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element));

/*
functie pentru adaugarea unui element la finalul unei liste.
parametrii : pointer la lista
             pointer la element
returneaza: 1 in caz de succes
            0 daca nu s-a putut aloca memorie
*/
int AddLinkedListItem(LinkedList , void *);

/*
functie pentru adaugarea unui element la un index intr-o lista
parametrii : pointer la lista
			 pointer la elementul ce trebuie inserat
			 indexul la care trebuie adaugat
returneaza : 1 daca s-a adaugat cu succes
             0 nu s-a putut aloca memorie
			 -1 index invalid
*/
int PutLinkedListItem(LinkedList list, void * element, int index);

/*
functie pentru determinare elementului de la un index in lista
parametrii : pointer la lista
			 un index
returneaza : pointer la elementul de pe indexul dat in caz de succes
			 NULL daca indexul este invalid
*/
void * GetLinkedListItem(LinkedList list, int index);

/*
functie pentru stergerea din lista a unui element dupa valoare si returnarea lui
parametrii : pointer la lista
	         pointer la un element cu valoare identica cu cel cautat
			 pointer la o funtie ce poate compara 2 elemente , parametrii: pointer la primul element
																		   pointer la al doilea element
															   returneaza: -1 daca al doilea element este mai mare
															               0 daca sunt egale
																		   1 daca primul element este mai mare
returneaza: pointer la elementul eliminat
            NULL daca acesta nu a fost gasit
*/
void * DeleteLinkedListItem(LinkedList list, void * element, int(*comparare_element)(void * element1, void * element2));

/*
functie pentru cautarea dupa valoarea a unui element in lista
parametrii : pointer la lista
			 pointer la un element cu valori egale ale celui cautat
			 pointer la o funtie ce poate compara 2 elemente , parametrii: pointer la primul element
			  														       pointer la al doilea element
															   returneaza: -1 daca al doilea element este mai mare
															               0 daca sunt egale
																		   1 daca primul element este mai mare
returneaza : pointer la elementul cautat
			 NULL daca nu s-a regasit in lista
*/
void * SearchLinkedListItem(LinkedList list, void * element, int(*comparare_element)(void * element1, void * element2));

/*
functie pentru ordonarea elementelor unei liste
parametrii : pointer la lista
			 pointer la o funtie ce poate compara 2 elemente , parametrii: pointer la primul element
																		   pointer la al doilea element
															   returneaza: -1 daca al doilea element este mai mare
																		   0 daca sunt egale
																		   1 daca primul element este mai mare
nu se returneaza nimic
*/
void SortLinkedList(LinkedList list, int(*comparare_element)(void * element1, void * element2));

/*
functie care adauga in lista destinatie elementele din lista sursa , se vor adauga la final
parametrii : pointer la lista destinatie
			 pointer la lista sursa
			 pointer la o functie ce poate copia un element, parametrii : pointer la element
			                                                 returneaza : pointer la copia element
															              NULL daca nu s-a putut aloca memorie
		     pointer la o functie ce poate sterge un element, parametrii : pointer la element
returneaza : 1 in caz de succes
			 0 daca nu s-a putut aloca memorie
*/
int MergeLinkedLists(LinkedList list_destinatie, LinkedList list_sursa, void * (*copy)(void * element), void(*dezaloca_element)(void * element));

/*
functie pentru stergerea unei liste
parametrii : pointer la lista
             pointer la o functie ce poate sterge un element : parametrii : pointer la element
functia nu returneaza nimic
*/
void DeleteLinkedList(LinkedList list, void(*dezaloca_element)(void * element));

int PutLinkedListPush(LinkedList, void *);

void * DeleteLinkedListPop(LinkedList);
#endif