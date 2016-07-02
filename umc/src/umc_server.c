#include "umc_server.h"

#include "sys/select.h"
#include <pthread.h>
#include <commons/log.h>
#include <stdlib.h>

pthread_t umcst;
void** nada;

fd_set master;
fd_set read_fds;

t_log* umcslog;

int fdmax;


/*
 *
 * THREAD DEL UMCSERVER
 *
 */

void UmcServerThread(){
	// EL HILO VA A HACER SUS COSAS SIEMPRE QUE EL PROCESO PADRE
	// SIGA VIVO

	pthread_create(&umcst, NULL, UmcServerThreadRoutine, "nada");

}

void* UmcServerThreadRoutine(void* parametro){

	umcslog = log_create("umcs.log", "UMCSERVER", TRUE, LOG_LEVEL_INFO);

	if(initServer()){
		log_info(umcslog, "Server iniciado.");
	}
	else{
		log_error(umcslog, "No se pudo iniciar el server.");
	}


	checkConnections();


	// CAMBIAR ESTO POR list_dest_elem CON UNA FUNCION QUE HAGA
	// FREE DE CADA MALLOC
	list_destroy(coreList);

	// CIERRO EL LOG
	log_destroy(umcslog);

	return NULL;
}

Boolean initServer() {

//	INICIO EL SOCKET ESCUCHA
	serverSocket = socketCreateServer(CPU_HANDLER_SOCKET);
	if (serverSocket == NULL) {
		log_error(umcslog, "No se pudo crear el server escucha.");
		return FALSE;
	}
	if (!socketListen(serverSocket)) {
		log_error(umcslog, "No se pudo poner a escuchar al server.");
	}
	log_info(umcslog, "Server creado con exito y escuchando.");
	return TRUE;
}

/*
 *
 * EL PUTO SELEC
 *
 */

void checkConnections() {

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

/*
 *
 * PROCEDIMIENTOS PARA NUEVOS CLIENTES
 *
 */

void newClientHandler(Socket* client) {

	log_info(umcslog, "Nueva Conexion detectada, realizando handshake");

	//ADMINISTRO LA NUEVA CONEXION Y REALIZO EL HANDSHAKE
	SocketBuffer* buffer = socketReceive(client);
	if(buffer == NULL) {
		log_error(umcslog, "Error al recibir de Cliente");
	} else {
		Stream strReceived = (Stream) buffer->data;
		Char id = getStreamId(strReceived);

		log_info(umcslog, "ID Nuevo Cliente: %d.",id);
		switch (id) {

		case CPU_ID:
			log_info(umcslog, "Nuevo Cliente CPU");
			newCpuClient(client,strReceived);
			break;

		case KERNEL_ID:
			log_info(umcslog, "Nuevo Cliente KERNEL");
			newKernelClient(client,strReceived);
			break;

		}
	}
}

void newCpuClient(Socket* cpuClient, Stream dataSerialized) {

	t_core *cpu;
	StrCpuUmc* scu = unserializeCpuUmc(dataSerialized); //ESTO ESTA ROMPIENDO
	puts("Data deserializada.");
	printf("Action: %d\n", scu->action);
	StrUmcCpu* suc;
	SocketBuffer* sb;
	espacioAsignado ea;
	pcb pcb;
	switch(scu->action) {
		case HANDSHAKE:
			log_info(umcslog, "UCM-CPU: HANDSHAKE recibido");
			suc = newStrUmcCpu(CPU_ID, HANDSHAKE, (espacioAsignado) ea, 0, 0, 0, 0);
			sb = serializeKerCpu(suc);
			if (socketSend(cpuClient, sb)) {
				log_info(umcslog, "UMC-CPU: HANDSHAKE enviado");
			} else {
				log_error(umcslog, "UMC-CPU: HANDSHAKE fallo al devolver");
			}
			break;
		default:
			log_error(umcslog, "Nueva CPU no puedo hacer el Handshake");
			break;
	}
}

void newKernelClient(Socket* kernelClient, Stream dataSerialized) {

	StrKerUmc* sku = unserializeKerUmc(dataSerialized);
	StrUmcKer* suk;
	SocketBuffer* sb;
	if(sku->action == HANDSHAKE) {
		log_info(umcslog, "Nuevo Cliente Kernel %d aceptado.",kernelClient->descriptor);
		suk = newStrKerCon(KERNEL_ID, HANDSHAKE, 0,0);
		sb = serializeKerCon(suk);
		if (socketSend(kernelClient, sb)) {
			log_info(umcslog, "UMC-KER: HANDSHAKE se devolvio handshake");
		} else {
			log_error(umcslog, "UMC-KER: HANDSHAKE fallo al devolver");
		}
	} else {
		log_error(umcslog, "No se pudo determinar la accion de un cliente Kernel.");
	}

}

/*
 *
 * PROCDIMIENTOS PARA CLIENTES CONOCIDOS
 *
 */

void clientHandler(int clientDescriptor) {

	Socket* tempClient = malloc(sizeof(Socket));
	tempClient->descriptor = clientDescriptor;

	SocketBuffer* buffer = socketReceive(tempClient);

	if (buffer == NULL) {

		//SE CALLO UN CLIENTE
		log_info(umcslog, "KERNEL : Se cayo el cliente %d", clientDescriptor);

		//REALIZO EL PROCEDIMIENTO DE CAIDA DE UN CLIENTE
//		clientDown(clientDescriptor);

		//ELIMINO EL DESCRIPTOR DEL CONJUNTO
		FD_CLR(clientDescriptor, &master);
		log_info(umcslog, "KERNEL : Se elimina el cliente %d de la bolsa de descriptores", clientDescriptor);

		//CIERRO EL SOCKET
		socketDestroy(tempClient);
		free(tempClient);
		log_info(umcslog, "KERNEL : Se elimina el descriptor %d para ese cliente", clientDescriptor);

	} else {

		// ** SE ATIENDEN SOLICITUDES DE CLIENTES **
		log_info(umcslog, "KERNEL : El cliente %d ha enviado un paquete", clientDescriptor);

		Stream strReceived = (Stream) buffer->data;
		Char id = getStreamId(strReceived);

		switch (id) {

		case CPU_ID:
			log_info(umcslog, "KERNEL : El cliente %d es un CPU", clientDescriptor);
			cpuClientHandler(tempClient, strReceived);
			break;

		case KERNEL_ID:
			log_info(umcslog, "KERNEL : El cliente %d es una consola", clientDescriptor);
			kernelClientHandler(tempClient, strReceived);
			break;

		}
	}
}

void cpuClientHandler(Socket* cpuClient, Stream data) {

	pcb* aux;
//	t_hilo* pcbClipboard;
	StrCpuUmc* scu = unserializeCpuUmc(data);

	switch (scu->action) {

	// ACA VA EL RECONOCIMIENTO DE ACCIONES

	default:
		log_error(umcslog, "UMC: CPU %d ha enviado un action incomprensible",cpuClient->descriptor);
		break;

	}

}

void kernelClientHandler(Socket *consoleClient, Stream data){
	pcb *pcb;
	Console *console;
	StrKerUmc *sku = unserializeKerUmc(data);
	Byte *ptrByte;
	switch (sku->action) {

	default:
		log_error(umcslog, "UMC: No se pudo determinar el action del cliente KERNEL");
		log_error(umcslog, "UMC : Action = %d",sku->action);
		break;
	}
}
