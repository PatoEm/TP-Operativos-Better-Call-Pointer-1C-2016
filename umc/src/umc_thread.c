

#include "umc.h"
#include "umc_thread.h"

pthread_t umcst;
void** nada;

fd_set master;
fd_set read_fds;
t_log* umcslog;

int fdmax;

void newUmcThread() {
	// EL HILO VA A HACER SUS COSAS SIEMPRE QUE EL PROCESO PADRE
	// SIGA VIVO

	//pthread_create(&umcst, NULL, umcThread, NULL);

			pthread_t hiloLoco;
			pthread_attr_t attrHiloLoco;
			pthread_attr_init(&attrHiloLoco);
			pthread_attr_setdetachstate(&attrHiloLoco, PTHREAD_CREATE_DETACHED);
			pthread_create(&hiloLoco, &attrHiloLoco, &umcThread,
					NULL);
			pthread_attr_destroy(&attrHiloLoco);


}

void umcThread(){
//	*******************************************************************
//	CREO EL NEUVO SOCKET

	Socket* serverSocket = socketCreateServer(thread_socket);
	if (serverSocket == NULL) {
		puts("No se ppuede escuchar el server");
//		log_error(umcslog, "No se pudo crear el server escucha.");
		//return FALSE;
	}
	if (!socketListen(serverSocket)) {
		puts("No se pone a escuchar");
//		log_error(umcslog, "No se pudo poner a escuchar al server.");
	}

	thread_socket ++;

	//log_info(umcslog, "Server creado con exito y escuchando.");
	//return TRUE;
//	*******************************************************************


//	ACEPTO LA CONEXION
	Socket* cpuClient = socketAcceptClient(serverSocket);

	SocketBuffer* sb;
	StrCpuUmc* in_cpu_msg;

//	EL PUTO CASE

	while(TRUE){

		sb = socketReceive(cpuClient);

		if(sb == NULL) puts("No se pudo recibir del CPU.");

		in_cpu_msg = unserializeCpuUmc(sb);

		Char id = getStreamId(in_cpu_msg);

		log_info(umcslog, "ID Nuevo Cliente: %d.",id);
		switch (id) {

			case CPU_ID:
				log_info(umcslog, "Cliente CPU");
				manageCpuRequest(cpuClient,in_cpu_msg);
				break;

			case KERNEL_ID:
				log_info(umcslog, "Cliente KERNEL");
				manageKernelRequest(cpuClient,in_cpu_msg);
				break;
		}
	}

//	FIN
}


