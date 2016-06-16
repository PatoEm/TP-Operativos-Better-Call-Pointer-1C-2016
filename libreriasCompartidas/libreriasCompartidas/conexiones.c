/*
 * conexiones.c
 *
 *  Created on: 4/6/2016
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

Boolean sendResponse(Char, void*, Socket*);
/**************************************/

/*************************************
* Implementacion de funciones
*************************************/

void manageSocketConnections(){
	socketConnections = list_create();
	Socket* s = socketCreateServer(umcPort);
	while (TRUE){
		pthread_t socketConnection;
		puts("Escuchando conexiones del Kernel o CPUs.");
		socketListen(s);
		Socket* socketClient;
		socketClient = socketAcceptClient(s);
		if (socketClient!=NULL) {
			puts("Alguien se conecto.");
			pthread_create(&socketConnection, NULL, manageSocketConnection, (void*) socketClient);
			list_add(socketConnections, &socketConnection);
		}
	}

}

void* manageSocketConnection(void* param) {
	Socket* socket = (Socket*) param;
	Boolean connected = TRUE;
	puts("Gestion de conexiones.");
	while (connected) {
		puts("Esperando el request.");
		SocketBuffer* sb = socketReceive(socket);
		puts("Entro el request.");
		if (sb != NULL) {
			Char id = getStreamId((Stream) sb->data);
			StrKerUmc* sku = NULL;
			StrCpuUmc* scu = NULL;
			switch (id) {
			case KERNEL_ID:
				sku = unserializeKerUmc((Stream) sb->data);
				connected = manageKernelRequest(socket, sku);
				break;
			case CPU_ID:
				scu = unserializeCpuUmc((Stream) sb->data);
				connected = manageCpuRequest(socket, scu);
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

Boolean manageCpuRequest(Socket* socket, StrCpuUmc* scu){
	switch (scu->action) {
		case 1 /*MEM_READ*/:
			printf("Nuevo stream CPU-UMC de MEM_READ");
			// hacer lo que tenga que hacer
			break;
		case 2 /*MEM_WRITE*/:
			printf("Nuevo stream CPU-UMC de MEM_WRITE");
			// hacer lo que tenga que hacer
			break;
		case 3 /*CREATE_SEG*/:
			printf("Nuevo stream CPU-UMC de CREATE_SEG");
			// hacer lo que tenga que hacer
			break;
		case 4 /*DELETE_SEG*/:
			printf("Nuevo stream CPU-UMC de DELETE_SEG");
			// hacer lo que tenga que hacer
			break;
		case 5 /*HANDSHAKE*/:
			printf("Nueva conexion de CPU");
			printf("HANDSHAKE de CPU recibido");
			printf("HANDSHAKE respuesta enviado");
			// hacer lo que tenga que hacer
			break;
		default:
			printf("No se pudo identificar la accion de la CPU");
			break;
	}

	StrUmcCpu* suc ;/*= newStrUmcCpu();*/
	Boolean result = sendResponse(CPU_ID, suc, socket);

	return result;
}

Boolean manageKernelRequest(Socket* socket, StrKerUmc* sku){
	switch (sku->action) {
		case 1 /*MEM_READ*/:
			printf("Nuevo stream KERNEL-UMC de MEM_READ");
			// hacer lo que tenga que hacer
			break;
		case 2 /*MEM_WRITE*/:
			printf("Nuevo stream KERNEL-UMC de MEM_WRITE");
			// hacer lo que tenga que hacer
			break;
		case 3 /*CREATE_SEG*/:
			printf("Nuevo stream KERNEL-UMC de CREATE_SEG");
			// hacer lo que tenga que hacer
			break;
		case 4 /*DELETE_SEG*/:
			printf("Nuevo stream KERNEL-UMC de DELETE_SEG");
			// hacer lo que tenga que hacer
			break;
		case 5 /*HANDSHAKE*/:
			printf("Nueva conexion de KERNEL");
			printf("HANDSHAKE de Kernel recibido");
			printf("HANDSHAKE respuesta enviado");
			// hacer lo que tenga que hacer
			break;
		default:
			printf("No se pudo identificar la accion de la CPU");
			break;
	}

	StrUmcKer* suk ;/*= newStrUmcKer();*/
	Boolean result = sendResponse(KERNEL_ID, suk, socket);

	return result;
}

Boolean sendResponse(Char target, void* stream, Socket* socket) {
	SocketBuffer* sb = NULL;
	puts("Serializando.");
	switch(target) {
		case CPU_ID:
			sb = serializeUmcCpu((StrUmcCpu*) stream);
			puts("Enviando respuesta a la CPU.");
			break;
		case KERNEL_ID:
			sb = serializeUmcKer((StrUmcKer*) stream);
			puts("Enviando respuesta al KERNEL.");
			break;
		default:
			return FALSE;
	}

	//Envio respuesta
	if( !socketSend(socket, sb)) {
		puts("No se pudo enviar el Stream.");
		if (sb != NULL) {
			free(sb);
			sb = NULL;
		}
		return FALSE;
	}
	puts("Enviado con exito");

	return TRUE;
}


