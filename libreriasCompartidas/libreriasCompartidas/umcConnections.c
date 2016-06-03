/*
 * umcConnections.c
 *
 *  Created on: 3/6/2016
 *      Author: utnso
 */


/***************************************
 * Dependencias
 **************************************/
#include <stdio.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include "pointerSocketes.h"
#include "pointerStream.h"
#include <pthread.h>
/*************************************/


/***************************************
 * Variables Globales
 **************************************/
t_list* socketConnections = NULL;
Int32U umcPort = 0;
/**************************************/


/***************************************
 * Prototipos de funciones
***************************************/
void manageSocketConnections();
void* manageSocketConnection(void*);

Boolean manageCpuRequest(Socket*, StrCpuUmc*);
Boolean manageKernelRequest(Socket*, StrKerUmc*);
/**************************************/


/*************************************
 * Implementacion de funciones
/*************************************/

// esta funcion iria en el mainUmc.c
void manageSocketConnections() {
	socketConnections = list_create();
	Socket* s = socketCreateServer(umcPort);
	while (TRUE) {
		pthread_t socketConnection;
		puts("Escuchando conexiones del Kernel o CPUs.");
		socketListen(s);
		Socket* socketClient;
		socketClient = socketAcceptClient(s);
		if	(socketClient != NULL){
			puts("Alguien se conecto.");
			pthread_create(&socketConnection, NULL, manageSocketConnection, (void*) socketClient);
			list_add(socketConnections, &socketConnection);
		}
	}
}

void* manageSocketConnection(void* param){
	Socket* socket = (Socket*) param;
	Boolean connected = TRUE;
	puts("Gestion de conexiones.");
	while (connected){
		puts("Esperando el request.");
		SocketBuffer* sb = socketReceive(socket);
		puts("Entro el request.");
		if (sb != NULL) {
			Char id = getStreamID((Stream) sb->data);
			StrKerUmc* sku = NULL;
			StrCpuUmc* scm = NULL;
			switch (id) {
			case KERNEL_ID:
				sku = unserializeKerUmc((Stream) sb->data);
				connected = manageKernelRequest(socket, sku);
				break;
			case CPU_ID:
				scm = unserializeCpuUmc((Stream) sb->data);
				connected = manageCpuRequest(socket, scm);
				break;
			default:
				connected = FALSE;
				break;
			}
		} else {
				puts("No pudo recibir request, desconectando al cliente.");
				connected = FALSE;
			}
		}
	return NULL;
	}



