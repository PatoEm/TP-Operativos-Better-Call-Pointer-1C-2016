/*
 ============================================================================
 Name        : nucleo.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "nucleo.h"


int main(void) {
	puts("Inicio Nucleo"); /* prints !!!Hello World!!! */
	int fdSocket = crearSocketServidor(6666);

	return EXIT_SUCCESS;
}
