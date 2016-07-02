/*
 * cpuHandler.h
 *
 *  Created on: 5/6/2016
 *      Author: utnso
 */

#ifndef UMC_SERVER_H_
#define UMC_SERVER_H_

// DEFINES
#define CPU_HANDLER_SOCKET 2020


// INCLUDES
#include <libreriasCompartidas/pointerSocketes.h>
#include <libreriasCompartidas/pointerStream.h>
#include <commons/collections/list.h>
#include <stdio.h>

// VARIABLES GLOBALES
t_list* coreList;
t_list* consoleList;
Socket* serverSocket;


// DEFINICION DE FUNCIONES
void UmcServerThread();
void* UmcServerThreadRoutine(void*);
Boolean initServer();
void checkConnections();
void newClientHandler(Socket*);
void newCpuClient(Socket*, Stream);
void clientHandler(int );
int cpuCoreInList(t_list*, Socket*);
void cpuClientHandler(Socket*, Stream);
void newKernelClient(Socket*, Stream);


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



#endif /* UMC_SERVER_H_ */
