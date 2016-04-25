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
		int fdSocketUMC = crearSocketServidor(progPort);
		escucharSocket(fdSocketUMC, espera); /*el espera va así y acá?
		posta que me re perdí, porque es quien modifica la cant de miliseg
		antes de la respuesta pero temo que entendí cualquiera*/
		int fdSocketCliente= aceptarConexiones(fdSocketUMC);

		printf("%d",fdSocketUMC);
		return EXIT_SUCCESS;

}
