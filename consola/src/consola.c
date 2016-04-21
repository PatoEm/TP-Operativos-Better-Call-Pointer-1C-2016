/*
 * consola.c
 *
 *  Created on: 17/4/2016
 *      Author: utnso
 */

#include "consola.h"
#include "/home/utnso/tp-2016-1c-Better-call-pointer/CosasGenerales/sockets.c"
#define IP "127.0.0.1"
#define PUERTO "7100"

int main() {

int socketCliente = crearSocketCliente(NULL,PUERTO,IP);

int prueba = conectarSocket(socketCliente, IP, PUERTO);

printf("%d",prueba);

return 0;
}

