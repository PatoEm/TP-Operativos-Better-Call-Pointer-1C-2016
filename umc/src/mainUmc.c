/*
 ============================================================================
 Name        : umc.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "umc.h"

int main(void) {

	void leerArchivoDeConfiguracion();

		puts("Inicio UMC");
		int fdSocketUMC = crearSocketServidor(puertoTCP);
		escucharSocket(fdSocketUMC, espera);
		int fdSocketCliente= aceptarConexiones(fdSocketUMC);

		printf("%d",fdSocketUMC);
		return EXIT_SUCCESS;

}
