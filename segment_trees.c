/**
 * Copyright 2021 - Dumitrescu Alexandra
 * grupa 323, seria CA, ACS UPB
 * Tema #1 - RMQ - Range Minimum Query
 * Noiembrie 2021
 */

/**
 * SEGMENT TREES
 */

#include <stdio.h>
#include <stdlib.h>
#include "math_utils.h"
#define MAX 999999999
#define GO_LEFT(x) (x * 2 + 1)
#define GO_RIGHT(x) (x * 2 + 2)
#define GO_UP(x) ((x - 1) / 2)

/**
 * PREPROCESARE:
 *
 *	1.	Algoritmul se bazeaza pe folosirea arborilor de intervale pentru
 *		a preprocesa datele de intrare.
 *
 *		1.1 Arborele de intervale va contine in frunze valoarea
 *			fiecarui element din vector, iar fiecare nod intern
 *			retine minimul pentru toate frunzele descendente.
 *
 *		1.2	Astfel, fiecare nod intern va codifica de fapt minimul pentru un
 *			subinterval.													(1)
 *
 *		1.3 Arborele binar va fi retinut sub forma unui arbore binar		(2)
 *			complet pentru a facilita reprezentarea sa printr-un vector si
 *			implicit accesul direct al fiilor, si al parintelui. Pentru
 *			acestea am definit MACRO-uri. Dimensiunea sa va fi de
 *			(2 * 2^(log_2(N) + 1) - 1) elemente.
 *
 *
 *	CONCLUZII:
 *	1. Memoria auxiliara necesara este de un vector de
 *						(2 * 2^(log_2(N) + 1) - 1) elemente.
 *
 */

/**
 * @_recursive_preprocess()
 *
 * @param start - Index de start pentru subintervalul din vectorul de date
 *				de intrare
 * @param end - Index final pentru subintervalul din vectorul de date
 *				de intrare
 * @param idx - Index curent in arbore
 * @param v - Date de intrare
 * @param segment_tree - Arborerle binar de intervale
 *
 *		1. Constructia arborelui este recursiva.
 *
 *			1.1	Conditia de oprire este ca subintervalul la care ajungem
 *				sa fie de dimensiune 1. Astfel, am ajuns la o frunza si
 *				retinem valoarea din vector.
 *
 *			1.2	Pentru un nod intern, valoarea pe care o va retine este
 *				minimul dintre valoarea celor 2 copii.
 *
 *			1.3 Precum am zis la (1) fiecare nod codifica minimul pentru	(3)
 *				un subinterval. Tinand cont de 1.2, un nod intern ce
 *				codifica minimul pentru intervalul (xi, yi) va fi calculat
 *				prin minimul dintre copiii sai ce codifica minimul
 *				pentru (xi, mijloc_interval) si (mijloc_interval + 1, end)
 */

void _recursive_preprocess(int start, int end, int idx,
							int *v, int **segment_tree)
{
	if (start == end)
		(*segment_tree)[idx] = v[start];
	else {
		int middle = (start + end)/2;
		/* (3) */
		_recursive_preprocess(start, middle, GO_LEFT(idx), v, segment_tree);
		_recursive_preprocess(middle + 1, end, GO_RIGHT(idx), v, segment_tree);
		int left_child = (*segment_tree)[GO_LEFT(idx)];
		int right_child = (*segment_tree)[GO_RIGHT(idx)];
		(*segment_tree)[idx] = get_min(left_child, right_child);
	}
}

void preprocess(int N, int *v, int **segment_tree, int *dim)
{
	(*dim) = 2 * (1 << (log_2(N) + 1)) - 1;	/* (2) */
	*segment_tree = malloc((*dim) * sizeof(int));
	_recursive_preprocess(0, N - 1, 0, v, segment_tree);
}


/**
 * INTEROGARI:
 *
 *		1.	La fiecare pas verificam daca intervalul interogat se afla
 *			in intervalul codificat de nodul respectiv.
 *
 *		2.	Se disting 3 cazuri:
 *			A.	Intervalul este cuprins complet in interval codificat
 *				de nodul curent	caz in care se returneaza valoarea nodului. (A)
 *
 *			B.	Intervalul este in afara intervalului codificat, caz
 *				in care se returneaza o valoare MAX							(B)
 *
 *			C.	Intervalul este cuprins partial in intervalul codificat de nod,
 *				caz in care se reapeleaza recursiv pentru cautarea minimului
 *				din intervalele [left, middle] si [middle + 1, right]		(C)
 *
 */
short in_interval(int left_q, int right_q, int left, int right)		/* (A) */
{
	if (left_q <= left && right <= right_q)
		return 1;
	return 0;
}

short out_interval(int left_q, int right_q, int left, int right)	/* (B) */
{
	if (left_q > right || right_q < left)
		return 1;
	return 0;
}

int rmq(int N, int *v, int *segment_tree, int left_query,
	 int right_query, int left, int right, int idx)
{
	if (in_interval(left_query, right_query, left, right))
		return segment_tree[idx];
	if (out_interval(left_query, right_query, left, right))
		return MAX;

	int middle, left_child, right_child;

	middle = left + (right - left)/2;
	left_child = rmq(N, v, segment_tree, left_query, right_query,
				left, middle, GO_LEFT(idx));
	right_child = rmq(N, v, segment_tree, left_query, right_query,
				middle + 1, right, GO_RIGHT(idx));
	return get_min(left_child, right_child);
}

/**
 * @get_data
 *
 * Functie ce aloca memorie & citeste datele din fisierul
 * de intrare test.in
 */
void get_data(int *N, int *M, int **v, FILE *in)
{
	fscanf(in, "%d", N);
	fscanf(in, "%d", M);

	(*v) = malloc((*N) * sizeof(**v));

	for (int i = 0; i < *N; i++)
		fscanf(in, "%d", &((*v)[i]));
}

/**
 * @get_queries()
 *
 * Functie ce citeste si raspunde la interogari
 *
 */
void get_query(int N, int M, int *v, int *segment_tree, FILE *in)
{
	int left_bound, right_bound;

	FILE *out = fopen("test.out", "wt");

	for (int nr_query = 0; nr_query < M; nr_query++) {
		fscanf(in, "%d %d", &left_bound, &right_bound);
		int rmq_answ = rmq(N, v, segment_tree, left_bound - 1,
						 right_bound - 1, 0, N - 1, 0);
		fprintf(out, "%d\n", rmq_answ);
	}

	fclose(in);
	fclose(out);
}

int main(void)
{
	int N, M, *v = NULL, *segment_tree = NULL, dim;

	FILE *in = fopen("test.in", "rt");

	if (in == NULL)
		printf("error");

	get_data(&N, &M, &v, in);
	preprocess(N, v, &segment_tree, &dim);
	get_query(N, M, v, segment_tree, in);

	free(segment_tree);
	free(v);
}
