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

    void leerArchivoDeConfiguracion();


	int fdSocketConsola = crearCliente(ipNucleo,nucleoPort );
	printf("%d",fdSocketConsola);



	return EXIT_SUCCESS;
}
