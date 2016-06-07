#include "cpuHandler.h"
#include "sys/select.h"
#include <pthread.h>

pthread_t cpuht;
void** nada;

fd_set master;
fd_set read_fds;

int fdmax;

void cpuHandlerThread(){
	// EL HILO VA A HACER SUS COSAS SIEMPRE QUE EL PROCESO PADRE
	// SIGA VIVO

	pthread_create(&cpuht, NULL, cpuHandlerThreadRoutine, "nada");

}


void* cpuHandlerThreadRoutine(void* parametro){
	if(initCpuServer()){
	puts("Server iniciado");
	}
	else{
		puts("Server no iniciado");
	}


	checkCpuConnections();
	puts("Connections Check");

	return "nada";
}


Boolean initCpuServer() {

//	INICIO LA LISTA DE CPUS
	coreList = list_create();
	if(coreList == NULL){
		puts("Error al crear la lista de CPUs.");
		return FALSE;
	}


//	INICIO EL SOCKET ESCUCHA
	serverSocket = socketCreateServer(CPU_HANDLER_SOCKET);
	if (serverSocket == NULL) {
		puts("No se pudo crear el server escucha.");
		return FALSE;
	}
	if (!socketListen(serverSocket)) {
		puts("No se pudo poner a escuchar al server.");
	}
	puts("Server creado con exito y escuchando.");
	return TRUE;
}




void checkCpuConnections() {

	//SETEO A "0" EL MASTER Y EL TEMPORAL
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	//CARGO EL SOCKET DE ESCUCHA A MASTER
	FD_SET(serverSocket->descriptor, &master);

	//CARGO EL SOCKET MAS GRANDE
	fdmax = serverSocket->descriptor;

	struct timeval timeout;

	//BUCLE PRINCIPAL
	while (TRUE) {

		read_fds = master;
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;

		//HAGO EL SELECT
		int selectResult = select(fdmax + 1, &read_fds, NULL, NULL, &timeout);

		if (selectResult == -1) {

			break;


		} else {

			//RECORRO TODOS LOS DESCRIPTORES MONITOREADOS PARA VER QUIEN LLAMO
			int i;
			for (i = 0; i <= fdmax; i++) {

				if (FD_ISSET(i, &read_fds)) {

					//FUE EL SOCKET DE ESCUCHA??
					if (i == serverSocket->descriptor) {

						//SI, ENTONCES GESTIONO LA NUEVA CONEXION ENTRANTE
						Socket* clientSocket = socketAcceptClient(serverSocket);
						newClientHandler(clientSocket);

						//LO CARGO A LA LISTA DE DESCRIPTORES A MONITOREAR Y ACTUALIZO EL MAXIMO
						FD_SET(clientSocket->descriptor, &master);

						if (clientSocket->descriptor > fdmax) {
							fdmax = clientSocket->descriptor;
						}

					} else {
						//NO, ENTONCES GESTIONO EL SOCKET QUE HABLO...
						clientHandler((Socket*)i);
					}
				}
			}
		}
	}
}

void newClientHandler(Socket* cliente){

	SocketBuffer* buffer;
	t_core* datos;
	t_core core;
	buffer = socketReceive(cliente);

	core.busy = TRUE;
	core.socket = cliente;

	if(buffer == NULL){
		puts("Error al recibir información del cliente.");
	} else{
		if(buffer->data[0] == 'a'){
			puts("Se ha conectado una nueva CPU.");

			// AGREGAR CPU A LA LISTA
			datos = malloc(sizeof(datos));
			*datos = core;



		} else {
			puts("El nuevo cliente dice cosas raras :/");
		}
	}

}

void clientHandler(Socket* cliente){



}
