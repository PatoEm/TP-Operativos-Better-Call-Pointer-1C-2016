/*
 * nucleo.c
 *
 *  Created on: 17/4/2016
 *      Author: PatoEm
 */

#include "/home/utnso/tp-2016-1c-Better-call-pointer/CosasGenerales/sockets.c"

#include "nucleo1.h"
#include <unistd.h>
#define IP "127.0.0.1"
#define PUERTO "6667"

int main(){

	int socketNucleo = crearSocketServidor(PUERTO, IP);

	escucharSocket(socketNucleo,1);

	printf("Tiene 10 segundos para conectarse");
	sleep(10000);

	int prueba = aceptarConexionSocket(socketNucleo);
	printf("%d",prueba);

	cerrarSocket(socketNucleo);

return 0;
}

