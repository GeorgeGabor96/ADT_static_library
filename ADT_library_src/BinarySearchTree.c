#include <stdio.h>
#include <stdlib.h>
#include "BinarySearchTree.h"

typedef struct nod
{
	void * content;
	struct nod * left;
	struct nod * right;
}nod;

struct tree
{
	nod * root;
	int numar_elemente;
};


BinarySearchTree CreateBST()
{
	BinarySearchTree new_tree = (BinarySearchTree)malloc(sizeof(struct tree));
	if (new_tree == NULL)
	{
		return NULL;
	}

	new_tree->root = NULL;
	new_tree->numar_elemente = 0;

	return new_tree;
}


static void print_tree(nod * root, int level, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (root == NULL)
	{
		return;
	}
	print_tree(root->right, level + 1, pfile, afisare_element);
	if (root->right != NULL)
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
	afisare_element(pfile, root->content);
	fprintf(pfile, "\n");
	if (root->left != NULL)
	{
		for (int i = 0; i < (level + 1) * 10 - 3; i++)
		{
			fprintf(pfile, " ");
		}
		fprintf(pfile, "\\\n");
	}
	print_tree(root->left, level + 1, pfile, afisare_element);
}

int PrintBST(BinarySearchTree tree, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (tree->numar_elemente == 0)
	{
		return 0;
	}

	print_tree(tree->root, 0, pfile, afisare_element);
	fprintf(pfile, "\n");

	return 1;
}


static void afisare_preordine(nod * root, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (root == NULL)
	{
		return;
	}

	afisare_element(pfile, root->content);
	fprintf(pfile, "\n");
	afisare_preordine(root->left, pfile, afisare_element);
	afisare_preordine(root->right, pfile, afisare_element);
}

int PreorderBST(BinarySearchTree tree, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (tree->numar_elemente == 0)
	{
		return 0;
	}

	fprintf(pfile, "Preordine:\n");
	afisare_preordine(tree->root, pfile, afisare_element);

	return 1;
}


static void afisare_inordine(nod * root, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (root == NULL)
	{
		return;
	}

	afisare_inordine(root->left, pfile, afisare_element);
	afisare_element(pfile, root->content);
	fprintf(pfile, "\n");
	afisare_inordine(root->right, pfile, afisare_element);
}

int InorderBST(BinarySearchTree tree, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (tree->numar_elemente == 0)
	{
		return 0;
	}

	fprintf(pfile, "Inordine:\n");
	afisare_inordine(tree->root, pfile, afisare_element);

	return 1;
}


static void afisare_postordine(nod * root, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (root == NULL)
	{
		return;
	}

	afisare_postordine(root->left, pfile, afisare_element);
	afisare_postordine(root->right, pfile, afisare_element);
	afisare_element(pfile, root->content);
	fprintf(pfile, "\n");
}

int PostorderBST(BinarySearchTree tree, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (tree->numar_elemente == 0)
	{
		return 0;
	}

	fprintf(pfile, "Postordine:\n");
	afisare_postordine(tree->root, pfile, afisare_element);

	return 1;
}


inline static nod * aloca_nod(void * element)
{
	nod * new_nod = (nod *)calloc(1, sizeof(struct nod));
	if (new_nod == NULL)
	{
		return NULL;
	}

	new_nod->content = element;
	return new_nod;
}

static nod * adauga_nod(nod * root, nod * new_nod, int(*comparare_elemente)(void * element1, void * element2))
{
	if (root == NULL)
	{
		root = new_nod;
	}
	else if (comparare_elemente(new_nod->content, root->content) < 0)
	{
		root->left = adauga_nod(root->left, new_nod, comparare_elemente);
	}
	else
	{
		root->right = adauga_nod(root->right, new_nod, comparare_elemente);
	}
	return root;
}

int AddBSTItem(BinarySearchTree tree, void * element, int(*comparare_elemente)(void * element1, void * element2))
{
	if (element == NULL)
	{
		return 0;
	}

	nod * new_nod = aloca_nod(element);

	if (new_nod == NULL)
	{
		return 0;
	}

	tree->root = adauga_nod(tree->root, new_nod, comparare_elemente);
	tree->numar_elemente++;

	return 1;
}


static nod * cauta_nod(nod * root, void * element, int(*comparare_elemente)(void * element1, void * element2))
{
	if (root == NULL)
	{
		return NULL;
	}
	if (comparare_elemente(element, root->content) == 0)
	{
		return root;
	}
	else if (comparare_elemente(element, root->content) < 0)
	{
		return cauta_nod(root->left, element, comparare_elemente);
	}
	else
	{
		return cauta_nod(root->right, element, comparare_elemente);
	}
}

void * SearchBSTItem(BinarySearchTree tree, void * element, int(*comparare_elemente)(void * element1, void * element2))
{
	if (element == NULL)
	{
		return NULL;
	}

	nod * nod_cautat = cauta_nod(tree->root, element, comparare_elemente);
	if (nod_cautat == NULL)
	{
		return NULL;
	}

	return nod_cautat->content;
}


inline static nod * find_max(nod * root)
{
	if (root->right == NULL)
	{
		return root;
	}
	else
	{
		return find_max(root->right);
	}
}

