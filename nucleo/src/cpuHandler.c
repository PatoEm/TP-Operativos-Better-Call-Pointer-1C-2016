#include "cpuHandler.h"
#include "sys/select.h"
#include <pthread.h>
#include <commons/log.h>
#include <stdlib.h>
#include "nucleo.h"
#include <libreriasCompartidas/pointerStream.h>
pthread_t cpuht;
void** nada;

fd_set master;
fd_set read_fds;

t_log* cpuhlog;

int fdmax;

pcb* pcbVacio = NULL;

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

	cpuhlog = malloc(sizeof(t_log));

	memcpy(cpuhlog, log_create("nucleo.log", "NUCLEO", TRUE, LOG_LEVEL_INFO),
			sizeof(t_log));
	//cpuhlog = log_create("cpuh.log", "CPUHANDLER", TRUE, LOG_LEVEL_INFO);

	pcbVacio = malloc(sizeof(pcb));
	memcpy(pcbVacio, newEmptyPcb(), sizeof(pcb));

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
	serverSocket = socketCreateServer(atoi(puertoPropio));
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

//		satisfacerCpuAlPedo();

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
						clientHandler((int) i);
					}
				}

			}
			//satisfacerCpuAlPedo();
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
	//pcb* pcbLoca=malloc(sizeof(pcb*));
	//pcbLoca = newEmptyPcb();
	//pcb* pcb = newEmptyPcb();
	//memcpy (pcbLoca,newEmptyPcb(),sizeof(pcb));

	switch (sck->action) {
	case HANDSHAKE:

		log_info(cpuhlog, "KER-CPU: HANDSHAKE recibido");

		//(Char id, Char action, pcb pcb, Int8U quantum,Byte* data, Int32U dataLen, Byte* nombreDispositivo,Int32U lenNomDispositivo)
		skc = newStrKerCpu(KERNEL_ID, HANDSHAKE, *pcbVacio, 0, 0, NULL, 0,
		NULL /*NOMBRE DISPOSITIVO*/, 0 /*LEN NOMBRE DISPOSITIVO*/);
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
		skc = newStrKerCon(KERNEL_ID, HANDSHAKE, 0, 0, NULL);
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
		clientDown(clientDescriptor);

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

String stringFromByteArray(Byte* data, Int32U size) {
	int i;
	String result = malloc(size);
	char* c = (char*) data;
	for (i = 0; i < size; i++) {
		result[i] = *c;
		c++;
	}
	result[size] = '\0';
	return result;
}

void cpuClientHandler(Socket* cpuClient, Stream data) {

	pcb* pcb_aux = malloc(sizeof(pcb*));
	StrCpuKer* in_cpu_msg = unserializeCpuKer(data);
	StrKerCon* out_con_msg;
	StrKerCpu* out_cpu_msg;
	StrUmcKer* in_umc_msg;
	SocketBuffer* sb;
	Socket* consola_aux;
	Byte * nombreDispositivo;
	int valor_cantidad_tiempo;
	int valor;
	char* variable;
	//StrCpuKer*stringToCpu;

	atributosIO * atributos;
	atributosWait * atributosWaitLoco;
	pthread_t hiloIO;
	pthread_attr_t attrHiloIO;
	pthread_t hiloWait;
	pthread_attr_t attrHiloWait;

	if (in_cpu_msg->action == PRIMER_PCB) {

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

	switch (in_cpu_msg->action) {

	case ASIGNAR_VALOR_COMPARTIDA:
		log_info(cpuhlog, "CPU pide: ASIGNAR_VALOR_COMPARTIDA");
		// Obtener nombre de la variable.
		variable = stringFromByteArray(in_cpu_msg->nombreDispositivo,
				in_cpu_msg->lenNomDispositivo);

		// Obtener nuevo valor
		valor = in_cpu_msg->pid;

		// Asigno el valor.
		grabar_valor(variable, valor);

		// Mando el valor asignado en el campo LEN NOMBRE DISPOSITIVO
//		(Char id, Char action, pcb pcb, Int8U quantum,
//				Byte* data, Int32U dataLen, Byte* nombreDispositivo,
//				Int32U lenNomDispositivo)

		out_cpu_msg = newStrKerCpu(KERNEL_ID, ASIGNAR_VALOR_COMPARTIDA,
				in_cpu_msg->pcb, valor, 0, NULL, 0, NULL /*NOMBRE DISPOSITIVO*/,
				0 /*LEN NOMBRE DISPOSITIVO*/);
		sb = serializeKerCpu(out_cpu_msg);

		// Envio al cpu que me hablo
		if (!socketSend(cpuClient, sb)) {
			log_error(cpuhlog, "No se asigno %d a %s.\n", valor, variable);
		} else {
			log_info(cpuhlog, "Se asigno %d a %s.\n", valor, variable);
		}

		break;

	case OBTENER_VALOR_COMPARTIDA:
		log_info(cpuhlog, "CPU pide: OBTENER_VALOR_COMPARTIDA");
		// Obtener valor de la variable.
		valor = obtener_valor(
				stringFromByteArray(in_cpu_msg->log, in_cpu_msg->logLen));

		out_cpu_msg = newStrKerCpu(KERNEL_ID, OBTENER_VALOR_COMPARTIDA,
				in_cpu_msg->pcb, valor, 0, NULL, 0, NULL /*NOMBRE DISPOSITIVO*/,
				0 /*LEN NOMBRE DISPOSITIVO*/);
		sb = serializeKerCpu(out_cpu_msg);

		// Envio al cpu que me hablo
		if (!socketSend(cpuClient, sb)) {
			log_error(cpuhlog, "No se envio el valor %d al CPU.\n", valor);
		} else {
			log_info(cpuhlog, "Se envio el valor %d al CPU.\n", valor);
		}

		break;

	case WAIT_SEM_ANSISOP:

		atributosWaitLoco = malloc(sizeof(atributosWait));

		nombreDispositivo = stringFromByteArray(in_cpu_msg->log,
				in_cpu_msg->logLen);

		atributosWaitLoco->identificador = (char*) nombreDispositivo;
		atributosWaitLoco->pcbLoca = in_cpu_msg->pcb;
		atributosWaitLoco->cpuSocket = cpuClient;

		pthread_t hiloWait;
		pthread_attr_t attrHiloWait;
		pthread_attr_init(&attrHiloWait);
		pthread_attr_setdetachstate(&attrHiloWait, PTHREAD_CREATE_DETACHED);
		pthread_create(&hiloWait, &attrHiloWait, (void*) &funcionHiloWait,
				atributosWaitLoco);
		pthread_attr_destroy(&attrHiloWait);

		break;

	case SIGNAL_SEM_ANSISOP:

		//nombreDispositivo = in_cpu_msg->log;

		nombreDispositivo = stringFromByteArray(in_cpu_msg->log,
				in_cpu_msg->logLen);

		signalAnsisop((char*) nombreDispositivo);
		confirmarCpu(cpuClient);
		break;

		// ACA VA EL RECONOCIMIENTO DE ACCIONES
	case RECIBIR_NUEVO_PROGRAMA: //TODO

		log_info(cpuhlog, "KERNEL : CPU %d ha enviado RECIBIR_NUEVO_PROGRAMA",
				cpuClient->descriptor);

//		if (enviarPcbACpu(cpuClient)) {
//			log_info(cpuhlog, "KERNEL : Programa enviado a CPU %d",
//					cpuClient->descriptor);
//		} else {
//			log_info(cpuhlog, "KERNEL : No hay trabajitos para CPU %d ",
//					cpuClient->descriptor);
//			list_add(listaCpu, (void*) cpuClient);
//			log_info(cpuhlog, "CPU al pedo añadida, LISTACPU = %d",
//					listaCpu->elements_count);
//		}

		pthread_mutex_lock(mutexListaCpu);
		list_add(listaCpu, (void*) cpuClient);
		pthread_mutex_unlock(mutexListaCpu);

		satisfacerCpuAlPedo();

		break;

	case IMPRIMIRTEXTO:
		// Creo y serializo string kernel a consola.
		out_con_msg = newStrKerCon(KERNEL_ID, IMPRIMIRTEXTO, 0,
				in_cpu_msg->logLen, in_cpu_msg->log);
		sb = serializeKerCon(out_con_msg);

		// Extraigo el socket de la respectiva consola.
		pcb_aux = &(in_cpu_msg->pcb);
		consola_aux = pcb_aux->consola;

		// Envio a la conchola
		if (!socketSend(consola_aux, sb)) {
			log_error(cpuhlog, "No se pudo mandar IMPRIMIRTEXTO a la consola.");
		} else {
			log_info(cpuhlog, "Se envio IMPRIMIRTEXTO a la consola.");
		}
		confirmarCpu(cpuClient);
		break;

	case IMPRIMIR:
		// Creo y serializo string kernel a consola.
		out_con_msg = newStrKerCon(KERNEL_ID, IMPRIMIR, in_cpu_msg->pid,
				in_cpu_msg->logLen, in_cpu_msg->log);
		sb = serializeKerCon(out_con_msg);

		// Extraigo el socket de la respectiva consola.
		pcb_aux = &(in_cpu_msg->pcb);
		consola_aux = pcb_aux->consola;

		// Envio a la conchola
		if (!socketSend(consola_aux, sb)) {
			log_error(cpuhlog, "No se pudo mandar IMPRIMIR a la consola.");
		} else {
			log_info(cpuhlog, "Se envio IMPRIMIR a la consola.");
		}
		confirmarCpu(cpuClient);
		break;

	case ENTRADA_SALIDA:

		atributos = malloc(sizeof(atributosIO));
//	 	Proba esto para obtener el string.
		nombreDispositivo = stringFromByteArray(in_cpu_msg->nombreDispositivo,
				in_cpu_msg->lenNomDispositivo);
//	  nombreDispositivo=in_cpu_msg->nombreDispositivo;
		valor_cantidad_tiempo = atoi(
				stringFromByteArray(in_cpu_msg->log, in_cpu_msg->logLen));

		atributos->identificador = (char*) nombreDispositivo;
		atributos->cantidad = valor_cantidad_tiempo;
		atributos->pcbLoca = &in_cpu_msg->pcb;

		pthread_attr_init(&attrHiloIO);
		pthread_attr_setdetachstate(&attrHiloIO, PTHREAD_CREATE_DETACHED);
		pthread_create(&hiloIO, &attrHiloIO, (void*) &funcionHiloIO, atributos);
		pthread_attr_destroy(&attrHiloIO);

		log_info(cpuhlog, "KERNEL : El CPU pidio IO.");

		confirmarCpu(cpuClient);

		break;

	case FINALIZAR_PROGRAMA:
		log_info(cpuhlog, "Finalizo el programa %d.", in_cpu_msg->pcb.id);
		moverAColaExit(&in_cpu_msg->pcb);

		char* mensajeFinalizar = "El programa finalizo correctamente.";

		// Creo y serializo string kernel a consola.
		out_con_msg = newStrKerCon(KERNEL_ID, CERRARCONSOLA, 0,
				strlen(mensajeFinalizar), (Byte*) mensajeFinalizar);
		sb = serializeKerCon(out_con_msg);

		// Extraigo el socket de la respectiva consola.
		pcb_aux = &(in_cpu_msg->pcb);
		consola_aux = pcb_aux->consola;

		// Envio a la conchola
		if (!socketSend(consola_aux, sb)) {
			log_error(cpuhlog, "No se pudo cerrar la consola.");
		} else {
			log_info(cpuhlog, "Se envio CERRARCONSOLA.");
		}

		// TODO CONFIMACION DE CERRAR CONSOLA

//		sb = socketReceive(consola_aux);
//		StrConKer* in_con_msg = unserializeConKer((Stream)sb);
//
//		if(in_con_msg->action == TODO_PIOLA){
//			log_info(cpuhlog, "Consola Cerrada");
//		}


		StrUmcKer* streamALaUmc;
		streamALaUmc = newStrKerUmc(KERNEL_ID, FINALIZAR_PROGRAMA,
		NULL, 0, pcb_aux->id, 0, 0, 0, 0);
		sb = serializeUmcKer(streamALaUmc);
		if (!socketSend(umcServer->ptrSocket, sb)) {
			log_error(cpuhlog, "No se pudo finalizar el programaa umc %d",
					pcb_aux->id);
		} else {
			log_info(cpuhlog, "Se finalizo el programa a umc %d", pcb_aux->id);
		}

		sb = socketReceive(umcServer->ptrSocket);
		in_umc_msg = unserializeUmcKer((Stream) sb);
		if(in_umc_msg->action==TODO_PIOLA){
			log_info(cpuhlog, "La UMC confirmo la Finalizacion del programa");
		}


		confirmarCpu(cpuClient);
		break;

	case ABORTAR_PROGRAMA:
		log_info(cpuhlog, "Aborto el programa %d.", in_cpu_msg->pcb.id);
		moverAColaExit(&in_cpu_msg->pcb);

		char* mensajeAbortar = "El programa fue abortado.";

		// Creo y serializo string kernel a consola.
		out_con_msg = newStrKerCon(KERNEL_ID, CERRARCONSOLA, 0,
				strlen(mensajeAbortar), mensajeAbortar);
		sb = serializeKerCon(out_con_msg);

		// Extraigo el socket de la respectiva consola.
		pcb_aux = &(in_cpu_msg->pcb);
		consola_aux = pcb_aux->consola;

		// Envio a la conchola
		if (!socketSend(consola_aux, sb)) {
			log_error(cpuhlog, "No se pudo cerrar la consola.");
		} else {
			log_info(cpuhlog, "Se envio CERRARCONSOLA.");
		}
		//Envio UMC
		//StrUmcKer* streamALaUmc;
		streamALaUmc = newStrKerUmc(KERNEL_ID, FINALIZAR_PROGRAMA,
		NULL, 0, pcb_aux->id, 0, 0, 0, 0);
		sb = serializeUmcKer(streamALaUmc);
		if (!socketSend(umcServer->ptrSocket, sb)) {
			log_error(cpuhlog, "No se pudo abortar el programaa umc %d",
					pcb_aux->id);
		} else {
			log_info(cpuhlog, "Se aborto el programa a umc %d", pcb_aux->id);
		}
		sb = socketReceive(umcServer->ptrSocket);
				 in_umc_msg = unserializeUmcKer((Stream) sb);
				if(in_umc_msg->action==TODO_PIOLA){
					log_info(cpuhlog, "La UMC confirmo la Finalizacion del programa");
				}

		confirmarCpu(cpuClient);
		break;

	case TERMINE_EL_QUANTUM:

		log_info(cpuhlog, "Finalizo el quantum del programa %d.",
				in_cpu_msg->pcb.id);

		if (buscarPCB(listaExec, &in_cpu_msg->pcb)) {
			moverAColaReady(&in_cpu_msg->pcb);

		} else {

			pcb_aux = &in_cpu_msg->pcb;

			log_info(cpuhlog, "Este programa esta abortado (%d).",
					in_cpu_msg->pcb.id);
			streamALaUmc = newStrKerUmc(KERNEL_ID, FINALIZAR_PROGRAMA,
			NULL, 0, pcb_aux->id, 0, 0, 0, 0);
			sb = serializeUmcKer(streamALaUmc);
			if (!socketSend(umcServer->ptrSocket, sb)) {
				log_error(cpuhlog, "No se pudo abortar el programaa umc %d",
						pcb_aux->id);
			} else {
				log_info(cpuhlog, "Se aborto el programa a umc %d",
						pcb_aux->id);
			}
			sb = socketReceive(umcServer->ptrSocket);
				in_umc_msg = unserializeUmcKer((Stream) sb);
				if(in_umc_msg->action==TODO_PIOLA){
					log_info(cpuhlog, "La UMC confirmo la Finalizacion del programa");
				}

		}
		confirmarCpu(cpuClient);
		break;

//	case STACK_SIZE:
//
//		if((sb = socketReceive(cpuClient)) == NULL) {
//			log_error(cpuhlog,"No se pudo recibir el stack size");
//		}
//		stringToCpu = unserializeCpuKer(sb);
//
//		stringToCpu = newStrKerCpu(CPU_ID, STACK_SIZE, *pcbVacio, stackSize, 0, NULL, 0, NULL, 0);
//		sb = serializeKerCpu(stringToCpu);
//		if (!socketSend(cpuClient, sb)) {
//			log_error(cpuhlog, "no se pudo enviar: stack size al cpu");
//		}
//
//		break;
	default:
		log_error(cpuhlog,
				"KERNEL : CPU %d ha enviado un action incomprensible",
				cpuClient->descriptor);
		break;

	}
	// OJO! EL FREE LO HAGO ACA ABAJO, NO EN CADA CASE
	//free(sb);
}

void consoleClientHandler(Socket *consoleClient, Stream data) {
	pcb *pcbLoco = malloc(sizeof(pcb*));
	SocketBuffer* sb;
	StrKerUmc* sku;
	StrConKer *sck = unserializeConKer(data);
	switch (sck->action) {
	case STD_OUTPUT:

		log_error(cpuhlog, "KERNEL : El Cliente %d mando STD_OUTPUT",
				consoleClient->descriptor);
		//SI CONSOLA ME MANDA OUT, NO PASA NADA, PORQUE EL TCB NO CAMBIA
		//Y YA ESTA DESBLOQUEADO. ACA NO DEBERIA DE HACER NADA EL KERNEL
		break;

	case ARCHIVO_ANSISOP:

		log_info(cpuhlog, "KERNEL : El Cliente %d mando ANSISOP",
				consoleClient->descriptor);

		//SE GENERA EL NUEVO PCB
		pcbLoco = crearNuevoPcb(consoleClient, (char*) sck->fileContent,
				sck->fileContentLen);

		SocketBuffer* buffer;
		StrUmcKer* streamALaUmc;

		streamALaUmc = newStrKerUmc(KERNEL_ID, INICIALIZAR_PROGRAMA,
				sck->fileContent, sck->fileContentLen, pcbLoco->id,
				pcbLoco->paginasDeCodigo, 0, 0,
				cantidadPaginasArchivo(sck->fileContentLen));
		buffer = serializeKerUmc(streamALaUmc);
		if (!socketSend(umcServer->ptrSocket, buffer)) {
			log_error(cpuhlog, "No se pudo inicializar programa %d",
					pcbLoco->id);
		} else {
			log_info(cpuhlog, "Se inicializo el programa %d", pcbLoco->id);
		}

		//CARGO LA CONSOLA INGRESADA JUNTO CON EL PCB QUE
		//TRAJO A LA LISTA DE CONSOLAS QUE VOY A USAR PARA GESTIONAR
		//LOS SERVICIOS EXPUESTOS A LA CPU
		Console *clientConsole = malloc(sizeof(Console));

		clientConsole->consoleClient = consoleClient;
		clientConsole->pcb = pcbLoco;
		clientConsole->data = sck->fileContent;
		clientConsole->dataLength = sck->fileContentLen;

//			mtxLock(&mtxConsoleList);
//		pthread_mutex_lock(mutexListaExec);
//		list_add(consoleList, clientConsole);
//		pthread_mutex_unlock(mutexListaExec);
//			mtxUnlock(&mtxConsoleList);
//		log_info(cpuhlog, "KERNEL : Consola %d añadida a la lista",
//				consoleClient->descriptor);

		// CONFIRMACION DE UMC

		buffer = socketReceive(umcServer->ptrSocket);
		StrUmcKer* in_umc_msg = unserializeUmcKer((Stream) buffer);

		if(in_umc_msg->action == PROGRAMA_RECIBIDO){
			log_info(cpuhlog, "Programa recbido por la UMC.");
			moverAColaReady(pcbLoco);
		}
		if(in_umc_msg->action == PROGRAMA_NO_INICIALIZADO){
			log_error(cpuhlog, "La UMC mando abortar el nuevo programa. :(");
			char* mensajeAbortar = "\n> La SWAP no pudo inicializar programa. <\n";
			StrKerCon* out_con_msg;
			out_con_msg = newStrKerCon(KERNEL_ID, CERRARCONSOLA, 0,
					strlen(mensajeAbortar), mensajeAbortar);
			sb = serializeKerCon(out_con_msg);

			// Envio a la conchola
			if (!socketSend(consoleClient, sb)) {
				log_error(cpuhlog, "No se pudo cerrar la consola.");
			} else {
				log_info(cpuhlog, "Se envio CERRARCONSOLA.");
			}

		}

		break;

	case ABORTAR_PROGRAMA:
		printf("Se pidio ABORTAR programa para consola.\n");
		pthread_mutex_lock(mutexListaExec);
		pcbLoco = buscarPCBPorConsola(listaExec, consoleClient);
		pthread_mutex_unlock(mutexListaExec);

		if (pcbLoco != NULL) {
			moverAColaExit(pcbLoco);

//		sku = newStrKerUmc(KERNEL_ID, FINALIZAR_PROGRAMA,
//		NULL, 0, pcbLoco->id, 0, 0, 0, 0);
//		sb = serializeUmcKer(sku);
//		if (!socketSend(umcServer->ptrSocket, sb)) {
//			log_error(cpuhlog, "No se pudo abortar el programaa umc %d",
//					pcbLoco->id);
//		} else {
//			log_info(cpuhlog, "Se aborto el programa a umc %d", pcbLoco->id);
//		}
		}
		puts("PROGRAMA ABORTADO.");
		break;
	default:
		log_error(cpuhlog,
				"KERNEL : No se pudo determinar el action del cliente CONSOLA");
		log_error(cpuhlog, "KERNEL : Action = %d", sck->action);
		break;
	}
}

int pedirTamanioDePagina(int puerto) {

	umcServer = socketCreateClient();
	do {
		puts("**********************************");
		puts("Intentando conectar con el hilo de la umc.");
		printf("IP: %s, PUERTO: %d\n", ipUMC, puerto);
		sleep(3);
	} while (!socketConnect(umcServer, ipUMC, puerto));

	SocketBuffer * buffer;
	StrKerUmc * streamKerUmc;
	StrUmcKer * streamUmcKer;
	//(Char id, Char action, Byte* data, Int32U size, Int32U pid, Int32U cantPage, Int32U pagina, Int32U offset, Int32U tamanio)
	streamKerUmc = newStrKerUmc(KERNEL_ID, TAMANIO_DE_MARCOS, (Byte*) "hola", 0,
			0, 0, 0, 0, 0);
	buffer = serializeKerUmc(streamKerUmc);
	socketSend(umcServer->ptrSocket, buffer);

	buffer = socketReceive(umcServer->ptrSocket);

	if (buffer == NULL)
		puts("Error al recibir del cliente");

	streamUmcKer = unserializeUmcKer((Stream) buffer);

	return (streamUmcKer->size);
}

int cantidadPaginasArchivo(int longitudArchivo) {

	int aux = (longitudArchivo / tamanioPaginas) + stackSize;

	if (longitudArchivo % tamanioPaginas == 0) {
		return aux;
	} else {

		return aux + 1;
	}
	//int a=cantidadPaginas(longitudArchivo, tamanioPaginas);
	return 0;
}

int calcularCantPagCod(int longitudArchivo){
	int aux = (longitudArchivo / tamanioPaginas);

	if (longitudArchivo % tamanioPaginas == 0) {
		return aux;
	} else {

		return aux + 1;
	}
	//int a=cantidadPaginas(longitudArchivo, tamanioPaginas);
	return 0;

}


String intToStr(Int32U integer) {
	String result = malloc(sizeof(Byte) * 10);
	sprintf(result, "%d", integer);
	return result;
}

bool enviarPcbACpu(Socket * cpuLoca) {

	log_info(cpuhlog, "Pido patito Cola Ready");
	pthread_mutex_lock(mutexColaReady);
	if (listaReady->elements_count != 0) {
//		printf("La CPU %d esta al re pedo.\n", cpuLoca->descriptor);

		pcb* pcbAEnviar = (pcb*) list_get(listaReady, 0);
		buscarYEliminarPCBEnLista(listaReady, pcbAEnviar);
		pthread_mutex_unlock(mutexColaReady);

		pcbAEnviar->estado = EXEC; //2 EXEC
//		pcbAEnviar->cpu = cpuLoca; Activar esto para que mate procesos en exec cuando cae la cpu, ver primero el tema de la umc.

		pthread_mutex_lock(mutexListaExec);
		list_add(listaExec, pcbAEnviar);
		pthread_mutex_unlock(mutexListaExec);

//	Esto lo hago a manopla arroba, para no liberar el patito.
//	moverAListaExec(pcbAEnviar);

		pthread_mutex_lock(mutexQuantum);


	char*	auxStackSize = intToStr(stackSize);


		StrKerCpu* skc = newStrKerCpu(KERNEL_ID, ENVIO_PCB, *pcbAEnviar,
				quantum, quantumSleep,
				NULL, 0, auxStackSize /*NOMBRE DISPOSITIVO*/,
				strlen(auxStackSize) /*LEN NOMBRE DISPOSITIVO*/);
		pthread_mutex_unlock(mutexQuantum);

		SocketBuffer* sb = serializeKerCpu(skc);
		if (!socketSend(cpuLoca, sb)) {
			printf("No se pudo enviar el stream al cpu");
		} else {
			printf("Se envio el pcb del programa %d al cpu", pcbAEnviar->id);
		}
		free(auxStackSize);
		free(cpuLoca);
		free(sb);
		return TRUE;
	} else {
		pthread_mutex_unlock(mutexColaReady);
		return FALSE;
	}

//	Socket * punteroACopia = malloc(sizeof(Socket));
//
//	memcpy(punteroACopia, cpuClient, sizeof(cpuClient));
//
//	pthread_t hiloCpuAlPedo;
//	pthread_attr_t attrHiloCpuAlPedo;
//	pthread_attr_init(&attrHiloCpuAlPedo);
//	pthread_attr_setdetachstate(&attrHiloCpuAlPedo, PTHREAD_CREATE_DETACHED);
//	pthread_create(&hiloCpuAlPedo, &attrHiloCpuAlPedo, (void*) &funcionHiloCpuAlPedo, punteroACopia);
//	pthread_attr_destroy(&attrHiloCpuAlPedo);

//	 Aca iria un else en caso de que no haya en la cola de ready,
//	 Tendría que mandar un mensaje para que el cpu siga pidiendo.
}

void funcionHiloCpuAlPedo(Socket * cpuLoca) {

	bool seguirBuscando = TRUE;

	while (seguirBuscando) {
		pthread_mutex_lock(mutexColaReady);

		if (listaReady->elements_count == 0) {
			log_info(cpuhlog, "La CPU %d esta al re pedo.", cpuLoca->descriptor);
			pthread_mutex_unlock(mutexColaReady);
			sleep(1);
		} else {
			seguirBuscando = FALSE;
		}
	}

	pcb* pcbAEnviar = (pcb*) list_get(listaReady, 0);
	buscarYEliminarPCBEnLista(listaReady, pcbAEnviar);
	pthread_mutex_unlock(mutexColaReady);

	pcbAEnviar->estado = EXEC; //2 EXEC

	pthread_mutex_lock(mutexListaExec);
	list_add(listaExec, pcbAEnviar);
	pthread_mutex_unlock(mutexListaExec);

//	Esto lo hago a manopla arroba, para no liberar el patito.
//	moverAListaExec(pcbAEnviar);

	pthread_mutex_lock(mutexQuantum);

//	(Char id, Char action, pcb pcb, Int32U quantum,
//			Int32U quantumSleep, Byte* data, Int32U dataLen,
//			Byte* nombreDispositivo, Int32U lenNomDispositivo)

	StrKerCpu* skc = newStrKerCpu(KERNEL_ID, ENVIO_PCB, *pcbAEnviar, quantum, quantumSleep,NULL,0,NULL,0);
	pthread_mutex_unlock(mutexQuantum);

	SocketBuffer* sb = serializeKerCpu(skc);
	if (!socketSend(cpuLoca, sb)) {
		printf("No se pudo enviar el stream al cpu");
	} else {
		printf("Se envio el pcb del programa %d al cpu", pcbAEnviar->id);
	}
	free(cpuLoca);
	free(sb);

}

void clientDown(int descriptor) {
	StrKerCon* out_con_msg;
	SocketBuffer* sb;
	StrKerUmc* out_umc_msg;
	Socket* consola_aux;
	StrUmcKer * in_umc_msg;

	pcb* pcbHuerfano = buscarPcbPorCpu(listaExec, descriptor);

	if (pcbHuerfano != NULL) {
		log_error(cpuhlog, "El cliente tenía al PCB %d", pcbHuerfano->id);
		log_info(cpuhlog, "Pido patito Cola Ready");

		log_info(cpuhlog, "Aborto el programa %d.", pcbHuerfano->id);
		moverAColaExit(pcbHuerfano);

		char* mensajeAbortar = "El programa fue abortado por cpu.";

		// Creo y serializo string kernel a consola.

		out_con_msg = newStrKerCon(KERNEL_ID, CERRARCONSOLA, 0,
				strlen(mensajeAbortar), mensajeAbortar);
		sb = serializeKerCon(out_con_msg);

		// Extraigo el socket de la respectiva consola.
		consola_aux = pcbHuerfano->consola;

		// Envio a la conchola
		if (!socketSend(consola_aux, sb)) {
			log_error(cpuhlog, "No se pudo cerrar la consola.");
		} else {
			log_info(cpuhlog, "Se envio CERRARCONSOLA.");
		}
		//Envio UMC
		//StrUmcKer* streamALaUmc;
		out_umc_msg = newStrKerUmc(KERNEL_ID, FINALIZAR_PROGRAMA, NULL, 0, pcbHuerfano->id, 0, 0, 0, 0);
		sb = (SocketBuffer*)serializeUmcKer(out_umc_msg);
		if (!socketSend(umcServer->ptrSocket, sb)) {
			log_error(cpuhlog, "No se pudo abortar el programaa umc %d", pcbHuerfano->id);
		} else {
			log_info(cpuhlog, "Se aborto el programa a umc %d", pcbHuerfano->id);
		}
		sb = socketReceive(umcServer->ptrSocket);
			in_umc_msg = unserializeUmcKer((Stream) sb);
			if(in_umc_msg->action==TODO_PIOLA){
				log_info(cpuhlog, "La UMC confirmo la Finalizacion del programa");
			}

	} else {

		pthread_mutex_lock(mutexListaCpu);
		if (eliminarCpuPorDescriptorYDevuelveUnBool(listaCpu, descriptor)) {
			log_error(cpuhlog, "Se elimino cpu caida, lista de CPUs = %d",
					listaCpu->elements_count);
		}
		pthread_mutex_unlock(mutexListaCpu);
	}

	pcbHuerfano = buscarPcbPorConsola(listaExec, descriptor);
	if(pcbHuerfano == NULL) pcbHuerfano = buscarPcbPorConsola(listaReady, descriptor);
	if(pcbHuerfano == NULL) pcbHuerfano = buscarPcbPorConsola(listaBlock, descriptor);

	if(pcbHuerfano != NULL){
		log_error(cpuhlog, "El cliente era una CONSOLA");
		log_error(cpuhlog, "El cliente tenía al PCB %d", pcbHuerfano->id);

		pthread_mutex_lock(mutexListaExec);
		buscarYEliminarPCBEnLista(listaExec, pcbHuerfano);
		pthread_mutex_unlock(mutexListaExec);

		pthread_mutex_lock(mutexColaReady);
		buscarYEliminarPCBEnLista(listaReady, pcbHuerfano);
		pthread_mutex_unlock(mutexColaReady);

		pthread_mutex_lock(mutexListaBlock);
		buscarYEliminarPCBEnLista(listaBlock, pcbHuerfano);
		pthread_mutex_unlock(mutexListaBlock);

		list_add(listaExit, pcbHuerfano);

		log_error(cpuhlog, "Se agrego el PCB %d a la lista de EXIT.", pcbHuerfano->id);
	}
}

bool confirmarCpu(Socket* cpu) {

	SocketBuffer* sb;
	StrKerCpu* skc;

	skc = newStrKerCpu(KERNEL_ID, TODO_PIOLA, *pcbVacio, 0, 0, NULL, 0,
	NULL /*NOMBRE DISPOSITIVO*/, 0 /*LEN NOMBRE DISPOSITIVO*/);
	;
	sb = serializeKerCpu(skc);

	if (!socketSend(cpu, sb)) {
		printf("No se pudo enviar confirmación a CPU. \n");
		return FALSE;
	}
	log_info(cpuhlog, "Se envió confirmación a CPU.");
	return TRUE;
}
