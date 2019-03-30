#include <stdio.h>
#include <stdlib.h>
#include "BalancedBST.h"

//implementare cu Arbore AA

typedef struct nod
{
	struct nod * left;
	struct nod * right;
	int level;
	void * content;
}nod;

struct Bbst
{
	nod * root;
	int nr_elemente;
};


BalancedBST CreateBalancedBST()
{
	BalancedBST new_tree = (BalancedBST)malloc(sizeof(struct Bbst));
	if (new_tree == NULL)
	{
		return NULL;
	}

	new_tree->nr_elemente = 0;
	new_tree->root = NULL;

	return new_tree;
}


static void print_tree(nod * root, int level, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (root == NULL)
	{
		return;
	}
	if (root->right != NULL && root->level == root->right->level)
	{
		print_tree(root->right, level, pfile, afisare_element);
	}
	else
	{
		print_tree(root->right, level + 1, pfile, afisare_element);
	}

	if (root->right != NULL)
	{
		for (int i = 0; i < (level + 1) * 10 - 3; i++)
		{
			fprintf(pfile, " ");
		}
		if (root->right != NULL && root->level == root->right->level)
		{
			fprintf(pfile, "|\n");
		}
		else
		{
			fprintf(pfile, "/\n");
		}
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

int PrintBalancedBST(BalancedBST tree, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (tree->nr_elemente == 0)
	{
		return 0;
	}
	print_tree(tree->root, 0, pfile, afisare_element);

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

int PreorderBalancedBST(BalancedBST tree, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (tree->nr_elemente == 0)
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

int InorderBalancedBST(BalancedBST tree, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (tree->nr_elemente == 0)
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

int PostorderBalancedBST(BalancedBST tree, FILE * pfile, void(*afisare_element)(FILE * pfile, void * element))
{
	if (tree->nr_elemente == 0)
	{
		return 0;
	}
	fprintf(pfile, "Postordine:\n");
	afisare_postordine(tree->root, pfile, afisare_element);
	return 1;
}


static inline nod * aloca_nod(void * element)
{
	nod * new_nod = (nod *)malloc(sizeof(nod));
	if (new_nod == NULL)
	{
		return NULL;
	}

	new_nod->left = NULL;
	new_nod->right = NULL;
	new_nod->level = 1;//presupunem ca va fi frunza
	new_nod->content = element;

	return new_nod;
}

static inline nod * spargere(nod * root)
{
	if (root == NULL || root->right == NULL || root->right->right == NULL)
	{
		return root;
	}
	else if (root->level == root->right->right->level)
	{
		nod * aux = root->right;
		root->right = aux->left;
		aux->left = root;
		aux->level++;
		return aux;
	}
	else
	{
		return root;
	}
}

static inline nod * inclinare(nod * root)
{
	if (root == NULL || root->left == NULL)
	{
		return root;
	}
	else if (root->level == root->left->level)
	{
		//nodul din stanga parintelui are nivelul mai mic cu unu
		nod * aux = root->left;
		root->left = aux->right;
		aux->right = root;
		return aux;
	}
	else
	{
		return root;
	}
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
	root = inclinare(root);
	root = spargere(root);
	return root;
}

int AddBalancedBSTItem(BalancedBST tree, void * element, int(*comparare_elemente)(void * element1, void * element2))
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
	tree->nr_elemente++;
	return 1;
}


static nod * cauta_nod(nod * root, void * element, int(*comparare_elemente)(void * element1, void * element2))
{
	if (root == NULL)
	{
		return NULL;
	}

	int ok = comparare_elemente(element, root->content);
	if (ok == 0)
	{
		return root;
	}
	else if (ok < 0)
	{
		return cauta_nod(root->left, element, comparare_elemente);
	}
	else
	{
		return cauta_nod(root->right, element, comparare_elemente);
	}
}

void * SearchBalancedBSTItem(BalancedBST tree, void * element, int(*comparare_elemente)(void * element1, void * element2))
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


static inline void swap_content(nod * nod1, nod * nod2)
{
	void * aux = nod1->content;
	nod1->content = nod2->content;
	nod2->content = aux;
}

static inline int get_level(nod * root)
{
	if (root == NULL)
	{
		return 0;
	}
	else
	{
		return root->level;
	}
}

static inline void decrease_level(nod * root)
{
	if (root == NULL)
	{
		return;
	}
	int level_left = get_level(root->left);
	int level_right = get_level(root->right);
	int level = (level_left > level_right ? level_right : level_left) + 1;
	if (level < root->level)
	{
		root->level = level;
		if (level < level_right)
		{
			root->right->level = level;
		}
	}
}

static void * find_max(nod * root)
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

static nod * sterge_nod(nod * root, void * element, nod ** nod_sters, int(*comparare_elemente)(void * element1, void * element2))
{
	if (root == NULL)
	{
		return NULL;
	}

	int ok = comparare_elemente(element, root->content);
	if (ok < 0)
	{
		root->left = sterge_nod(root->left, element, nod_sters, comparare_elemente);
	}
	else if (ok > 0)
	{
		root->right = sterge_nod(root->right, element, nod_sters, comparare_elemente);
	}
	else
	{
		//am gasit elementul
		if (root->left == NULL && root->right == NULL)
		{
			*nod_sters = root;
			return NULL;
		}
		if (root->left == NULL)
		{
			*nod_sters = root;
			root = root->right;
		}
		else if (root->right == NULL)
		{
			*nod_sters = root;
			root = root->left;
		}
		else
		{
			nod * auxiliar = find_max(root->left);
			swap_content(auxiliar, root);
			root->left = sterge_nod(root->left, element, nod_sters, comparare_elemente);
		}
	}
	//reechilibrare
	decrease_level(root);
	root = inclinare(root);
	root->right = inclinare(root->right);
	if (root->right != NULL)
	{
		root->right->right = inclinare(root->right->right);
	}
	root = spargere(root);
	root->right = spargere(root->right);
	return root;
}

void * DeleteBalancedBSTItem(BalancedBST tree, void * element, int(*comparare_elemente)(void * element1, void * element2))
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

	tree->nr_elemente--;
	void * element_sters = nod_sters->content;
	free(nod_sters);

	return element_sters;
}


static inline void adauga_noduri_arbore(BalancedBST tree, nod ** sir_noduri, int numar_elemente, int(*comparare_elemente)(void * element1, void * element2))
{
	for (int i = 0; i < numar_elemente; i++)
	{
		tree->root = adauga_nod(tree->root, sir_noduri[i], comparare_elemente);
	}
}

static int adauga_nod_sir(nod * root, nod ** sir_noduri, int * nr_elemente, void * (*copy_element)(void * element))
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

static void sterge_sir(nod ** sir, int nr_elemente, void(*dezaloca_nod)(void * element))
{
	for (int i = 0; i < nr_elemente; i++)
	{
		dezaloca_nod(sir[i]->content);
		free(sir[i]);
	}
	free(sir);
}

int MergeBalancedBSTs(BalancedBST tree_destinatie, BalancedBST tree_sursa, void * (*copy_element)(void * element), int(*comparare_elemente)(void * element1, void * element2), void(*dezaloca_element)(void * element))
{
	nod ** sir_noduri = (nod **)malloc(sizeof(nod *) * tree_sursa->nr_elemente);//facem copii la nodurile din al doilea arbore
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
	adauga_noduri_arbore(tree_destinatie, sir_noduri, tree_sursa->nr_elemente, comparare_elemente);
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

int HightBalancedBST(BalancedBST tree, void * element, int(*comparare_elemnte)(void * element1, void * element2))
{
	if (element == NULL)
	{
		return -1;
	}
	nod * nod_cautat = cauta_nod(tree->root, element, comparare_elemnte);
	if (nod_cautat == NULL)
	{
		return -1;
	}

	return find_heigth(nod_cautat);
}


static void stergere_noduri(nod * root, void(*dezaloca_element)(void * element))
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

void DeleteBalancedBST(BalancedBST tree, void(*dezaloca_element)(void * element))
{
	stergere_noduri(tree->root, dezaloca_element);
	free(tree);
}






















