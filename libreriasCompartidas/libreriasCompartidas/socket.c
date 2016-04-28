/*
 * Socket.c
 *
 *  Created on: 22/4/2016
 *      Author: utnso
 */

#include "socket.h"

//Cliente MotherFucker

int crearCliente(const char * ipServidor, const char * puertoServidor) //Creo un socket y le paso la ip y puerto del servidor así sabe a donde conectarse
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
	puts("Intentando conectarse...");
	int conexion = connect(serverSocket, serverInfo->ai_addr,
			serverInfo->ai_addrlen);
	if (conexion == ERRORPUTO) {
		puts("Error al conectarse");
	} else {
		puts("Conectado al servidor");
	}

	freeaddrinfo(serverInfo);	// No lo necesitamos mas
	return serverSocket;

}

//Servidor

int crearSocketServidor(const char * puerto) {

	struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		// No importa si uso IPv4 o IPv6
	hints.ai_flags = AI_PASSIVE;// Asigna el address del localhost: 127.0.0.1
	hints.ai_socktype = SOCK_STREAM;	// Indica que usaremos el protocolo TCP
	puts("1");
	getaddrinfo(NULL, puerto, &hints, &serverInfo); // Notar que le pasamos NULL como IP, ya que le indicamos que use localhost en AI_PASSIVE
	puts("2");
	int listenningSocket;
	listenningSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype,
			serverInfo->ai_protocol);

	puts("3");
	int pruebabind = bind(listenningSocket, serverInfo->ai_addr,
			serverInfo->ai_addrlen);
	puts("4");
	if (pruebabind == ERRORPUTO) {
		puts("Fallo el bind, pero puede ser que fallar esté bien");
	} else {
		puts("Bind Ok");

	}

	freeaddrinfo(serverInfo); // Ya no lo vamos a necesitar

	return listenningSocket;

}

void escucharSocket(int fdSocketServidor, int conexionesPermitidas) {
	int pruebalisten = listen(fdSocketServidor, conexionesPermitidas);// IMPORTANTE: listen() es una syscall BLOQUEANTE.
	if (pruebalisten == ERRORPUTO) {
		puts("Fallo el listen");
	} else {
		puts("Listen Ok");
	}

}

int aceptarConexiones(int fdSocketServidor) {

	struct sockaddr_in addr;// Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc.
	socklen_t addrlen = sizeof(addr);

	int fdSocketCliente = accept(fdSocketServidor, (struct sockaddr *) &addr,
			&addrlen);
	if (fdSocketCliente == ERRORPUTO) {
		puts("Fallo el accept");
	} else {
		puts("Cliente aceptado");
	}

	return fdSocketCliente;
}

//Enviar y Recibir Mensajes

void enviarMensaje(int fdSocketCliente, const void * message,
		int tamanioPaquete) {

	int bytes_enviados = send(fdSocketCliente, message, tamanioPaquete, 0);
	if (bytes_enviados == ERRORPUTO) {
		puts("Fallo el send");
	} else {
		puts("Mensaje enviado \n");
	}

	//printf("Envie el mensaje: %s \n", message);

}
void recibirMensaje(int fdSocketCliente, void * buffer, int tamanioPaquete) {

	int bytes_recibidos = recv(fdSocketCliente, buffer, tamanioPaquete, 0);
	if (bytes_recibidos == ERRORPUTO) {
		puts("Fallo el recv");
	} else {
		puts("Mensaje recibido \n");
	}

}

//Cerrar

void cerrarSocket(int fdSocket) {
	int cerrar = close(fdSocket);
	if (cerrar == ERRORPUTO) {
		puts("Fallo el close");
	} else {
		puts("Socket cerrado \n");
	}
}

//Select



