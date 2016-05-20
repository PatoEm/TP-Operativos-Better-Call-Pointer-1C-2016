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

	//Conecto con el núcleo primero que nada Author: Dr.Mengueche

	clienteNucleo=crearCliente(ipNucleo,nucleoPort);

	//Conecto con el cpu. Author: Dr.Mengueche
	puts("hola");
	int clienteUMC=crearCliente(ipUMC,umcPort);

	//Imprimo al cliente. Author: Dr.Mengueche


	return EXIT_SUCCESS;

}
