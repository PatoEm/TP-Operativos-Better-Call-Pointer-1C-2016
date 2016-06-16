/*
 * cpuHandler.h
 *
 *  Created on: 5/6/2016
 *      Author: utnso
 */

#ifndef CPUHANDLER_H_
#define CPUHANDLER_H_

// DEFINES
#define CPU_HANDLER_SOCKET 2020


// INCLUDES
#include <libreriasCompartidas/pointerSocketes.h>
#include <libreriasCompartidas/pointerStream.h>
#include <commons/collections/list.h>
#include <stdio.h>

// VARIABLES GLOBALES
t_list* coreList;
Socket* serverSocket;


// DEFINICION DE FUNCIONES
void cpuHandlerThread();
void* cpuHandlerThreadRoutine(void*);
Boolean initCpuServer();
void checkCpuConnections();
void newClientHandler(Socket* client);
void newCpuClient(Socket* cpuClient, Stream dataSerialized);
void clientHandler(Socket* cliente);
int cpuCoreInList(t_list* lista, Socket* cliente);


// ESTRUCTURAS

typedef struct core {
	pcb* pcb;
	Socket* cpuClient;
} t_core;



#endif /* CPUHANDLER_H_ */
