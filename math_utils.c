/**
 * Copyright 2021 - Dumitrescu Alexandra
 * grupa 323, seria CA, ACS UPB
 * Tema #1 - RMQ - Range Minimum Query
 * Noiembrie 2021
 */
#include <stdlib.h>
#include <stdio.h>
#include "math_utils.h"

/**
 * @log_2()
 * @param a - Numarul pe care aplicam functia log_2
 *
 *	Functia cauta si returneaza floor(log_2(@param 1))
 */
int log_2(int a) {
	int i = 1, pow = 0;
	while(i < a) {
		pow ++;
		i = (i << 1);
	}
	if(i == a)
		return pow;
		
	return pow - 1;
}

/**
 * @square_root()
 * @param a - Numarul pe care aplicam radical
 *
 *	Functia returneaza parte intreaga din radical unui nr
 */
int square_root(int a)
{
	int sq_root = 1;
	while(sq_root * sq_root <= a)
		sq_root ++;
	return sq_root - 1;
}

/**
 * @is_perfect_square()
 * @param a - Numarul pe care il verificam
 *
 *	Functia verifica daca un nr e patrat perfect, folosindu-se
 *	de square_root.
 */
int is_perfect_square(int a)
{
	if(square_root(a) * square_root(a) == a)
		return 1;
	return 0;
}

/**
 * @get_min()
 * Utilitar care intoarce minimul intre 2 nr
 */
int get_min(int a, int b) {
	if(a < b)
		return a;
	return b;
}
