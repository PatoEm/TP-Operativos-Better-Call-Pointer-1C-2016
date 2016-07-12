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
#include "nucleo.h"

// VARIABLES GLOBALES
t_list* coreList;
t_list* consoleList;
Socket* serverSocket;


// DEFINICION DE FUNCIONES
void cpuHandlerThread();
void* cpuHandlerThreadRoutine(void*);
Boolean initCpuServer();
void checkCpuConnections();
void newClientHandler(Socket* client);
void newCpuClient(Socket* cpuClient, Stream dataSerialized);
void clientHandler(int cliente);
int cpuCoreInList(t_list* lista, Socket* cliente);
void cpuClientHandler(Socket* cpuClient, Stream data);
void newConsoleClient(Socket* consoleClient, Stream dataSerialized);
int cantidadPaginasArchivo(int longitudArchivo);
int pedirTamanioDePagina(int puerto);

// ESTRUCTURAS

typedef struct core {
	pcb* pcb;
	Socket* cpuClient;
} t_core;
typedef struct console {
	Socket *consoleClient;
	pcb *pcb;
	Byte *data;
	Int32U dataLength;
} Console;




#endif /* CPUHANDLER_H_ */
