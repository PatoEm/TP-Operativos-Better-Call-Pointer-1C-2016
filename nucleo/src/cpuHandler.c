#include "cpuHandler.h"
#include "sys/select.h"
#include <pthread.h>
#include <commons/log.h>
#include <stdlib.h>
#include "nucleo.h"

pthread_t cpuht;
void** nada;

fd_set master;
fd_set read_fds;

t_log* cpuhlog;

int fdmax;

/*
 *
 * THREAD DEL CPUHANDLER
 *
 */

void cpuHandlerThread() {
	// EL HILO VA A HACER SUS COSAS SIEMPRE QUE EL PROCESO PADRE
	// SIGA VIVO

	pthread_create(&cpuht, NULL, cpuHandlerThreadRoutine, "nada");

}

void* cpuHandlerThreadRoutine(void* parametro) {

	cpuhlog = log_create("cpuh.log", "CPUHANDLER", TRUE, LOG_LEVEL_INFO);

	if (initCpuServer()) {
		log_info(cpuhlog, "Server iniciado.");
	} else {
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
	if (coreList == NULL) {
		log_error(cpuhlog, "Error al crear la lista de CPUs.");
		return FALSE;
	}

//	INICIO LA LISTA DE CONSOLAS
	consoleList = list_create();
	if (consoleList == NULL) {
		log_error(cpuhlog, "Error al crear la lista de Consolas.");
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

/*
 *
 * EL PUTO SELEC
 *
 */

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
						clientHandler((Socket*) i);
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

	log_info(cpuhlog, "Nueva Conexion detectada, realizando handshake");

	//ADMINISTRO LA NUEVA CONEXION Y REALIZO EL HANDSHAKE
	SocketBuffer* buffer = socketReceive(client);
	if (buffer == NULL) {
		log_error(cpuhlog, "Error al recibir de Cliente");
	} else {
		Stream strReceived = (Stream) buffer->data;
		Char id = getStreamId(strReceived);

		log_info(cpuhlog, "ID Nuevo Cliente: %d.", id);
		switch (id) {

		case CPU_ID:
			log_info(cpuhlog, "Nuevo Cliente CPU");
			newCpuClient(client, strReceived);
			break;

		case CONSOLA_ID:
			log_info(cpuhlog, "Nuevo Cliente CONSOLA");
			newConsoleClient(client, strReceived);
			break;

		}
	}
}

void newCpuClient(Socket* cpuClient, Stream dataSerialized) {

	t_core *cpu;
	StrCpuKer* sck = unserializeCpuKer(dataSerialized); //ESTO ESTA ROMPIENDO
	puts("Data deserializada.");
	printf("Action: %d\n", sck->action);
	StrKerCpu* skc;
	SocketBuffer* sb;
	pcb pcb;
	switch (sck->action) {
	case HANDSHAKE:
		log_info(cpuhlog, "KER-CPU: HANDSHAKE recibido");
		skc = newStrKerCpu(CPU_ID, HANDSHAKE, pcb, 0);
		sb = serializeKerCpu(skc);
		if (socketSend(cpuClient, sb)) {
			log_info(cpuhlog, "KER-CPU: HANDSHAKE enviado");
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

void newConsoleClient(Socket* consoleClient, Stream dataSerialized) {

	StrConKer* sck = unserializeConKer(dataSerialized);
	StrKerCon* skc;
	SocketBuffer* sb;
	if (sck->action == HANDSHAKE) {
		log_info(cpuhlog, "Nuevo Cliente Consola %d aceptado.",
				consoleClient->descriptor);
		skc = newStrKerCon(CONSOLA_ID, HANDSHAKE, 0, 0);
		sb = serializeKerCon(skc);
		if (socketSend(consoleClient, sb)) {
			log_info(cpuhlog, "KER-CON: HANDSHAKE se devolvio handshake");
			//LOG OBLIGATORIO
			//conexion_consola(consoleClient->descriptor);
			list_add(consoleList, consoleClient);
		} else {
			log_error(cpuhlog, "KER-CON: HANDSHAKE fallo al devolver");
		}
	} else {
		log_error(cpuhlog,
				"No se pudo determinar la accion de un cliente Consola.");
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
		log_info(cpuhlog, "KERNEL : Se cayo el cliente %d", clientDescriptor);

		//REALIZO EL PROCEDIMIENTO DE CAIDA DE UN CLIENTE
//		clientDown(clientDescriptor);

		//ELIMINO EL DESCRIPTOR DEL CONJUNTO
		FD_CLR(clientDescriptor, &master);
		log_info(cpuhlog,
				"KERNEL : Se elimina el cliente %d de la bolsa de descriptores",
				clientDescriptor);

		//CIERRO EL SOCKET
		socketDestroy(tempClient);
		free(tempClient);
		log_info(cpuhlog,
				"KERNEL : Se elimina el descriptor %d para ese cliente",
				clientDescriptor);

	} else {

		// ** SE ATIENDEN SOLICITUDES DE CLIENTES **
		log_info(cpuhlog, "KERNEL : El cliente %d ha enviado un paquete",
				clientDescriptor);

		Stream strReceived = (Stream) buffer->data;
		Char id = getStreamId(strReceived);

		switch (id) {

		case CPU_ID:
			log_info(cpuhlog, "KERNEL : El cliente %d es un CPU",
					clientDescriptor);
			cpuClientHandler(tempClient, strReceived);
			break;

		case CONSOLA_ID:
			log_info(cpuhlog, "KERNEL : El cliente %d es una consola",
					clientDescriptor);
			consoleClientHandler(tempClient, strReceived);
			break;

		}
	}
}

void enviarPcbACpu(Socket * cpuClient) {

	if (queue_is_empty(colaReady) != 0) {
		pthread_mutex_lock(mutexColaReady);
		pcb* pcbAEnviar = (pcb*) queue_pop(colaReady);
		pthread_mutex_unlock(mutexColaReady);

		pthread_mutex_lock(mutexQuantum);
		StrKerCpu* skc = newStrKerCpu(KERNEL_ID, 0, *pcbAEnviar, quantum);
		pthread_mutex_unlock(mutexQuantum);

		SocketBuffer* sb = serializeKerCpu(skc);
		if (!socketSend(cpuClient->ptrAddress, sb)) {
			printf("No se pudo enviar el stream al cpu");
		} else {
			printf("Se envio el pcb del programa %d al cpu", pcbAEnviar->id);
		}
		free(sb);
	}
}

void cpuClientHandler(Socket* cpuClient, Stream data) {

	pcb* aux;
//	t_hilo* pcbClipboard;
	StrCpuKer* sck = unserializeCpuKer(data);

	if (sck->action == PRIMER_PCB) {

		/*
		 * ACA NO SE HACE NADA, SI EL PLANIFICADOR TIENE TCBS PARA ENVIAR, LOS ENVIA
		 * SI NO, LA CPU VA A TENER QUE ESPERAR. PARA ESTE PUNTO, EL PLANIFICADOR YA SABE
		 * QUE EXISTE ESTA CPU
		 */

		enviarPcbACpu(cpuClient);

		log_info(cpuhlog, "KERNEL : CPU %d ha enviado FIRST_PCB",
				cpuClient->descriptor);
		return;
	}

//	pcbClipboard = copyPcbToClipboard(&(sck->pcb));

	switch (sck->action) {
	//case WAIT_SEM_ANSISOP:

	//todo acá debería estar el nombre de que semaforo modificar

	//break;
	// ACA VA EL RECONOCIMIENTO DE ACCIONES

	default:
		log_error(cpuhlog,
				"KERNEL : CPU %d ha enviado un action incomprensible",
				cpuClient->descriptor);
		break;

	}

}

void consoleClientHandler(Socket *consoleClient, Stream data) {
	pcb *pcb;
	Console *console;
	StrConKer *sck = unserializeConKer(data);
	Byte *ptrByte;
	switch (sck->action) {
//		case STD_INPUT:
//
//			log_debug(getLogger(), "KERNEL : El Cliente %d mando STD_INPUT",consoleClient->descriptor);
//			console = getConsoleByTid(sck->tid);
//
//			SysCall *scd = getSysCallByTid(console->tcb->tid);
//			if(scd != NULL){
//
//				scd->dataLength = sck->bufferWriterLen;
//				scd->data = malloc(sizeof(sck->bufferWriterLen));
//				ptrByte = (Byte*) &sck->bufferWriter;
//				memcpy(&scd->data,ptrByte,sck->bufferWriterLen);
//
//				wakeTcbForSTDINComplete(console->tcb->tid);
//			}
//
//			break;

	case STD_OUTPUT:

		log_error(cpuhlog, "KERNEL : El Cliente %d mando STD_OUTPUT",
				consoleClient->descriptor);
		//SI CONSOLA ME MANDA OUT, NO PASA NADA, PORQUE EL TCB NO CAMBIA
		//Y YA ESTA DESBLOQUEADO. ACA NO DEBERIA DE HACER NADA EL KERNEL
		break;

	case ARCHIVO_ANSISOP:

		log_error(cpuhlog, "KERNEL : El Cliente %d mando ANSISOP",
				consoleClient->descriptor);

		//SE GENERA EL NUEVO PCB
		pcb = crearNuevoPcb(sck->fileContent, sck->fileContentLen);
		puts("hola");			//createNewPcb(sck);
//			log_info(cpuhlog, "KERNEL : El proceso %d comenzara",pcb->id);

		//CARGO LA CONSOLA INGRESADA JUNTO CON EL PCB QUE
		//TRAJO A LA LISTA DE CONSOLAS QUE VOY A USAR PARA GESTIONAR
		//LOS SERVICIOS EXPUESTOS A LA CPU
		Console *clientConsole = malloc(sizeof(Console));

		clientConsole->consoleClient = consoleClient;
		clientConsole->pcb = pcb;
		clientConsole->data = sck->fileContent;
		clientConsole->dataLength = sck->fileContentLen;

//			mtxLock(&mtxConsoleList);
		list_add(consoleList, clientConsole);

//			mtxUnlock(&mtxConsoleList);
		log_error(cpuhlog, "KERNEL : Consola %d añadida a la lista",
				consoleClient->descriptor);

		//MUEVO EL NUEVO PCB A LA COLA DE NEW
//			newProcessesHandlerThread(pcb);

		break;

	default:
		log_error(cpuhlog,
				"KERNEL : No se pudo determinar el action del cliente CONSOLA");
		log_error(cpuhlog, "KERNEL : Action = %d", sck->action);
		break;
	}
}

