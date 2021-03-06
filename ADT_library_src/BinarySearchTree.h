#ifndef BST
#define BST

typedef struct tree * BinarySearchTree;

/*
functie pentru crearea unui arbore binar de cautare .
returneaza : pointer la arborele creat ;
			 NULL daca nu s-a putut aloca memorie
*/
BinarySearchTree CreateBST();

/*
functie pentru afisarea unui arbore binar de cautare
parametrii : pointer la arbore
			 pointer la fisierul unde se doreste scrierea
			 pointer la o functie ce poate scrie un element din arbore intr-un fisier, parametrii : pointer la fisier
			                                                                                        pointer la element
returneaza : 0 daca arborele nu are nici un element
			 1 daca arborele are elemente
*/
int PrintBST(BinarySearchTree tree, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element));

/*
functie pentru afisare in preordine a unui arbor binar de cautare
parametrii : pointer la arbore
             pointer la fisierul unde se doreste scrierea
			 pointer la o functie ce poate scrie un element din arbore intr-un fisier, parametrii : pointer la fisier
			                                                                                        pointer la element
returneaza : 0 daca arborele nu are nici un element
			 1 daca arborele are elemente
*/
int PreorderBST(BinarySearchTree tree, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element));

/*
functie pentru afisare in inordine a unui arbor binar de cautare
parametrii : pointer la arbore
			 pointer la fisierul unde se doreste scrierea
			 pointer la o functie ce poate scrie un element din arbore intr-un fisier, parametrii : pointer la fisier
			                                                                                        pointer la element
returneaza : 0 daca arborele nu are nici un element
			 1 daca arborele are elemente
*/
int InorderBST(BinarySearchTree tree, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element));

/*
functie pentru afisare in postordine a unui arbor binar de cautare
parametrii : pointer la arbore
			 pointer la fisierul unde se doreste scrierea
			 pointer la o functie ce poate scrie un element din arbore intr-un fisier, parametrii : pointer la fisier
			                                                                                        pointer la element
returneaza : 0 daca arborele nu are nici un element
			 1 daca arborele are elemente
*/
int PostorderBST(BinarySearchTree tree, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element));

/*
functie pentru adaugarea unui element intr-un arbore binar de cautare.
parametrii : pointer la arbore
			 pointer la element
			 pointer la o functie ce poate compara 2 elemente din arbore, parametrii : pointer la primul element
			                                                                           pointer la al doilea element
			                                                              returneaza : 0-pentru egalitatea,
																		               1-daca primul este mai mare,
																					   -1- al doilea este mai mare
returneaza : 1 in caz de succes
			 0 daca nu s-a putut aloca memorie
*/
int AddBSTItem(BinarySearchTree tree, void * element, int(*comparare_elemente)(void * element1, void * element2));

/*
functie pentru cautarea unui element dupa valoare intr-un arbore binar de cautare
parametrii : pointer la arbore
			 pointer la element cautat
			 pointer la o functie ce poate compara 2 elemente din arbore, parametrii : pointer la primul element
			                                                                           pointer la al doilea element
																		  returneaza : 0-pentru egalitatea, 
																		               1-daca primul este mai mare,
																					   -1-al doilea este mai mare
returneaza : pointer la element daca acesta se gaseste in arbore
			 NULL daca nu apare
*/
void * SearchBSTItem(BinarySearchTree tree, void * element, int(*comparare_elemente)(void * element1, void * element2));

/*
functie pentru stergerea si returnarea unui element, dupa valoare, dintr-un arbore
parametrii : pointer la arbore
			 pointer la element cautat
pointer la o functie ce poate compara 2 elemente din arbore, parametrii : pointer la primul element
              			                                                  pointer la al doilea element
			                                                 returneaza : 0-pentru egalitatea,
			                                                              1-daca primul este mai mare,
			                                                             -1-al doilea este mai mare
returneaza : pointer la element daca acesta se gaseste in arbore
			 NULL daca nu apare
*/
void * DeleteBSTItem(BinarySearchTree tree, void * element, int(*comparare_elemente)(void * element1, void * element2));

/*
functie pentru combinarea a doi arbori binari de cautare
argumente : pointer la arbore destinatie
			pointer la arbore sursa
			pointer la o functie ce poate clona un element, parametrii : pointer la element
			                                                returneaza : pointer la clona elementului
															             NULL daca nu s-a alocat memorie
			pointer la o funtie ce poate compara 2 elemente , parametrii: pointer la primul element
																		  pointer la al doilea element
															  returneaza: -1 daca al doilea element este mai mare
																		  0 daca sunt egale
																		  1 daca primul element este mai mare
			pointer la o functie ce poate dezaloca elemente,  parametrii : pointer la element
Returneaza 1 in caz de succes
		   0 daca nu s-a putut aloca memorie, 
*/
int MergeBSTs(BinarySearchTree tree_destinatie, BinarySearchTree tree_sursa, void * (*copy_element)(void * element), int(*comparare_elemente)(void * element1, void * element2), void(*dezaloca_element)(void * element));

/*
functie pentru determinarea inaltimii subarborelui specificat unui arbore binar de cautare
parametrii : pointer la arbore
returneaza : inaltimea arborelui
             -1 daca arborele nu are elemente
*/
int HightBST(BinarySearchTree tree, void * element, int(*comparare_element)(void * element1, void * element2));

/*
functie pentru dezalocarea unui arbore binar de cautare
parametrii : pointer la arbore
			 pointer la o functie ce poate dezaloca un element din arbore
*/
void DeleteBST(BinarySearchTree tree, void(*dezaloca_element)(void * element));

#endif