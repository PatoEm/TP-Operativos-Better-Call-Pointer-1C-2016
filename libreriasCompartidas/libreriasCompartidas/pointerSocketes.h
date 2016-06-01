/*
 * pointerSocketes.h
 *
 *  Created on: 31/5/2016
 *      Author: utnso
 */

#ifndef LIBRERIASCOMPARTIDAS_POINTERSOCKETES_H_
#define LIBRERIASCOMPARTIDAS_POINTERSOCKETES_H_

#include "pointerTipos.h"

#define MAX_CONEXIONES 10
#define BUFF_SIZE 4096

//TIPOS DE DATOS
//=================================================================
typedef struct {
	Int32U 				descriptor;
	struct sockaddr_in *ptrAddress;
} Socket;

typedef struct {
	Socket *ptrSocket;
	Socket *ptrSocketServer;
} SocketClient;

typedef struct {
	Char 	data[BUFF_SIZE];
	Int32U 	size;
} SocketBuffer ;

//=================================================================

//FUNCIONES DE MANEJO DE SOCKETS
//=================================================================
Socket* socketCreateServer(Int32U port);
SocketClient* socketCreateClient();

Boolean socketListen(Socket *ptrSocket);
Boolean socketConnect(SocketClient *ptrSocketClient, String ptrServerAddress, Int32U serverPort);

Socket* socketAcceptClient(Socket *ptrListenSocket);

SocketBuffer *socketReceive(Socket *ptrSender);
Boolean socketSend(Socket *ptrDestination, SocketBuffer *ptrBuffer);

Boolean socketDestroy(Socket *ptrSocket);
SocketClient* socketClientMock(String, String);
Boolean handshake(SocketClient* client, Char id);
//=================================================================

#endif /* LIBRERIASCOMPARTIDAS_POINTERSOCKETES_H_ */
