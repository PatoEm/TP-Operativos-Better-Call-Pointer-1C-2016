/*
 * Socket.c
 *
 *  Created on: 22/4/2016
 *      Author: utnso
 */

#include "socket.h"


#define PACKAGESIZE 1024

//Cliente MotherFucker

int crearCliente(char ipServidor[], char puertoServidor[])//Creo un socket y le paso la ip y puerto del servidor así sabe a donde conectarse
{


	struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		// Permite que la maquina se encargue de verificar si usamos IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM;	// Indica que usaremos el protocolo TCP

	getaddrinfo(ipServidor, puertoServidor, &hints, &serverInfo);	// Carga en serverInfo los datos de la conexion

	int serverSocket;
	serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);


	connect(serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen);
	printf("Conectado al servidor \n");
	freeaddrinfo(serverInfo);	// No lo necesitamos mas
	return serverSocket;

}

void enviarMensaje(int socketCliente, char message[PACKAGESIZE])
{
	//int enviar = 1;
	//char message[PACKAGESIZE];

	/*
	while(enviar){
		fgets(message, PACKAGESIZE, stdin);			// Lee una linea en el stdin (lo que escribimos en la consola) hasta encontrar un \n (y lo incluye) o llegar a PACKAGESIZE.
		if (!strcmp(message,"exit\n")) enviar = 0;			// Chequeo que el usuario no quiera salir
		if (enviar) send(serverSocket, message, strlen(message) + 1, 0); 	// Solo envio si el usuario no quiere salir.
	}
	*/
	//strcpy(message,"Hola");

	send(socketCliente, message, strlen(message) + 1, 0);


	printf("Envie el mensaje: %s \n",message);


	//
}
void recibirMensaje(int socketCliente, int tamanioPaquete)
{
	open(socketCliente);
	char message[tamanioPaquete];
	recv(socketCliente, (void*) message, strlen(message)+1, 0);

}

void cerrarSocket (int socket)
{
	close(socket);

}

