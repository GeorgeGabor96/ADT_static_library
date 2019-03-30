#ifndef HASH
#define HASH

typedef struct htable * HashTable;

/*
	functie pentru crearea unei tabele de dispersie
	parametrii : pointer la o functie de hash corespunzatoare tipului de data ce se va introduce(daca se transmite null se fa folosi o functie implicita)
	             numarul de octeti a tipului de data
	returneaza : pointer la o noua tabela de dispersie
	             NULL daca nu s-a putut crea tabela
*/
HashTable CreateHashTable(int(*hash_function)(void * element, int numar_elemente), int numar_octeti);

/*
	functie pentru afisarea unei tabele de dispersie
	parametrii : pointer la tabela
                 pointer la fisierul unde se va scrie
			     pointer la o functie ce stie sa tipareasca un element din tabela in fisier, parametrii : pointer la un fisier
			                                                                                              pointer la un element
    returneaza : 1 in caz de succes
                 0 daca tabela nu are nici un element
*/
int PrintHashTable(HashTable table, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element));

/*
functie pentru adaugarea unui element intr-o tabela de dispersie
parametrii : pointer la tabela
			 pointer la elementul ce trebuie inserat
returneaza : 1 daca s-a adaugat cu succes
             0 nu s-a putut aloca memorie
*/
int AddHashTableItem(HashTable table, void * element);

/*
functie pentru stergerea dupa valoare a unui element dintr-o tabela de dispersie
parametrii : pointer la tabela
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
int DeleteHashTableItem(HashTable table, void * element, void(*dezaloca_element)(void * element), int(*comparare_element)(void * element1, void * element2));

/*
functie pentru determinarea daca un element se afla intr-o tabela de dispersie
parametrii : pointer la tabela
             pointer la un element cu valori egale ale celui cautat
             pointer la o functie ce poate compara 2 elemente de acelasi fel, argumente : pointer la primul element
                                                                                          pointer la al doilea element
                                                                              returneaza : -1 daca al doilea este mai mare decat primul
                                                                                           0 daca sunt egale
                                                                                           1 daca primul este mai mare decat al doilea
se returneaza : indexul la care se afla eleementul
                -1 daca nu a fost gasit elementul
*/
int SearchHashTableItem(HashTable table, void * element, int(*comparare_elemente)(void * element1, void * element2));

/*
	functie pentru redistribuirea elementelor unei tabele de dispersie pe baza unei noi functii de has
	parametrii : pointer la o tabela
	             pointer la noua functie de has(daca se introduce NULL se foloseste o functia default)
	returneaza : pointer la tabela modificata in caz de succes
	             pointer la aceeasi tabela in caz de esec
*/
void * ReHashTable(HashTable table, int(*hash_function)(void * element, int numar_elemente));

/*
	functie pentru stergerea unei tabele de dispersie
	parametrii : pointer la tabela de dipersie
				 pointer la o functie ce poate sterga un element, parametrii : pointer la element
	functia nu returneaza numic
*/
void DeleteHashTable(HashTable table, void(*dezaloca_element)(void * element));

#endif