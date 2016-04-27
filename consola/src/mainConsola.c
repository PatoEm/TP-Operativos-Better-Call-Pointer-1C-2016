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


	/*lee archivo de configuración y se conecta al nucleo. COMENTEMOS QUE HACEMOS*/
    leerArchivoDeConfiguracion("consola/configconsola");


	int fdSocketConsola = crearCliente(ipNucleo,nucleoPort );
	printf("%d",fdSocketConsola);



	return EXIT_SUCCESS;
}
