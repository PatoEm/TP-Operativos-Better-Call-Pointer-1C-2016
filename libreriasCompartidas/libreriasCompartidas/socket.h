/*
 * socket.h
 *
 *  Created on: 22/4/2016
 *      Author: utnso
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     //memset
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>      //perror
#include <arpa/inet.h>  //INADDR_ANY
#include <unistd.h>     //close  usleep
#include <netdb.h> 		//gethostbyname
#include <netinet/in.h>
#include <fcntl.h> //fcntl

#define ERRORPUTO -1

//#define PUERTO "6667"
//#define PUERTO2 "6668"

#define BACKLOG 5			// Define cuantas conexiones vamos a mantener pendientes al mismo tiempo
#define PACKAGESIZE 1024

int crearCliente(const char * ipServidor, const char * puertoServidor);

int crearSocketServidor(const char * puerto);

void enviarMensaje(int fdSocketCliente, const void * message,
		int tamanioPaquete);

void recibirMensaje(int fdSocketCliente, void * buffer, int tamanioPaquete);

void cerrarSocket(int fdSocket);

#endif /* SOCKET_H_ */
