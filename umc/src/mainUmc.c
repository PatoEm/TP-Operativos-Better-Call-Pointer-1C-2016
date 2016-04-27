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



	leerArchivoDeConfiguracion("configumc");

	/*no tengo idea para que sirve esto, lo puedo romper? COMENTEMOS QUE HACEMOS*/

	puts("Inicio UMC");


	int fdSocketUMC = crearCliente(ip_Swap,puerto_Swap);



	int fdSocketCPU= crearSocketServidor(puertoCPU);
	escucharSocket(fdSocketCPU,5);
	int conexionCPU = aceptarConexiones(fdSocketCPU);



	/*Socket funcionando en la primera, falla el bind en la segunda. Testeado by Dr.Mengueche*/

	int fdSocketNucleo= crearSocketServidor(puertoNucleo);
	escucharSocket(fdSocketNucleo,1);
	int aceptarNucleo=aceptarConexiones(fdSocketNucleo);

	printf("número de cliente %d",fdSocketUMC);


	//printf("el socket de la umc es:%d y el del nucleo es: %d",fdSocketUMC, aceptarNucleo);
	return EXIT_SUCCESS;

}
