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
#define PUERTO "7100"

int main(){
	int nuevoCliente=0;
	int socketNucleo = crearSocketServidor(PUERTO, IP);

	while(nuevoCliente==0){
	escucharSocket(socketNucleo,1);
	nuevoCliente=aceptarConexionSocket(socketNucleo);

	}

	printf("%d",nuevoCliente);

	cerrarSocket(socketNucleo);

return 0;
}

