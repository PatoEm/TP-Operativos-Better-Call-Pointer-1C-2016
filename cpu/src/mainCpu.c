/*
 ============================================================================
 Name        : cpu.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "cpu.h"

int main(void) {

	leerArchivoDeConfiguracion("configcpu");

	/*Conecto con el núcleo primero que nada Author: Dr.Mengueche*/

	int clienteNucleo=crearCliente(nucleoPort,nucleoPort);
	char* mensaje;
	mensaje=(char*)malloc(12*sizeof(char));
	if(mensaje==NULL){
		printf("falló al reservar la memoria");
		return -1;
	}
	recibirMensaje(clienteNucleo,mensaje,12*sizeof(char));
	printf("%s",mensaje);

	/*Conecto con el cpu. Author: Dr.Mengueche*/

	int clienteUMC=crearCliente(ipUMC,umcPort);
	enviarMensaje(clienteUMC,mensaje,12*sizeof(char));


	/*Imprimo al cliente. Author: Dr.Mengueche*/


	return EXIT_SUCCESS;

}
