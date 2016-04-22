/*
 * Socket.c
 *
 *  Created on: 22/4/2016
 *      Author: utnso
 */

#include "socket.h"

#define PACKAGESIZE 1024

//Cliente MotherFucker

int crearCliente(const char * ipServidor, int puertoServidor[]) //Creo un socket y le paso la ip y puerto del servidor así sabe a donde conectarse
{

	struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // Permite que la maquina se encargue de verificar si usamos IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM;	// Indica que usaremos el protocolo TCP

	getaddrinfo(ipServidor, puertoServidor, &hints, &serverInfo);// Carga en serverInfo los datos de la conexion

	int serverSocket;
	serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype,
			serverInfo->ai_protocol);

	int conexion = connect(serverSocket, serverInfo->ai_addr,
			serverInfo->ai_addrlen);
	if (conexion == ERRORPUTO) {

	} else {
		printf("Conectado al servidor \n");
	}

	freeaddrinfo(serverInfo);	// No lo necesitamos mas
	return serverSocket;

}

void enviarMensaje(int fdSocketCliente, const void * message,
		int tamanioPaquete) {

	int bytes_enviados = send(fdSocketCliente, message, tamanioPaquete, 0);
	if (bytes_enviados == ERRORPUTO) {
		printf("Fallo el send");
	}else {
		printf("Mensaje enviado \n");
	}

	printf("Envie el mensaje: %s \n", message);

}
void recibirMensaje(int fdSocketCliente, void * buffer, int tamanioPaquete) {

	int bytes_recibidos = recv(fdSocketCliente, buffer, tamanioPaquete, 0);
	if (bytes_recibidos == ERRORPUTO) {
		printf("Fallo el recv");
	}else {
		printf("Mensaje recibido \n");
	}

}

void cerrarSocket(int fdSocket) {
	int cerrar = close(fdSocket);
	if (cerrar == ERRORPUTO) {
		printf("Fallo el close");
	}else {
		printf("Socket cerrado \n");
	}
}

