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

	int fdSocketCPU= crearSocketServidor(puertoCPU);
	escucharSocket(fdSocketCPU,5);
	int conexionCPU = aceptarConexiones(fdSocketCPU);
	char* mensaje;
	mensaje=(char*)malloc(12*sizeof(char));
	if(mensaje==NULL){
		printf("falló al reservar la memoria");
		return -1;
	}
	recibirMensaje(conexionCPU,mensaje,12*sizeof(char));
	printf("%s",mensaje);

	/*Socket funcionando en la primera, falla el bind en la segunda. Testeado by Dr.Mengueche*/

	int fdSocketNucleo= crearSocketServidor(puertoNucleo);
	escucharSocket(fdSocketNucleo,1);
	int aceptarNucleo=aceptarConexiones(fdSocketNucleo);


	int fdSocketUMC = crearCliente(ip_Swap,puerto_Swap);
	enviarMensaje(fdSocketUMC,mensaje,12*sizeof(char));


	//printf("el socket de la umc es:%d y el del nucleo es: %d",fdSocketUMC, aceptarNucleo);

	return EXIT_SUCCESS;

}
