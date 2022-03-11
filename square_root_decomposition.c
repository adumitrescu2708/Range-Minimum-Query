/**
 * Copyright 2021 - Dumitrescu Alexandra
 * grupa 323, seria CA, ACS UPB
 * Tema #1 - RMQ - Range Minimum Query
 * Noiembrie 2021
 */

/**
 * SQUARE ROOT DECOMPOSITION
 */

#include <stdio.h>
#include <stdlib.h>
#include "math_utils.h"

/**
 *	PREPROCESARE:
 *
 *	1. Ideea ce sta la baza preprocesarii este impartirea
 *	vectorului initial in sqrt(N) blocuri a cate sqrt(N)				(1)
 *	elemente. Pentru fiecare bloc, vom retine intr-un vector
 *	auxiliar minimul sau, iar interogarile se vor reduce la
 *	folosirea minimelor blocurilor.
 *
 *		1.1 In cazul in care numarul N nu este patrat perfect
 *			vom imparti vectorul initial in sqrt(N) + 1 blocuri			(2)
 *
 *		1.2 Am implementat functii matematice auxiliare.
 *			Pentru mai multe detalii: vezi si <math_utils.c>
 *
 *	CONCLUZII:
 *	1. Memoria auxiliara necesara: vector de ceil(sqrt(N)) elemente
 *
 */

/**
 * @preprocess()
 *
 * @param N - Numarul de elemente
 * @param v - Datele de intrare pe care vrem sa le preprocesam
 * @param preprocessed_data - Vector ce va contine elementele preprocesate
 *
 *
 *	1. Se aloca memoria necesara pentru vectorul de preprocesare.
 *
 *	2. Parcurgem vectorul cu datele de intrare si calculam un minim
 *	local pentru fiecare bloc. O data ce ajungem la capatul unui bloc,
 *	adaugam minimul in vector si resetam minimul.						(3)
 *
 *	3. In cazul in care N nu este patrat perfect, trebuie sa mai adaugam
 *	minimul si pentru al (sqrt(N) + 1) - lea bloc.						(4)
 *
 */

void preprocess(int N, int *v, int **preprocessed_data)
{
	int block_size = square_root(N);	/* (1) */
	int block_count = square_root(N);

	if (!is_perfect_square(N))	/* (2) */
		block_count++;

	(*preprocessed_data) = malloc(block_count * sizeof(**preprocessed_data));

	int block_minimum, i, block_idx;

	for (i = 0, block_idx = 0; i < N; i++) {
		if (i % block_size == 0) {
			if (i != 0) {	/* (3) */
				(*preprocessed_data)[block_idx] = block_minimum;
				block_idx++;
			}
			block_minimum = v[i];
		}
		if (v[i] < block_minimum)
			block_minimum = v[i];
	}

	if (!is_perfect_square(N) && block_idx < block_count)	/* (4) */
		(*preprocessed_data)[block_idx] = block_minimum;
}


/**
 *	INTEROGARI:
 *
 *	1.	Fiecare interogare se rezolva prin gasirea minimului dintre toate
 *		blocurile intregi continute in intervalul dat.
 *
 *		1.1	Blocurile vor corespunde pozitiilor:
 *			[(left_idx / sqrt(N)), (right_idx / sqrt(N))]
 *
 *	2.	Pot exista situatii in care blocurile de inceput nu sunt continute
 *		complet, ci trunchiat. In aceasta situatie, parcurgem liniar si cautam
 *		minimul in cele 2 blocuri trunchiate.
 */

/**
 * @rmq
 *
 * @param left - Indexul stang al intervalului de cautare
 * @param right - Indexul drept al intervalului de cautare
 * @param N - Numarul de elemente
 * @param v - Datele de intrare
 * @param preprocessed_data - Datele preprocesate
 * @return - raspunsul interogarii - elementul minim cuprins
 *					in intervalul [@param1, @param2]
 *
 *		Raspunsul la interogare va fi retinut in rmq_ans si va fi returnat
 *		de functie la sfarsitul cautarii.
 *
 *
 *	1.	Indexarea in interogari este perceputa de la 1 la N, deci
 *		scadem o unitate din @param1, @param2.							(5)
 *
 *	2.	Daca intervalul de cautare contine un singur element, returnam
 *		respectivul element.											(6)
 *
 *	3.	Tratam mai intai posibilitatea ca primul bloc sa fie trunchiat.
 *		Cautam minimul pornind de la @param1, pana la primul multiplu de
 *		sqrt(N), ce indica inceputul unui bloc.							(7)
 *
 *	4.	Parcurgem blocurile continute complet in interogare si facem update
 *		lui rmq_ans in cazul in care gasim un element mai mic.			(8)
 *
 *	5.	Procedam la fel ca la 2. si pentru posibilitatea trunchierii ultimului
 *		bloc.															(9)
 *
 */

int rmq(int left, int right, int N, int *v, int *preprocessed_data)
{
	int block_size = square_root(N);
	int rmq_ans, i, j;

	left--;	/* (5) */
	right--;

	if (left == right)	/* (6) */
		return v[left];

	rmq_ans = v[left];	/* (7) */
	for (i = left; i % block_size != 0 && i < right; i++) {
		if (v[i] < rmq_ans)
			rmq_ans = v[i];
	}

	/* (8) */
	for (j = i / block_size; i + block_size < right ; i += block_size, j++) {
		if (i >= right)
			break;
		if (preprocessed_data[j] < rmq_ans)
			rmq_ans = preprocessed_data[j];
	}

	/* (9) */
	for (j = i; j <= right; j++) {
		if (v[j] < rmq_ans)
			rmq_ans = v[j];
	}

	return rmq_ans;
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
void get_queries(int N, int M, int *v, int *preprocessed_data, FILE *in)
{
	int left_bound, right_bound;
	FILE *out = fopen("test.out", "wt");

	for (int nr_query = 0; nr_query < M; nr_query++) {
		fscanf(in, "%d %d", &left_bound, &right_bound);
		int rmq_answ = rmq(left_bound, right_bound, N, v, preprocessed_data);
		fprintf(out, "%d\n", rmq_answ);
	}

	fclose(out);
	fclose(in);
}

int main(void)
{
	int N, M, *v = NULL, *preprocessed_data = NULL;
	FILE *in = fopen("test.in", "rt");

	get_data(&N, &M, &v, in);
	preprocess(N, v, &preprocessed_data);
	get_queries(N, M, v, preprocessed_data, in);

	free(v);
	free(preprocessed_data);
}
