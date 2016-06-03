/*
 * kernelSelect.c
 *
 *  Created on: 3/6/2016
 *      Author: utnso
 */


/***************************************
 * Dependencias
 **************************************/
#include "pointerSocketes.h"
#include <stdio.h>
#include "pointerStream.h"
/*************************************/


/***************************************
 * Variables Globales
 **************************************/
SocketClient* socketUmc;
Socket* serverSocket;

// conjunto maestro de descriptores
fd_set master;

// conjunto temporal de descriptores
fd_set read_fds

// maximo descriptor a monitorear
Int32U fdmax;
/**************************************/


/***************************************
 * Prototipos de funciones
***************************************/
Boolean initServer();
void checkConnections();
newClientHandler(Socket* );
/**************************************/


/*************************************
 * Implementacion de funciones
/*************************************/

// en lo que seria el main
Boolean funcionMain(){
	// hacer conexion con la UMC
	puts("Escuchando conexiones del Kernel o CPUs.");
	socketUmc socketCreateClient();

	//socketConnect(socketUmc, config->umcIP, config-> umcPORT ); la comento porque me rompe, tendria que incluir el config, paja
	if ( socketUmc == NULL){
		puts("No se pudo generar el cliente UMC.");
	} else {
		puts("Cliente generado con exito!!.");
		puts("Enviando HANDSHAKE a la umc.");

		if (handshake(socketUmc, KERNEL_ID)) {
			puts("KER-UMC: HANDSHAKE realizado con exito.");
		} else {
			puts("KER-UMC: No se pudo realizar el HANDSHAKE.");
			return FALSE;
		}
	}

	// genero el server y lo pongo a escuchar
	if (!initServer()) {
		puts("No se ha podido iniciar el socket escucha.");
		puts("El programa terminara.");
		return -1;
	}

	//realizo el SELECT para gestionar conexiones
	checkConnections();


	socketDestroy(serverSocket);

	return FALSE;
}

Boolean initServer() {
	//serverSocket = socketCreateServer(config->PUERTO); la comento por lo mismo de arriba, paja
	if (serverSocket == NULL) {
		puts("No se pudo crear el server escucha.");
		return FALSE;
	}

	if (!socketListen(serverSocket)) {
		puts("No se pudo poner a escuchar al server.");
	}

	puts("Server creado con exito y escuchando.");
	return TRUE;
}

void checkConnections() {
	//SETEO A "0" EL MASTER Y EL TEMPORAL
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	//CARGO EL SOCKET DE ESCUCHA A MASTER
	FD_SET(serverSocket->descriptor, &master);

	//CARGO EL SOCKET MAS GRANDE
	fdmax = serverSocket->descriptor;

	struct timeval timeout;

	//BUCLE PRINCIPAL
	while (TRUE) {

		read_fds = master;
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;

		//HAGO EL SELECT
		Int32U selectResult = select(fdmax + 1, &read_fds, NULL, NULL, &timeout);

		if (selectResult == -1) {

			//SE CAYO MI SOCKET DE ESCUCHA
			puts("Error en el socket de escucha");
			break;


		} else {

			//RECORRO TODOS LOS DESCRIPTORES MONITOREADOS PARA VER QUIEN LLAMO
			Int32U i;
			for (i = 0; i <= fdmax; i++) {

				if (FD_ISSET(i, &read_fds)) {

					//FUE EL SOCKET DE ESCUCHA??
					if (i == serverSocket->descriptor) {

						//SI, ENTONCES GESTIONO LA NUEVA CONEXION ENTRANTE
						Socket* clientSocket = socketAcceptClient(serverSocket);
						newClientHandler(clientSocket);

						//LO CARGO A LA LISTA DE DESCRIPTORES A MONITOREAR Y ACTUALIZO EL MAXIMO
						FD_SET(clientSocket->descriptor, &master);

						if (clientSocket->descriptor > fdmax) {
							fdmax = clientSocket->descriptor;
						}

					} else {
						//NO, ENTONCES GESTIONO EL SOCKET QUE HABLO...
						clientHandler(i);
					}
				}
			}
		}
	}
}

newClientHandler(Socket* client) {
	//continuara...
}