void manageCpuRequest(Socket* socket, StrCpuUmc* scu) {
	int pidActivo;
	espacioAsignado unaPagina;
	SocketBuffer*buffer;
	StrCpuUmc*streamCpuUmc = scu;
	StrUmcCpu*streamUmcCpu;
	char* bytes;
	while (!24/*CIERRE_CONEXION_CPU*/) {
		switch (streamCpuUmc->action) {
		case 36 /*TAMANIO_DE_MARCOS*/:
			//(Char id, Char action, espacioAsignado pageComienzo, Int32U offset, Int32U dataLen, Byte* data, Int32U pid)
			streamUmcCpu = newStrUmcCpu(UMC_ID, TAMANIO_DE_MARCOS, unaPagina, 0, marco_Size, "hola", 0);
			buffer = serializeUmcCpu(streamUmcCpu);
			socketSend(socket, buffer);
			break;
		case 23/*CAMBIO_PROCESO_ACTIVO*/:
			pidActivo = streamCpuUmc->pid;
			break;
		case 25/*SOLICITAR_BYTES*/:
			pthread_mutex_lock(mutexPedidos);
			if (paginasOcupadasPorPid(pidActivo) == 0
					|| cantidadDePaginasLibres() == 0) {
				streamUmcCpu = newStrUmcCpu(UMC_ID, 35 /*ABORTAR_PROGRAMA*/,
						unaPagina, scu->offset, scu->dataLen, bytes, pidActivo);
				buffer = serializeUmcCpu(streamUmcCpu);
				socketSend(socket, buffer);
			} else {
				if (tlbHabilitada()) {
					bytes = leerEnTLB(pidActivo, scu->pageComienzo.numDePag,
							scu->offset, scu->dataLen);
				} else
					bytes = solicitarBytes(pidActivo,
							scu->pageComienzo.numDePag, scu->offset,
							scu->dataLen);
				streamUmcCpu = newStrUmcCpu(UMC_ID, 25/*SOLICITAR_BYTES*/,
						unaPagina, scu->offset, scu->dataLen, bytes, pidActivo);
				buffer = serializeUmcCpu(streamUmcCpu);
				socketSend(socket, buffer);
			}
			pthread_mutex_unlock(mutexPedidos);
			break;
		case 34 /*ALMACENAR_BYTES*/:
			pthread_mutex_lock(mutexPedidos);
			if (paginasOcupadasPorPid(pidActivo) == 0
					|| cantidadDePaginasLibres() == 0) {
				streamUmcCpu = newStrUmcCpu(UMC_ID, 35 /*ABORTAR_PROGRAMA*/,
						unaPagina, scu->offset, scu->dataLen, bytes, pidActivo);
				buffer = serializeUmcCpu(streamUmcCpu);
				socketSend(socket, buffer);
			} else {

				if (tlbHabilitada()) {
					if (!escribirEnTLB(pidActivo, scu->pageComienzo.numDePag,
							scu->offset, scu->dataLen, scu->data)) {
						almacenarBytes(pidActivo, scu->pageComienzo.numDePag,
								scu->offset, scu->dataLen, scu->data);
					}
				} else
					almacenarBytes(pidActivo, scu->pageComienzo.numDePag,
							scu->offset, scu->dataLen, scu->data);
				streamUmcCpu = newStrUmcCpu(UMC_ID, 34 /*ALMACENAR_BYTES*/,
						unaPagina, scu->offset, scu->dataLen, bytes, pidActivo);
				buffer = serializeUmcCpu(streamUmcCpu);
				socketSend(socket, buffer);
			}
			pthread_mutex_unlock(mutexPedidos);
			break;
		default:
			printf("No se pudo identificar la accion de la CPU");
			break;
		}
		buffer = socketReceive(socket);
		if (buffer == NULL) {
			puts("Problemas al recibir del cpu");
			break;
		}
		streamCpuUmc = unserializeCpuUmc(buffer);
	}
//StrUmcCpu* suc;/*= newStrUmcCpu();*/
//Boolean result = sendResponse(CPU_ID, suc, socket);

//return result;
}

void manageKernelRequest(Socket* socket, StrKerUmc* sku) {
	StrUmcKer*streamAlKerner;
	SocketBuffer*buffer;
	switch (sku->action) {
	case 36 /*TAMANIO_DE_MARCOS*/:
		//(Char id, Char action, Byte* data, Int32U size, Int32U pid, Int32U cantPage)
		streamAlKerner = newStrUmcKer(UMC_ID, 36/*TAMANIO_DE_MARCOS*/,
		"hola", marco_Size, 0, 0);
		buffer = serializeUmcKer(streamAlKerner);
		socketSend(socket, buffer);
		break;
	case 20 /*INICIALIZAR_PROGRAMA*/:
		pthread_mutex_lock(mutexPedidos);

		if (0 == inicializarPrograma(sku->pid, sku->cantPage, sku->data)) {
			streamAlKerner = newStrUmcKer(UMC_ID,
					21/*PROGRAMA_NO_INICIALIZADO*/,
					NULL, 0, 0, 0);
			buffer = serializeUmcKer(streamAlKerner);
			socketSend(socket, buffer);
		} else {
			streamAlKerner = newStrUmcKer(UMC_ID,
			PROGRAMA_RECIBIDO,
			NULL, 0, 0, 0);
			buffer = serializeUmcKer(streamAlKerner);
			socketSend(socket, buffer);
		}
		pthread_mutex_unlock(mutexPedidos);
		break;

//todo hablar con pato el almacenar y leer bytes
	case 22 /*FINALIZAR_PROGRAMA*/:
		pthread_mutex_lock(mutexPedidos);
		finalizarPrograma(sku->pid);
		pthread_mutex_unlock(mutexPedidos);
		break;
	default:
		printf("No se pudo identificar la accion del Kernel");
		break;
	}

//StrUmcKer* suk ;/*= newStrUmcKer();*/
//Boolean result = sendResponse(KERNEL_ID, suk, socket);

//return result;
}


