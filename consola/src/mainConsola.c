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


	int fdSocketConsola = crearCliente("127.0.0.1", "6666");
	printf("%d",fdSocketConsola);



	return EXIT_SUCCESS;
}
