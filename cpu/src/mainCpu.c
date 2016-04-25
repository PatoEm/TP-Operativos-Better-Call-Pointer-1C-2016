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

	void leerArchivoDeConfiguracion();

	/*Conecto con el núcleo primero que nada Author: Dr.Mengueche*/

	int clienteNucleo=crearCliente(nucleoPort,nucleoPort);

	/*Conecto con el cpu. Author: Dr.Mengueche*/

	int clienteUMC=crearCliente(ipUMC,umcPort);


	/*Imprimo al cliente. Author: Dr.Mengueche*/

	printf("mi número de cliente es: %i", clienteNucleo);

	return EXIT_SUCCESS;

}
