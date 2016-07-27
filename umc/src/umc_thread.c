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

	pthread_t hiloLoco;
	pthread_attr_t attrHiloLoco;
	pthread_attr_init(&attrHiloLoco);
	pthread_attr_setdetachstate(&attrHiloLoco, PTHREAD_CREATE_DETACHED);
	pthread_create(&hiloLoco, &attrHiloLoco, &umcThread,
	NULL);
	pthread_attr_destroy(&attrHiloLoco);

}

void umcThread() {
//	*******************************************************************
//	CREO EL NEUVO SOCKET
	mi_socket = thread_socket;
	Socket* serverSocket = socketCreateServer(thread_socket);
	if (serverSocket == NULL) {
		log_error(umclog, "No se puede escuchar el server");
		//return FALSE;
	}
	log_info(umclog, "HILO %d: Hilo creado (%d).", mi_socket, mi_socket);
	if (!socketListen(serverSocket)) {
		log_error(umclog, "No se pudo poner a escuchar al server.");
	}

	log_info(umclog, "HILO %d: Listening...", mi_socket);
	thread_socket++;

//	*******************************************************************

//	ACEPTO LA CONEXION
	Socket* cpuClient = socketAcceptClient(serverSocket);

	log_info(umclog, "HILO %d: Cliente numero %d aceptado.", mi_socket,
			cpuClient->descriptor);

	SocketBuffer* sb;
	StrCpuUmc* in_cpu_msg;
	StrKerUmc* in_ker_msg;

//	EL PUTO CASE

	while (TRUE) {

		sb = socketReceive(cpuClient);

		log_info(umclog, "HILO %d: Buffer recibido del cliente %d.", mi_socket,
				cpuClient->descriptor);

		if (sb == NULL) {
			log_error(umclog, "HILO %d: Se cayo el cliente %d.\n", mi_socket,
					cpuClient->descriptor);
			break;
		}

		Stream strRecibido = (Stream) sb->data;
		Char id = getStreamId(strRecibido);

		switch (id) {

		case CPU_ID:
			in_cpu_msg = unserializeCpuUmc((Stream) sb);
			log_info(umclog, "HILO %d: El cliente %d es un CPU (%d).",
					mi_socket, cpuClient->descriptor, id);
			manageCpuRequest(cpuClient, in_cpu_msg);
			break;

		case KERNEL_ID:
			in_ker_msg = unserializeKerUmc((Stream) sb);
			log_info(umclog, "HILO %d: El cliente %d es el KERNEL (%d).",
					mi_socket, cpuClient->descriptor, id);
			manageKernelRequest(cpuClient, in_ker_msg);
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
	while ((streamCpuUmc->action) != 24/*CIERRE_CONEXION_CPU*/) {
		scu = streamCpuUmc;
		switch (streamCpuUmc->action) {
		case 36 /*TAMANIO_DE_MARCOS*/:
			log_info(umclog,
					"HILO %d: La CPU [%d] pide TAMANIO_DE_MARCOS, tamanio es %d.",
					mi_socket, socket->descriptor, marco_Size);
			//(Char id, Char action, espacioAsignado pageComienzo, Int32U offset, Int32U dataLen, Byte* data, Int32U pid)
			streamUmcCpu = newStrUmcCpu(UMC_ID, TAMANIO_DE_MARCOS, unaPagina, 0,
					marco_Size, (Byte*) "hola", 0);
			buffer = serializeUmcCpu(streamUmcCpu);
			socketSend(socket, buffer);
			break;
		case 23/*CAMBIO_PROCESO_ACTIVO*/:
			log_info(umclog,
					"HILO %d: La CPU [%d] pide CAMBIO_PROCESO_ACTIVO, nuevo proceso es %d.",
					mi_socket, socket->descriptor, streamCpuUmc->pid);
			pidActivo = streamCpuUmc->pid;
			streamUmcCpu = newStrUmcCpu(UMC_ID, TODO_PIOLA, unaPagina, 0,
					0, NULL, 0);
			buffer = serializeUmcCpu(streamUmcCpu);
			socketSend(socket, buffer);
			break;
		case 25/*SOLICITAR_BYTES*/:
			log_info(umclog, "HILO %d: La CPU [%d] pide SOLICITAR_BYTES.",
					mi_socket, socket->descriptor);
			pthread_mutex_lock(mutexPedidos);
			paginaEncontrada = TRUE;
			if (paginasOcupadasPorPid(pidActivo) == 0
					&& cantidadDePaginasLibres() == 0) {
				log_error(umclog,
						"HILO %d: Se manda ABORTAR_PROGRAMA a la CPU %d.",
						mi_socket, socket->descriptor);
				streamUmcCpu = newStrUmcCpu(UMC_ID, 35 /*ABORTAR_PROGRAMA*/,
						unaPagina, scu->offset, 0, NULL, pidActivo);
				buffer = serializeUmcCpu(streamUmcCpu);
				socketSend(socket, buffer);
			} else {
				if (tlbHabilitada()) {
					bytes = leerEnTLB(pidActivo, scu->pageComienzo.numDePag,
							scu->offset, scu->pageComienzo.bitUso);
				} else {
					bytes = solicitarBytes(pidActivo,
							scu->pageComienzo.numDePag, scu->offset,
							scu->pageComienzo.bitUso);
				}
				if (!paginaEncontrada)
					streamUmcCpu = newStrUmcCpu(UMC_ID, ABORTAR_PROGRAMA,
							unaPagina, 0, 0, NULL, 0);
				else
					streamUmcCpu = newStrUmcCpu(UMC_ID, 25/*SOLICITAR_BYTES*/,
							unaPagina, scu->offset, scu->pageComienzo.bitUso,
							(Byte*) bytes, pidActivo);
				buffer = serializeUmcCpu(streamUmcCpu);
				socketSend(socket, buffer);
				log_info(umclog, "HILO %d: Bytes enviados al CPU [%d].",
						mi_socket, socket->descriptor);
			}
			pthread_mutex_unlock(mutexPedidos);
			break;
		case ALMACENAR_BYTES:
			log_info(umclog, "HILO %d: La CPU [%d] pide ALMACENAR_BYTES.",
					mi_socket, socket->descriptor);
			pthread_mutex_lock(mutexPedidos);
			paginaEncontrada = TRUE;
			if (paginasOcupadasPorPid(pidActivo) == 0
					|| cantidadDePaginasLibres() == 0) {
				log_error(umclog,
						"HILO %d: Se manda ABORTAR_PROGRAMA a la CPU %d.",
						mi_socket, socket->descriptor);
				streamUmcCpu = newStrUmcCpu(UMC_ID, 35 /*ABORTAR_PROGRAMA*/,
						unaPagina, scu->offset, 0, NULL, pidActivo);
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
				if (!paginaEncontrada)
					streamUmcCpu = newStrUmcCpu(UMC_ID, ABORTAR_PROGRAMA,
							unaPagina, 0, 0, NULL, 0);
				else
					streamUmcCpu = newStrUmcCpu(UMC_ID, TODO_PIOLA, unaPagina, 0,
										0, NULL, 0);
				buffer = serializeUmcCpu(streamUmcCpu);
				socketSend(socket, buffer);
			}

			pthread_mutex_unlock(mutexPedidos);
			break;
		default:
			log_error(umclog,
					"HILO %d: No se pudo identificar la accion del CPU (%d).\n",
					mi_socket, socket->descriptor);
			break;
		}
		buffer = socketReceive(socket);
		if (buffer == NULL) {
			log_error(umclog, "HILO %d: Se cayo el CPU (%d).\n", mi_socket,
					socket->descriptor);
			break;
		}
		streamCpuUmc = unserializeCpuUmc((Stream) buffer);

	}

}

void manageKernelRequest(Socket* socket, StrKerUmc* sku) {
	StrUmcKer*streamAlKerner;
	StrSwaUmc*streamSwapAUmc;
	SocketBuffer*buffer;
	switch (sku->action) {
	case 36 /*TAMANIO_DE_MARCOS*/:
		log_info(umclog,
				"HILO %d: KERNEL [%d] pide TAMANIO_DE_MARCOS, tamanio es %d.",
				mi_socket, socket->descriptor, marco_Size);
		//(Char id, Char action, Byte* data, Int32U size, Int32U pid, Int32U cantPage)
		streamAlKerner = newStrUmcKer(UMC_ID, 36/*TAMANIO_DE_MARCOS*/, "hola",
				marco_Size, 0, 0);
		buffer = serializeUmcKer(streamAlKerner);
		socketSend(socket, buffer);
		break;
	case 20 /*INICIALIZAR_PROGRAMA*/:
		log_info(umclog,
				"HILO %d: KERNEL [%d] pide INICIALIZAR_PROGRAMA, programa es %d.",
				mi_socket, socket->descriptor, sku->pid);
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

	case 22 /*FINALIZAR_PROGRAMA*/:
		log_info(umclog,
				"HILO %d: KERNEL [%d] pide FINALIZAR_PROGRAMA, programa es %d.",
				mi_socket, socket->descriptor, sku->pid);
		pthread_mutex_lock(mutexPedidos);
		finalizarPrograma(sku->pid);
		buffer = socketReceive(socketSwap->ptrSocket);
		streamSwapAUmc = unserializeSwaUmc(buffer);
		streamAlKerner = newStrUmcKer(UMC_ID, TODO_PIOLA, NULL, 0, sku->pid, 0);
		pthread_mutex_unlock(mutexPedidos);
		break;
	default:
		log_error(umclog, "No se pudo identificar la accion del Kernel");
		break;
	}

}

