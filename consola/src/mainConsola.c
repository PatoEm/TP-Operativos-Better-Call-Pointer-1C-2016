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
    leerArchivoDeConfiguracion("configconsola");


	int fdSocketConsola = crearCliente(ipNucleo,nucleoPort );
	//int fdSocketConsola = crearCliente("127.0.0.1","6002" );
	enviarMensaje(fdSocketConsola,"Consola Ok",12*sizeof(char));






	return EXIT_SUCCESS;
}