static nod * sterge_nod(nod * root, void * element, nod ** element_sters, int(*comparare_elemente)(void * element1, void * element2))
{
	if (root == NULL)//elementul nu apare
	{
		return root;
	}
	else if (comparare_elemente(element, root->content) < 0)
	{
		root->left = sterge_nod(root->left, element, element_sters, comparare_elemente);
	}
	else if (comparare_elemente(element, root->content) > 0)
	{
		root->right = sterge_nod(root->right, element, element_sters, comparare_elemente);
	}
	else//am gasit elementul
	{
		if (root->left == NULL && root->right == NULL)//este frunza
		{
			*element_sters = root;
			root = NULL;
		}
		else if (root->left == NULL)
		{
			*element_sters = root;
			root = root->right;
		}
		else if (root->right == NULL)
		{
			*element_sters = root;
			root = root->left;
		}
		else
		{
			nod * auxiliar = find_max(root->left);
			void * temp = root->content;
			root->content = auxiliar->content;
			auxiliar->content = temp;
			root->left = sterge_nod(root->left, element, element_sters, comparare_elemente);//acum elementul are cel mult un fiu
		}
	}
	return root;
}

void * DeleteBSTItem(BinarySearchTree tree, void * element, int(*comparare_elemente)(void * element1, void * element2))
{
	if (element == NULL)
	{
		return NULL;
	}

	nod * nod_sters = NULL;
	tree->root = sterge_nod(tree->root, element, &nod_sters, comparare_elemente);

	if (nod_sters == NULL)
	{
		return NULL;
	}

	tree->numar_elemente--;
	void * element_sters = nod_sters->content;
	free(nod_sters);

	return element_sters;
}


static inline void adauga_noduri_arbore(BinarySearchTree tree, nod ** sir_noduri, int numar_elemente, int(*comparare_elemente)(void * element1, void * element2))
{
	for (int i = 0; i < numar_elemente; i++)
	{
		tree->root = adauga_nod(tree->root, sir_noduri[i], comparare_elemente);
	}
}

static int adauga_nod_sir(nod * root, nod ** sir_noduri, int * nr_elemente, void * (*copy_element)(void *))
{
	if (root == NULL)
	{
		return 1;
	}

	void * new_element = copy_element(root->content);
	if (new_element == NULL)
	{
		return 0;//un element nu s-a putut duplica
	}

	nod * new_nod = aloca_nod(new_element);
	if (new_nod == NULL)
	{
		return 0;//un nod nu s-a putut duplica
	}
	sir_noduri[(*nr_elemente)++] = new_nod;

	return adauga_nod_sir(root->left, sir_noduri, nr_elemente, copy_element) & adauga_nod_sir(root->right, sir_noduri, nr_elemente, copy_element);
}

static inline void sterge_sir(nod ** sir, int nr_elemente, void(*dezaloca_nod)(void * element))
{
	for (int i = 0; i < nr_elemente; i++)
	{
		dezaloca_nod(sir[i]->content);
		free(sir[i]);
	}
	free(sir);
}

int MergeBSTs(BinarySearchTree tree_destinatie, BinarySearchTree tree_sursa, void * (*copy_element)(void * element), int(*comparare_elemente)(void * element1, void * element2), void(*dezaloca_element)(void * element))
{
	nod ** sir_noduri = (nod **)malloc(sizeof(nod *) * tree_sursa->numar_elemente);//facem copii la nodurile din al doilea arbore
	if (sir_noduri == NULL)
	{
		return 0;
	}
	int nr_elemente_sir = 0;
	int ok = adauga_nod_sir(tree_sursa->root, sir_noduri, &nr_elemente_sir, copy_element);
	if (ok == 0)
	{
		sterge_sir(sir_noduri, nr_elemente_sir, dezaloca_element);
		return 0;//nu s-au copiat toate elementele
	}
	adauga_noduri_arbore(tree_destinatie, sir_noduri, tree_sursa->numar_elemente, comparare_elemente);
	free(sir_noduri);

	return 1;//succes
}


static int find_heigth(nod * root)
{
	if (root == NULL)
	{
		return -1;
	}

	int heigth_left = find_heigth(root->left);
	int heigth_right = find_heigth(root->right);
	return 1 + (heigth_left > heigth_right ? heigth_left : heigth_right);
}

int HightBST(BinarySearchTree tree, void * element, int(*comparare_element)(void * element1, void * element2))
{
	if (element == NULL)
	{
		return -1;
	}

	nod * nod_cautat = cauta_nod(tree->root, element, comparare_element);
	if (nod_cautat == NULL)
	{
		return -1;
	}
	return find_heigth(nod_cautat);
}


static void stergere_noduri(nod * root, void(*dezaloca_element)(void *))
{
	if (root == NULL)
	{
		return;
	}

	stergere_noduri(root->left, dezaloca_element);
	stergere_noduri(root->right, dezaloca_element);
	dezaloca_element(root->content);
	free(root);
}

void DeleteBST(BinarySearchTree tree, void(*dezaloca_element)(void * element))
{
	stergere_noduri(tree->root, dezaloca_element);
	
	free(tree);
}