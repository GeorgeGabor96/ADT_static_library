#ifndef VEC
#define VEC

typedef struct VECTOR_GEN * Vector;

/*
functie pentru alocarea unui vector .
se returneaza : pointer la vectorul creat ;
				NULL daca nu s-a putut aloca memorie
*/
Vector CreateVector();

/*
functie pentru scierea unui vector intr-un fisier.
parametrii : pointer la vector
			 pointer la fisierul de scris
			 pointer la o functie ce stie sa tipareasca un element din vector in fisier, parametrii : un pointer la fisier
																								      pointer la un element din vector
returneaza : 1 in caz de succes
			 0 daca vectorul este gol
*/
int PrintVector(Vector, FILE *, void(*afisare_element)(FILE * pfile, void * element));

/*
functie pentru adaugarea unui numar de elemente la finalul unui vector.
parametrii : pointer la vector
             pointer la sir de elemente
             numarul de elemente
returneaza : 1 in caz de succes
             0 daca nu s-a putut aloca memorie
*/
int AddVectorItems(Vector table, void ** new_elements, int nr_elements);

/*
functie pentru adaugarea unui element la un index intr-un vector
parametrii : pointer la vector
             pointer la elementul ce trebuie inserat
             indexul la care trebuie adaugat
returneaza : 1 daca s-a adaugat cu succes
             0 nu s-a putut aloca memorie
             -1 index invalid
*/
int PutVectorItem(Vector table, void * element, int index);

/*
functie pentru determinare elementului de la un index in vector
parametrii : pointer la vector
			 un index
se returneaza : pointer la elementul de pe indexul dat in caz de succes
				NULL daca indexul este invalid
*/
void * GetVectorItem(Vector table, int index);

/*
functie pentru stergerea unui element de la un index dat
parametrii: pointer la vector
			indexul de la care treguie sters
			pointer la o functie ce poate dezaloca elementul, argumente : pointer la un element
returneaza: 1 in caz de succes
			0 daca indexul nu este valid
*/
int DeleteVectorItem(Vector table, int index, void(*dezaloca_element)(void * element));

/*
functie pentru determinarea indexului unui element in vector
parametrii : pointer la vector
			 pointer la un element cu valori egale ale celui cautat
	   		 pointer la o functie ce poate compara 2 elemente de acelasi fel, argumente : pointer la primul element
																						  pointer la al doilea element
																			  returneaza : -1 daca al doilea este mai mare decat primul
																						   0 daca sunt egale
																						   1 daca primul este mai mare decat al doilea
se returneaza : valoarea indexului in caz de succes
				-1 daca nu a fost gasit elementul
*/
int SearchVectorItem(Vector table, void * element, int(*comparare_elemente)(void * element1, void * element2));

/*
functie pentru ordonarea elementelor unui vector
parametrii : pointer la vector
			 pointer la o functie ce poate compara 2 elemente de acelasi fel, parametrii: pointer la primul element
																					   	  pointer la al doilea element
																						  returneaza: -1 daca al doilea este mai mare decat primul
																						  0 daca sunt egale
																						  1 daca primul este mai mare dezat al doilea
nu se returneaza nimic
*/
void SortVector(Vector table, int(*comparare_elemente)(void * element1, void * element2));

/*
functie care adauga in vectorul destinatie elementele din vectorul sursa , se vor adauga la final
parametrii : pointer la vectorul destinatie
			 pointer la vectorul sursa
			 pointer la o functie ce poate face o copie a unui element, parametrii : pointer la element
																		returneaza : pointer la copia elementului
																	    NULL daca nu s-a putut aloca memorie
			 pointer la o functie care poate sterge un element, parametrii : pointer la element
se returneaza : 1 in caz de succes
				0 daca nu s-a putut aloca memorie
*/
int MergeVectors(Vector table_destinatie, Vector table_sursa, void * (*copy)(void * element), void(*dezaloca_element)(void * element));

/*
functie pentru stergerea unui vector
parametrii : pointer la vector
			 pointer la o functie ce poate sterga un element, parametrii : pointer la element
functia nu returneaza numic
*/
void DeleteVector(Vector table, void(*dezaloca_element)(void * element));

#endif