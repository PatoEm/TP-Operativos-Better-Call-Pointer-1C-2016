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


	/*lee archivo de configuración y se conecta a la consola. COMENTEMOS QUE HACEMOS*/


	puts("Inicio Nucleo");
	//printf("%s",*puertoPropio);
	//const char * puto = &puertoPropio;
 	int fdSocketConsola = crearSocketServidor(puertoPropio);
	//int fdSocketConsola = crearSocketServidor("6002");
	escucharSocket(fdSocketConsola,5);
	int fdSocketCliente1= aceptarConexiones(fdSocketConsola);
	char* mensaje;
	mensaje=(char*)malloc(12*sizeof(char));
	if(mensaje==NULL){
		printf("falló al reservar la memoria");
		return -1;
	}
	recibirMensaje(fdSocketCliente1,mensaje,12*sizeof(char));
	printf("%s",mensaje);

	/* creo el servidor para el cpu y lo acepto. Author: Dr.Mengueche */

	int fdSocketCpu= crearSocketServidor(cpuPort);
	escucharSocket(fdSocketCpu,5);
	int fdSocketClienteCpu1=aceptarConexiones(fdSocketCpu);
	enviarMensaje(fdSocketCliente1,mensaje,12*sizeof(char));


	/*Se conecta con la memoria. Author: Dr.Mengueche*/
	 /* Socket UMC testeado by Dr.Mengueche*/
	int fdSocketUMC=crearCliente(ipUMC,UMCPort);

	printf();

	return EXIT_SUCCESS;
}
