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
	int fdSocketNucleo = crearSocketServidor("6666");
	escucharSocket(fdSocketNucleo,5);
	int fdSocketCliente1= aceptarConexiones(fdSocketNucleo);

	printf("%d",fdSocketNucleo);
	return EXIT_SUCCESS;
}
