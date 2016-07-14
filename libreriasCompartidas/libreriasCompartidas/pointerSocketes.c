/*
 * pointerSocketes.c
 *
 *  Created on: 31/5/2016
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "pointerTipos.h"
#include "pointerSocketes.h"
#include "pointerStream.h"
#include "pcb.h"

/**
 * @NAME: socketCreate
 * @DESC: Crea un socket y devuelve un struct con el estado de creacion y el descriptor correspondiente.
 *
 * CREAR UN SOCKET:
 *
 * AF_INET: SOCKET DE INTERNET IPV4
 * SOCK_STREAM: ORIENTADO A LA CONEXION (TCP)
 * 0: USAR PROTOCOLO POR DEFECTO PARA AF_INET-SOCK_STREAM: PROTOCOLO TCP/IPV4
 *
 */
Socket* socketCreate() {

	Socket* ptrNewSocket = NULL;
	Int32U socketDescriptor;

	if ((socketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		return NULL;
	}

	ptrNewSocket 				= (Socket *)malloc(sizeof(Socket));
	ptrNewSocket->descriptor 	= socketDescriptor;

	return ptrNewSocket;

}

/**
 * @NAME: socketCreate
 * @DESC: Obtiene un Descriptor de Socket a traves de su Direccion
 * @PARAMS:
 * 		socketAddress	: Struct sockaddr_in con la informacion de la direccion del Descriptor
 */
Socket* socketGetServerFromAddress(struct sockaddr_in socketAddress) {

	Socket *ptrSocketServer = (Socket *)malloc(sizeof(Socket));

	ptrSocketServer->ptrAddress	 					= (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	ptrSocketServer->ptrAddress->sin_family 		= socketAddress.sin_family;
	ptrSocketServer->ptrAddress->sin_addr.s_addr	= socketAddress.sin_addr.s_addr;
	ptrSocketServer->ptrAddress->sin_port 			= socketAddress.sin_port;

	return ptrSocketServer;
}

/*
 * @NAME: socketCreateServer
 * @DESC: Crea un socket para ser utilizado como server. Realiza los procesos de Socket y Bind.
 * Devuelve un struct con el estado de creacion y el descriptor correspondiente.
 * @PARAMS:
 *		port	: puerto de escucha
 */
Socket* socketCreateServer(Int32U port)
{
	Socket	*ptrSocketServer = NULL;

	struct sockaddr_in socketInfo;
	int optval = 1;

	if ((ptrSocketServer = socketCreate()) == NULL) {
		return NULL;
	}

	//HACER QUE EL SO LIBERE LE PUTERO INMEDIATAMENTE LUEGO DE CERRAR EL SOCKET
	setsockopt(ptrSocketServer->descriptor, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	socketInfo.sin_family 		= AF_INET;
	socketInfo.sin_addr.s_addr 	= INADDR_ANY;
	socketInfo.sin_port 		= htons(port);

	// VINCULAR EL SOCKET CON UNA DIRECCION DE RED ALMACENADA EN 'socketInfo'
	if (bind(ptrSocketServer->descriptor, (struct sockaddr*) &socketInfo,
			sizeof(socketInfo)) != 0) {

		free(ptrSocketServer);
		return NULL ;

	}

	return ptrSocketServer;
}

/**
 * @NAME: socketCreateClient
 * @DESC: Crea un socket para ser utilizado como Cliente.
 * Devuelve un struct con descriptor correspondiente.
 */
SocketClient* socketCreateClient() {

	Socket *ptrNewSocket;

	if ((ptrNewSocket = socketCreate()) == NULL) {
		return NULL;
	}

	SocketClient *ptrSocketClient = (SocketClient *)malloc(sizeof(SocketClient));

	ptrSocketClient->ptrSocket = ptrNewSocket;

	return ptrSocketClient;
}

/*
 * @NAME: socketListen
 * @DESC: Pone a escuchar un socket.
 * Retorna TRUE en caso de exito, FALSE caso contrario
 * @PARAMS:
 *		ptrSocket	: El socket que se va a poner a escuchar
 */
Boolean socketListen(Socket *ptrSocket) {

	if (listen(ptrSocket->descriptor, MAX_CONEXIONES) != 0) {
		return FALSE;
	}

	return TRUE;
}

/*
 * @NAME: socketConnect
 * @DESC: Conecta al socket cliente al servidor a traves de una Direccion IP y un Puerto.
 * Devuelve TRUE en caso de exito, FALSE caso contrario
 * @PARAMS:
 *		ptrSocketClient		: Socket cliente
 *		ptrServerAddress	: Direccion IP del server al que desea conectarse
 *		serverPort			: Puerto del server al que desea conectarse
 */
Boolean socketConnect(SocketClient *ptrSocketClient, String ptrServerAddress, Int32U serverPort) {

	struct sockaddr_in socketAddress;

	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = inet_addr(ptrServerAddress);
	socketAddress.sin_port = htons(serverPort);

	// CONECTA AL SOCKET CON ESA DIRECCION A TRAVES DE ESE PUERTO
	if (connect(ptrSocketClient->ptrSocket->descriptor, (struct sockaddr*) &socketAddress, sizeof(socketAddress))
			== -1) {
		return FALSE; //false,error value
	}

	ptrSocketClient->ptrSocketServer = socketGetServerFromAddress(socketAddress);

	return TRUE;

}

/*
 * @NAME: socketacceptClient
 * @DESC: Acepta una conexion entrante y devuelve el descriptor asignado a ese cliente.
 * @PARAMS:
 *		ptrListenSocket	: El socket que realiza la escucha
 */
Socket* socketAcceptClient(Socket* ptrListenSocket) {

	Socket *ptrSocketClient = (Socket *)malloc(sizeof(Socket));

	ptrSocketClient->ptrAddress	= (struct sockaddr_in *)malloc( sizeof(struct sockaddr_in) );
	Int32U addrlen 				= sizeof(struct sockaddr_in);

	Int32U newSocketDescriptor;

	//CADA VEZ QUE SE ACEPTA UNA NUEVA CONEXION ENTRANTE, SE GENERA UN NUEVO DESCRIPTOR
	if ((newSocketDescriptor = accept(ptrListenSocket->descriptor,
			(struct sockaddr*) (ptrSocketClient->ptrAddress), (void *) &addrlen))
			< 0) {

		free(ptrSocketClient->ptrAddress);
		free(ptrSocketClient);
		return NULL;

	}

	ptrSocketClient->descriptor = newSocketDescriptor;
	return ptrSocketClient;

}

/*
 * @NAME: socketReceive
 * @DESC: Recibe un mensaje de un socket cliente y devuelve el buffer donde fue recibido.
 * @PARAMS:
 *		ptrSender	: El descriptor del emisor del mensaje
 */
SocketBuffer *socketReceive(Socket *ptrSender) {

	SocketBuffer *ptrBuffer = malloc( sizeof(SocketBuffer) );

	int bytesReceived;

	if ((bytesReceived = recv(ptrSender->descriptor, ptrBuffer->data, BUFF_SIZE, 0)) > 0) {
		ptrBuffer->size = bytesReceived;
		return ptrBuffer;
	}

	free(ptrBuffer);
	return NULL;

}

/**
 * @NAME: socketSend
 * @DESC: Realiza el envio de un mensaje a un cliente
 * Retorna TRUE en caso de exito, FALSE caso contrario
 * @PARAMS:
 *		ptrDestination	: El descriptor del cliente que va a recibi el mensaje
 *		ptrBuffer		: El buffer con el mensaje a enviar
 */
Boolean socketSend(Socket *ptrDestination, SocketBuffer *ptrBuffer) {

	if (send(ptrDestination->descriptor, ptrBuffer->data, ptrBuffer->size, 0) >= 0) {
		return TRUE;
	}

	return FALSE;
}
/**
 * @NAME: socketDestroy
 * @DESC: Realiza el cierre de un determinado descriptor.
 * Retorna TRUE en caso de exito, FALSE caso contrario
 * @PARAMS:
 *		ptrSocket	: El descriptor a cerrar
 */
Boolean socketDestroy(Socket *ptrSocket){

	if(close(ptrSocket->descriptor) == 0) {
		return TRUE;
	}
	return FALSE;
}


Boolean handshake(SocketClient* client, Char id) {
	SocketBuffer* sb;
	StrCpuKer* sck;
	StrConKer* sconk;
	StrKerUmc* sku;
	StrUmcKer* suk;
	StrKerCon* skcon;
	StrKerCpu* skc;
	pcb* pcb = newEmptyPcb();
	Char action;
	switch (id) {
		case CPU_ID:
			sck = newStrCpuKer(id, HANDSHAKE, *pcb, 0, 0, 0, NULL, 0);
			sb = serializeCpuKer(sck);
			//newStrCpuKer(Char id, Char action, pcb pcb, Int32U pid, Int32U logLen, Byte* log)
			break;
		case CONSOLA_ID:
			sconk = newStrConKer(id, HANDSHAKE, NULL, 0);
			sb = serializeConKer(sconk);
			break;
		case KERNEL_ID:
			sku = newStrKerUmc(id, HANDSHAKE, NULL, 0, 0, 0, 0, 0, 0);
			sb = serializeKerUmc(sku);
			break;
		default:
			break;
	}
	
	if(!socketSend(client->ptrSocket, sb)) {
		printf("No se pudo realizar el handshake. \n");
		return FALSE;
	}
	
	sb = socketReceive(client->ptrSocket);
	if (sb == NULL) {
		return FALSE;
	} else {
		switch(id) {
			case CPU_ID:
				skc = unserializeKerCpu((Stream) sb->data);
				action = skc->action;
				break;
			case CONSOLA_ID:
				skcon = unserializeKerCon((Stream) sb->data);
				action = skcon->action;
				break;
			case KERNEL_ID:
				suk = unserializeUmcKer((Stream) sb->data);
				action = suk->action;
				break;
			default:
				break;
		}
		
		if (action == HANDSHAKE) {
			return TRUE;
		} else {
			return FALSE;
		}
	}
}

