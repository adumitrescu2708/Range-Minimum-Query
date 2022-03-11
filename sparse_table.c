/**
 * Copyright 2021 - Dumitrescu Alexandra
 * grupa 323, seria CA, ACS UPB
 * Tema #1 - RMQ - Range Minimum Query
 * Noiembrie 2021
 */

/**
 * SPARSE TABLE
 */
#include <stdlib.h>
#include <stdio.h>
#include "math_utils.h"

/**
 *	PREPROCESARE:
 *
 *	1.	Ideea ce sta la baza algoritmului este memorarea intr-o matrice
 *		rara a minimului pentru toate subintervalele ce incep de pe pozitia
 *		i si au dimensiunea 2^j.
 *
 *		1.1		preprocessed_data[i][j] = min(v[k]), unde k = i:(i + 2^j - 1)
 *				i = 0:N
 *				j = 0:log_2(N)
 *
 *		1.2		Algoritmul de construire a matricei este unul bazat pe o
 *				metoda de programare dinamica. Matricea se construieste
 *				progresiv, la fiecare pas tinand cont de informatiile colectate
 *				anterior.
 *
 *		1.3		Am folosit functii matematice implementate.
 *				Pentru mai multe detalii: vezi si <math_utils.c>
 *
 *	2. Matricea rara														(1)
 *
 *		2.1		Intrucat vrem sa retinem minimul pentru intervale de un singur
 *				element, matricea de preprocesare se va comporta astfel:
 *				preprocessed_data[i][0] = v[i];
 *				Astfel, matricea noastra va avea N linii. In plus,
 *				prima coloana ce codifica minimul pentru intervale de un
 *				element va contine elementele in sine.						(2)
 *
 *		2.2		Pentru numarul de coloane, luam in considerare faptul ca vrem
 *				sa memoram minimul pentru intervale 2^j. Astfel, cea mai
 *				mare valoare a lui j va fi log_2(N), deci avem log_2(N) + 1
 *				coloane.
 *
 *	3. Minimul prin programare dinamica:									(3)
 *
 *		3.1		Daca dorim minimul pentru intervalul ce incepe pe pozitia i si
 *				are dimensiune 2^j, putem reduce cautarea folosind informatii
 *				anterioare, spargand intervalul in 2 subintervale, anume
 *				(i, i + 2^j - 1) si (i + 2^(j - 1), i + 2^j - 1).
 *
 *		3.2		Exemplu:
 *					v = [0, 1, 2, 3, 4, 5]
 *
 *				prep[1][2] = min(v[1], v[2], v[3], v[4])
 *				prep[1][1] = min(v[1], v[2])
 *				prep[1 + 2^1][1] = prep[3][1] = min(v[3], v[4]);
 *			>>	prep[1][2] = min(prep[1][1], prep[3][1])
 *
 *
 *	CONCLUZII:
 *	1.	Memoria auxiliara necesara: matrice de N linii si log_2(N) + 1 coloane
 *
 */

/**
 * @preprocess
 *
 * @param N - Numarul de elemente
 * @param v - Datele de intrare
 * @param preprocessed_data - Matricea in care preprocesam datele
 * @param size - Numarul de coloane din matrice
 *
 *	1. Se aloca memorie pentru matrice
 *	2. Se initializeaza prima coloana din matrice conform (2)
 *	3. Pasul de calculare a minimelor prin programare dinamica (3)
 *
 */

void preprocess(int N, int *v, int ***preprocessed_data)
{
	/* (1) */
	(*preprocessed_data) = malloc(N * sizeof(int *));
	for (int i = 0; i < N; i++)
		(*preprocessed_data)[i] = calloc(log_2(N) + 1, sizeof(int));


	/* (2) */
	for (int i = 0; i < N; i++)
		(*preprocessed_data)[i][0] = v[i];

	/* (3) */
	for (int j = 1; j < (log_2(N) + 1); j++) {
		for (int i = 0; (i + (1 << (j - 1))) < N; i++)
			(*preprocessed_data)[i][j] =
				get_min((*preprocessed_data)[i + (1 << (j - 1))][j - 1],
								 (*preprocessed_data)[i][j - 1]);
	}
}

/**
 * INTEROGARI:
 *
 * rmq()
 * @param left - Indexul stang al intervalului de cautare
 * @param right - Indexul drept al intervalului de cautare
 * @param preprocessed_data - matricea de date preprocesate
 * @return int - Raspunsul interogarii - elementul minim din intervalul
 *							[@param1, @param2]
 *
 *	1.	Indexarea in interogari este perceputa de la 1 la N, deci
 *		scadem o unitate din @param1, @param2.							(4)
 *
 *	2.	Minimul pentru intervalul (xi, yi) va fi minimul
 *		dintre (xi, xi + 2^K - 1) si (yi - 2^K + 1, yi), unde K este
 *		cea mai mica putere a lui 2 mai mica decat lungimea intervalului
 *		interogat.
 */


int rmq(int left, int right, int **preprocessed_data)
{
	left--;	/* (4) */
	right--;
	int closest_pow = log_2(-left + right + 1);

	if (preprocessed_data[left][closest_pow] <=
			 preprocessed_data[right - (1 << closest_pow) + 1][closest_pow])
		return preprocessed_data[left][closest_pow];
	else
		return preprocessed_data[right - (1 << closest_pow) + 1][closest_pow];
}

/**
 * @get_queries()
 *
 * Functie ce citeste si raspunde la interogari
 *
 */
void get_queries(int M, int **preprocessed_data, FILE *in)
{
	int left_bound, right_bound;

	FILE *out = fopen("test.out", "wt");

	for (int nr_query = 0; nr_query < M; nr_query++) {
		fscanf(in, "%d %d", &left_bound, &right_bound);
		fprintf(out, "%d\n", rmq(left_bound, right_bound, preprocessed_data));
	}

	fclose(out);
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


int main(void)
{
	int N, M, *v = NULL, **preprocessed_data = NULL;

	FILE *in = fopen("test.in", "rt");

	get_data(&N, &M, &v, in);
	preprocess(N, v, &preprocessed_data);
	get_queries(M, preprocessed_data, in);

	free(v);
	for (int i = 0; i < N; i++)
		free(preprocessed_data[i]);
	free(preprocessed_data);
	fclose(in);
}
