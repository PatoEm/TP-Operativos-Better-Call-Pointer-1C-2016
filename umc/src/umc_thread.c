

#include "umc.h"
#include "umc_thread.h"

pthread_t umcst;
void** nada;

fd_set master;
fd_set read_fds;
t_log* umcslog;

int fdmax;
int mi_socket;

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
	mi_socket = thread_socket;
	Socket* serverSocket = socketCreateServer(thread_socket);
	if (serverSocket == NULL) {
		log_error(umclog, "No se puede escuchar el server");
		//return FALSE;
	}
	log_info(umclog, "HILO %d: Hilo creado.\n", mi_socket);
	if (!socketListen(serverSocket)) {
		log_error(umclog, "No se pudo poner a escuchar al server.");
	}

	log_info(umclog, "HILO %d: Listening...\n", mi_socket);
	thread_socket ++;

//	*******************************************************************


//	ACEPTO LA CONEXION
	Socket* cpuClient = socketAcceptClient(serverSocket);

	log_info(umclog, "HILO %d: Cliente aceptado (%d).\n", mi_socket, cpuClient->descriptor);

	SocketBuffer* sb;
	StrCpuUmc* in_cpu_msg;
	StrKerUmc* in_ker_msg;

//	EL PUTO CASE

	while(TRUE){

		sb = socketReceive(cpuClient);

		log_info(umclog, "HILO %d: Buffer recibido de (%d).\n", mi_socket, cpuClient->descriptor);

		if(sb == NULL){
			log_error(umclog, "HILO %d: Se cayo el cliente (%d).\n", mi_socket, cpuClient->descriptor);
			break;
		}

		//in_cpu_msg = unserializeCpuUmc(sb);

		Stream strRecibido = (Stream) sb->data;
		Char id = getStreamId(strRecibido);

		log_info(umclog, "HILO %d: Buffer ID %d.\n", mi_socket, id);

		//log_info(umcslog, "ID Nuevo Cliente: %d.",id);
		switch (id) {

			case CPU_ID:
				in_cpu_msg = unserializeCpuUmc(sb);
				log_info(umclog, "HILO %d: Cliente (%d) es un CPU.\n", mi_socket, cpuClient->descriptor);
				//log_info(umcslog, "Cliente CPU");
				manageCpuRequest(cpuClient,in_cpu_msg);
				break;

			case KERNEL_ID:
				in_ker_msg = unserializeKerUmc(sb);
				log_info(umclog, "HILO %d: Cliente (%d) es el KERNEL.\n", mi_socket, cpuClient->descriptor);
				//log_info(umcslog, "Cliente KERNEL");
				manageKernelRequest(cpuClient,in_ker_msg);
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
	while ((streamCpuUmc->action)!=24/*CIERRE_CONEXION_CPU*/) {
		switch (streamCpuUmc->action) {
		case 36 /*TAMANIO_DE_MARCOS*/:
			log_info(umclog, "HILO %d: Me pidieron el tamanio de marcos (%d).\n", mi_socket, socket->descriptor);
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
			log_error(umclog, "HILO %d: No se pudo identificar la accion del CPU (%d).\n", mi_socket, socket->descriptor);
			break;
		}
		buffer = socketReceive(socket);
		if (buffer == NULL) {
			log_error(umclog, "HILO %d: Se cayo el cliente (%d).\n", mi_socket, socket->descriptor);
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
		log_error(umclog, "No se pudo identificar la accion del Kernel");
		break;
	}

//StrUmcKer* suk ;/*= newStrUmcKer();*/
//Boolean result = sendResponse(KERNEL_ID, suk, socket);

//return result;
}


