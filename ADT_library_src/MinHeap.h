#ifndef MIN
#define MIN

typedef struct Min_Heap_GEN * Heap;

/*
functie pentru alocarea unui Heap .
returneaza : pointer la heap-ul creat 
			 NULL daca nu s-a putut aloca memorie
*/
Heap CreateHeap();

/*
functie pentru scierea unui heap intr - un fisier.
parametrii : pointer la heap
             pointer la fisierul unde se va scrie
			 pointer la o functie ce stie sa tipareasca un element din heap in fisier, parametrii : pointer la un fisier
			                                                                                        pointer la un element
returneaza : 1 in caz de succes
             0 daca vectorul este gol
*/
int PrintHeap(Heap heap, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element));

/*
functie pentru adaugarea unui element intr-un heap.
parametrii : pointer la heap
			 pointer la element
			 pointer la o funtie ce poate compara 2 elemente , parametrii: pointer la primul element
																		   pointer la al doilea element
															   returneaza: -1 daca al doilea element este mai mare
															               0 daca sunt egale
																		   1 daca primul element este mai mare
returneaza 1 in caz de succes
           0 daca nu s-a putut aloca memorie
*/
int AddHeapItem(Heap heap, void * element, int(*comparare_elemente)(void *element1, void *element2));

/*
functie pentru returneaza elementului minim dintr-un heap
argumente : pointer la heap
returneaza : pointer la element minim
			 pointer la NULL daca heap-ul este gol
*/
void * GetHeapMin(Heap heap);

/*
functie pentru stergerea sir returnarea elementului minim dintr-un heap
parametrii : pointer la heap
			 pointer la o functie care poate sa compare 2 elemente din heap
returneaza : pointer la elementul minim
             pointer la NULL daca heap-ul este gol
*/
void * DeleteHeapMin(Heap heap, int(*comparare_elemente)(void * element1, void * element2));

/*
functie pentru stergerea dupa valoare a unui element din heap
parametrii : pointer la heap
			 pointer la un element identic cu cel ce il dorim sters
			 pointer la o functie ce poate dezaloca un element, parametrii : pointer la element
pointer la o funtie ce poate compara 2 elemente , parametrii: pointer la primul element
								                              pointer la al doilea element
												  returneaza: -1 daca al doilea element este mai mare
															  0 daca sunt egale
															  1 daca primul element este mai mare
returneaza : 1 pentru succes
			 0 daca elementul nu a fost gasit
*/
int DeleteHeapItem(Heap heap, void * element, void(*dezaloca_element)(void * element), int(*comparare_elemente)(void *element1, void *element2));

/*
functie pentru combinarea a 2 heap-uri
argumente : pointer la heap destinatie
			pointer la heap sursa
			pointer la o functie ce poate clona un element, parametrii : pointer la element
			                                                returneaza : pointer la clona elementului
															             NULL daca nu s-a alocat memorie
			pointer la o funtie ce poate compara 2 elemente , parametrii: pointer la primul element
																		  pointer la al doilea element
															  returneaza: -1 daca al doilea element este mai mare
																		  0 daca sunt egale
																		  1 daca primul element este mai mare
			pointer la o functie ce poate dezaloca elemente,  parametrii : pointer la element
Returneaza : 1 in caz de succes
             0 daca nu s-a putut aloca memorie,
*/
int MergeMinHeaps(Heap heap_destinatie, Heap heap_sursa, void * (*copy_element)(void * element), int(*comparare_elemente)(void *element1, void *element2), void(*dezaloca_element)(void *element));

/*
functie pentru stergerea unui heap
parametrii : pointer la heap
			 pointer la o functie ce poate sterge un element, parametrii : pointer la element
functia nu returneaza numic
*/
void DeleteHeap(Heap heap, void(*dezaloca_element)(void * element));

#endif
