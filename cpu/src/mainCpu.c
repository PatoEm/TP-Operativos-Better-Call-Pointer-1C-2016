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


		int fdSocketCPU = crearCliente(ipUMC,umcPort );/*lo mismo que con el SWAP:
		va UMC o van variables de CPU? posta no lo entiendo :/ */
		printf("%d",fdSocketCPU);

		return EXIT_SUCCESS;

}
