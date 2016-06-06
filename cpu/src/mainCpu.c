/*
 ============================================================================
 Name        : cpu.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "cpu.h"
#include "cpuHandler.h"

Socket* serverSocket;

int main() {
	leerArchivoDeConfiguracion("configcpu");
	puts("Hello world!!!");

//	serverSocket = socketCreateServer(2020);

	if(initServer()){
	puts("Server iniciado");
	}
	else{
		puts("Server no iniciado");
	}


	checkConnections();
	puts("Connections Check");


	return EXIT_SUCCESS;

}
