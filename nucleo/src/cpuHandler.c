#include "cpuHandler.h"
#include "sys/select.h"
#include <pthread.h>
#include <commons/log.h>
#include <stdlib.h>

pthread_t cpuht;
void** nada;

fd_set master;
fd_set read_fds;

t_log* cpuhlog;

int fdmax;

void cpuHandlerThread(){
	// EL HILO VA A HACER SUS COSAS SIEMPRE QUE EL PROCESO PADRE
	// SIGA VIVO

	pthread_create(&cpuht, NULL, cpuHandlerThreadRoutine, "nada");

}


void* cpuHandlerThreadRoutine(void* parametro){

	cpuhlog = log_create("cpuh.log", "CPUHANDLER", TRUE, LOG_LEVEL_INFO);

	if(initCpuServer()){
		log_info(cpuhlog, "Server iniciado.");
	}
	else{
		log_error(cpuhlog, "No se pudo iniciar el server.");
	}


	checkCpuConnections();


	// CAMBIAR ESTO POR list_dest_elem CON UNA FUNCION QUE HAGA
	// FREE DE CADA MALLOC
	list_destroy(coreList);

	// CIERRO EL LOG
	log_destroy(cpuhlog);

	return NULL;
}


Boolean initCpuServer() {

//	INICIO LA LISTA DE CPUS
	coreList = list_create();
	if(coreList == NULL){
		log_error(cpuhlog, "Error al crear la lista de CPUs.");
		return FALSE;
	}


//	INICIO EL SOCKET ESCUCHA
	serverSocket = socketCreateServer(CPU_HANDLER_SOCKET);
	if (serverSocket == NULL) {
		log_error(cpuhlog, "No se pudo crear el server escucha.");
		return FALSE;
	}
	if (!socketListen(serverSocket)) {
		log_error(cpuhlog, "No se pudo poner a escuchar al server.");
	}
	log_info(cpuhlog, "Server creado con exito y escuchando.");
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

void newClientHandler(Socket* client) {

	log_info(cpuhlog, "Nueva Conexion detectada, realizando handshake");

	//ADMINISTRO LA NUEVA CONEXION Y REALIZO EL HANDSHAKE
	SocketBuffer* buffer = socketReceive(client);
	if(buffer == NULL) {
		log_error(cpuhlog, "Error al recibir de Cliente");
	} else {
		Stream strReceived = (Stream) buffer->data;
		Char id = getStreamId(strReceived);

		log_info(cpuhlog, "ID Nuevo Cliente: %d.",id);
		switch (id) {

		case CPU_ID:
			log_info(cpuhlog, "Nuevo Cliente CPU");
			newCpuClient(client,strReceived);
			break;

		case CONSOLA_ID:
			log_info(cpuhlog, "Nuevo Cliente CONSOLA");
//			newConsoleClient(client,strReceived);
			break;

		}
	}
}

void newCpuClient(Socket* cpuClient, Stream dataSerialized) {

	t_core *cpu;
	StrCpuKer* sck = unserializeCpuKer(dataSerialized); //ESTO ESTA ROMPIENDO
	puts("Data deserializada.");
	StrKerCpu* skc;
	SocketBuffer* sb;
	pcb pcb;
	switch(sck->action) {
		case HANDSHAKE:
			log_debug(cpuhlog, "KER-CPU: HANDSHAKE recibido");
			skc = newStrKerCpu(CPU_ID, HANDSHAKE, pcb, 0);
			sb = serializeKerCpu(skc);
			if (socketSend(cpuClient, sb)) {
				log_debug(cpuhlog, "KER-CPU: HANDSHAKE enviado");
				//AGREGARLO A LA LISTA DE DESCRIPTORES DE CPU  DEL PLANIFICADOR
				cpu = malloc(sizeof(cpu));
				cpu->cpuClient = cpuClient;
				cpu->pcb = NULL;

				//LOG OBLIGATORIO
//				conexion_cpu(cpuClient->descriptor);

//				mtxLock(&mtxCpuList);
				list_add(coreList, cpu);
//				mtxUnlock(&mtxCpuList);
				//ACTIVO LA LISTA DE CPUS PARA EL PLANIFICADOR
//				semSignal(&semCpuList);
			} else {
				log_error(cpuhlog, "KER-CPU: HANDSHAKE fallo al devolver");
			}

			break;
		default:
			log_error(cpuhlog, "Nueva CPU no puedo hacer el Handshake");
			break;
	}
}


void clientHandler(Socket* cliente){

//	int cpuNodo;
//	cpuNodo = cpuCoreInList(coreList, cliente);
//	t_core* datos = malloc(sizeof(t_core));
//
//	SocketBuffer* buffer;
//
//	buffer = socketReceive(cliente);
//
//	if(buffer->data[0] == 'd'){
//		puts("Una CPU se ha disponinbilizado.");
//	}

}



int cpuCoreInList(t_list* lista, Socket* cliente){

	int index;  // NO ESTOY SEGURO DE SI ARRANCA EN 0 O EN 1, PUEDE TRAER PROBLEMAS
	int ultimoIndex = list_size(lista) -1 ;
	t_core* data;

	for(index = 0; index < ultimoIndex; index = index + 1){
		data = (t_core*)list_get(lista, index);
		if(data->cpuClient == cliente){
			return index;
		}
	}
	return -1;
}
