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

#include "archivosAnsisop.h"


int main(void) {

	//lee archivo de configuración y se conecta al nucleo. COMENTEMOS QUE HACEMOS
	 leerArchivoDeConfiguracion("configconsola");
	 
	char* contenidoArchivo;
	
	contenidoArchivo=(char*)malloc(sizeof(char*));
	if(contenidoArchivo==NULL){
		printf("no se pudo reservar memoria para el archivo");
		return-1;
	}
	



	int fdSocketConsola = crearCliente(ipNucleo,nucleoPort );
	//int fdSocketConsola = crearCliente("127.0.0.1","6002" );
	enviarMensaje(fdSocketConsola,"Consola Ok",11*sizeof(char));



	leerProgramaAnsisop("programasEjemplo/completo.ansisop");
	puts("hola");



	return EXIT_SUCCESS;
}
