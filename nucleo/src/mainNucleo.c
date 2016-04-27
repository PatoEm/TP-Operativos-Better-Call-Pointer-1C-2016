/*
 ============================================================================
 Name        : nucleo.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "nucleo.h"


int main(void) {

	 leerArchivoDeConfiguracion("confignucleo");

	/*Se conecta con la memoria. Author: Dr.Mengueche*/
	 /* Socket UMC testeado by Dr.Mengueche*/
	int fdSocketUMC=crearCliente(ipUMC,UMCPort);

	/*lee archivo de configuración y se conecta a la consola. COMENTEMOS QUE HACEMOS*/


	puts("Inicio Nucleo");
	//printf("%s",*puertoPropio);
	//const char * puto = &puertoPropio;
 	int fdSocketConsola = crearSocketServidor(puertoPropio);
	//int fdSocketConsola = crearSocketServidor("6002");
	escucharSocket(fdSocketConsola,5);
	int fdSocketCliente1= aceptarConexiones(fdSocketConsola);


	/* creo el servidor para el cpu y lo acepto. Author: Dr.Mengueche */

	int fdSocketCpu= crearSocketServidor(cpuPort);
	escucharSocket(fdSocketCpu,5);
	int fdSocketClienteCpu1=aceptarConexiones(fdSocketCpu);


	printf("acá esta la consola:%d ",fdSocketConsola);

	return EXIT_SUCCESS;
}
