/*
 ============================================================================
 Name        : consola.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "consola.h"
int main(void) {
	char * tiempo = temporal_get_string_time();

	int fdSocketConsola = crearCliente("127.0.0.1", 6666);
	printf("%d",fdSocketConsola);
	puts(tiempo);
	free(tiempo);
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	return EXIT_SUCCESS;
}
