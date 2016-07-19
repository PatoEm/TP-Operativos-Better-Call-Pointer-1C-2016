/*
 ============================================================================
 Name        : cpu.c
 Author      : utn.so
 Version     :
 Copyright   : Your copyright notice
 Description : Main CPU
 ============================================================================
 */
/*****************************************
 * Dependencias
 ****************************************/
#include "commons/config.h"
#include "libreriasCompartidas/pointerStream.h"
#include "libreriasCompartidas/pointerSocketes.h"
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
/*****************************************
 * Constantes
 ****************************************/
#define CONFIG_FILE "configcpu"
#define PARAM_LENGTH 4
#define PUERTO_NUCLEO "PUERTO_NUCLEO"
#define IP_NUCLEO "IP_NUCLEO"
#define PUERTO_UMC "PUERTO_UMC"
#define IP_UMC "IP_UMC"

/*****************************************
 * Configuracion
 ****************************************/
t_config* tConfig = NULL;
//Int32U puertoNucleo;
//String ipNucleo;
//String ipUmc;
//Int32U puertoUmc;
//int tamanioPag;

/*****************************************
 * Socket & Streams
 ****************************************/
//SocketClient* nucleoClient = NULL;
//SocketClient* umcClient = NULL;
StrCpuKer* sck = NULL;
StrCpuUmc* scu = NULL;
StrUmcCpu* suc = NULL;
StrKerCpu* skc = NULL;

/*****************************************
 * PCB actual
 ****************************************/
pcb* pcbVacio = NULL;
/*****************************************
 * Funciones del main
 ****************************************/
Boolean loadConfig();
Boolean socketConnection();
Boolean getNextPcb();
char* pedirInstruccion(pcb*);

/*****************************************
 * GLOBAL
 ****************************************/
SocketBuffer* buffer;
t_log* logger = NULL;
/*****************************************
 *
 *
 *  |\  /|    /\    | |\   |
 *  | \/ |   /  \   | | \  |
 *  |    |  /----\  | |  \ |
 * 	|    | /      \ | |   \|
 *
 *
 ****************************************/

int main() {
	finalizoCorrectamente = FALSE;
//
//	pcb * pcbVacioLoco=newEmptyPcb();
//	StrCpuKer *hola = newStrCpuKer(CPU_ID,PRIMER_PCB,*pcbVacioLoco,0,0,"hola","hola",0);
//	SocketBuffer *bufferLoco =serializeCpuKer(hola);
//	bufferLoco->data;
//	hola=unserializeCpuKer(bufferLoco->data);
//
//	paginaDeStack * paginita =list_get(hola->pcb.indiceDelStack,0);
//	paginaDeStack * paginita2 =list_get(hola->pcb.indiceDelStack,1);

	asignadoVacio = newEspacioAsignado();
	log_debug(getLogger(),
			"Cargo variables de configuracion, me conecto al NUCLEO y a la UMC");
	pcbVacio = malloc(sizeof(pcb));
	pcbVacio = newEmptyPcb();
	t_config* tConfig = NULL;

	abortoCPU = 0; //NO BORREN ESTO ASI SE QUEDA ATENTO A LA SEÑAL

	if (loadConfig() && socketConnection()) {

		while (TRUE) {
			log_debug(getLogger(),
					"Devuelvo el PCB procesado y obtengo uno nuevo del NUCLEO");
			getNextPcb();
			seguirEjecutando = TRUE;
			log_debug(getLogger(), "Proceso el PCB del NUCLEO");
			Int8U quantum = skc->quantum;
			while (quantum > 0 && seguirEjecutando && !finalizoCorrectamente) {
				analizadorLinea(pedirInstruccion(&pcbProceso), &funciones,
						&funcionesDeKernel);
				moverProgramCounterPcb(&pcbProceso);
				quantum--;
			}

			if (!seguirEjecutando) {

				sck = newStrCpuKer(CPU_ID, ABORTAR_PROGRAMA, pcbProceso, 0, 0,
						NULL, NULL, 0);
				buffer = serializeCpuKer(sck);
				if (!socketSend(socketNucleo->ptrSocket, buffer)) {
					log_info(getLogger(),
							"No se pudo enviar el buffer al NUCLEO.");
					return FALSE;

				}
			}

			if (quantum == 0) {
				sck = newStrCpuKer(CPU_ID, TERMINE_EL_QUANTUM, pcbProceso, 0, 0,
				NULL, NULL, 0);
				buffer = serializeCpuKer(sck);
				if (!socketSend(socketNucleo->ptrSocket, buffer)) {
					log_info(getLogger(),
							"No se pudo enviar el buffer al NUCLEO.");
					return FALSE;
				}
			}
			seguirEjecutando = TRUE;
		}

		config_destroy(tConfig);
		while (1) {
			puts("Esperando..");
			sleep(10);
		}
		return FALSE;
	}
	return TRUE;
}

/*****************************************
 * Implementacion de funciones
 ****************************************/

Boolean loadConfig() {
	String configFilePath = CONFIG_FILE;

	log_debug(getLogger(), "Genero tabla de configuracion");
	tConfig = config_create(configFilePath);
	if (tConfig == NULL) {
		log_error(getLogger(), "No se encuentra o falta el archivo de configuracion en la direccion '%s'.\n", configFilePath);
		return FALSE;
	}

	log_debug(getLogger(), "Verifico consistencia, debe haber 4 campos");
	if (config_keys_amount(tConfig) == PARAM_LENGTH) {

		log_debug(getLogger(), "Verifico que los parametros del NUCLEO tengan sus valores OK");
		if (config_has_property(tConfig, "PUERTO_NUCLEO")) {
			nucleoPort = config_get_int_value(tConfig, "PUERTO_NUCLEO");
			log_info(getLogger(), "Configuracion del PUERTO_NUCLEO leida con exito");
		} else {
			log_error(getLogger(), "Falta un parametro: PUERTO_NUCLEO. \n");
			return FALSE;
		}

		if (config_has_property(tConfig, "IP_NUCLEO")) {
			ipNucleo = config_get_string_value(tConfig, "IP_NUCLEO");
			log_info(getLogger(), "Configuracion del IP_NUCLEO leida con exito");
		} else {
			log_error(getLogger(), "Falta un parametro: IP_NUCLEO. \n");
			return FALSE;
		}
		log_debug(getLogger(),
				"Verifico que los parametros de la UMC tengan sus valores OK");
		if (config_has_property(tConfig, "PUERTO_UMC")) {
			umcPort = config_get_int_value(tConfig, "PUERTO_UMC");
			log_info(getLogger(), "Configuracion del PUERTO_UMC leida con exito");
		} else {
			log_error(getLogger(), "Falta un parametro: PUERTO_UMC. \n");
			return FALSE;
		}

		if (config_has_property(tConfig, "IP_UMC")) {
			ipUMC = config_get_string_value(tConfig, "IP_UMC");
			log_info(getLogger(), "Configuracion del IP_UMC leida con exito");
		} else {
			log_error(getLogger(), "Falta un parametro: IP_UMC. \n");
			return FALSE;
		}

		log_info(getLogger(),
				"**********************************************************************");
		log_info(getLogger(),
				"**********************************************************************");
		log_info(getLogger(),
				"**********************************************************************");
		log_info(getLogger(), "Archivo de configuracion CPU leido exitosamente\n=============\n");
		log_info(getLogger(), "Se inicializo la CPU.");

		printf("Archivo de configuracion de CPU leido\n============\n");
		printf("PUERTO_NUCLEO: %d\nIP_NUCLEO: %s\n", nucleoPort, ipNucleo);
		printf("Puerto_UMC: %d\nIP_UMC: %s\n", umcPort, ipUMC);

		return TRUE;
	} else {
		log_error(getLogger(), "El archivo de configuracion del CPU no tiene todos los campos.\n");
		return FALSE;
	}
}

Boolean socketConnection() {
	log_debug(getLogger(), "Conexion al NUCLEO");
	socketNucleo = socketCreateClient();

	do {
		puts("**********************************");
		puts("Intentando conectar con el NUCLEO.");
		printf("IP: %s, PUERTO: %d\n", ipNucleo, nucleoPort);
		sleep(3);
	} while (!socketConnect(socketNucleo, ipNucleo, nucleoPort));

	if (handshake(socketNucleo, CPU_ID)) {
		log_info(getLogger(), "Handshake realizado con exito.");
	} else {
		log_info(getLogger(), "No se pudo realizar el handshake.");
		return FALSE;
	}

	//SocketClient* umcServer;
	espacioAsignado aux;
	aux.IDPaginaInterno = 0;
	aux.bitDePresencia = 0;
	aux.bitModificado = 0;
	aux.bitUso = 0;
	aux.numDePag = 0;
	aux.pid = 0;
	aux.punteroAPagina = 0;
	socketUMC = socketCreateClient();
	do {
		puts("**********************************");
		puts("Intentando conectar con la UMC.");
		printf("IP: %s, PUERTO: %d\n", ipUMC, umcPort);
		sleep(3);
	} while (!socketConnect(socketUMC, ipUMC, umcPort));
	StrCpuUmc* out_umc_msg = newStrCpuUmc(CPU_ID, HANDSHAKE, aux, 0, 0, "", 0);
	SocketBuffer* sb = serializeCpuUmc(out_umc_msg);
	if (socketSend(socketUMC->ptrSocket, sb)) {
		log_info(getLogger(), "Mensaje enviado a la UMC.");
	} else {
		log_error(getLogger(), "No se pudo enviar el mensaje a la UMC.");
		return FALSE;
	}

	if ((sb = socketReceive(socketUMC->ptrSocket)) == NULL) {
		log_error(getLogger(), "No se pudo recibir el stream de la UMC");
		return FALSE;
	}

	StrUmcCpu* in_umc_msg = unserializeUmcCpu(sb);

	int puertoNuevoUmc = in_umc_msg->dataLen;

	printf("Nuevo hilo UMC  es %d.\n", in_umc_msg->dataLen);


	socketUMC = socketCreateClient();
	do {
		puts("**********************************");
		puts("Intentando conectar con el hilo de la UMC.");
		printf("IP: %s, PUERTO: %d\n", ipUMC, puertoNuevoUmc);
		sleep(3);
	} while (!socketConnect(socketUMC, ipUMC, puertoNuevoUmc));
	log_info(getLogger(), "Me conecte al agradable hilo");

	tamanioPaginaUmc = pedirTamanioDePagina();

	//	while(1){}

	return TRUE;
}

Boolean getNextPcb() {

	//pcbProceso = newEmptyPcb();

	//pcbProceso = malloc(sizeof(pcb));

	sck = newStrCpuKer(CPU_ID, RECIBIR_NUEVO_PROGRAMA, *pcbVacio, 0, 0, 0, NULL /*NOMBRE DISPOSITIVO*/, 0 /*LEN NOMBRE DISPOSITIVO*/);

	log_info(getLogger(), "getNextPcb: Nuevo PCB vacio creado.");

	log_debug(getLogger(), "Serializo y armo el socket");
	SocketBuffer* sb = serializeCpuKer(sck);
	log_debug(getLogger(), "Envio el socketBuffer");
	if (!socketSend(socketNucleo->ptrSocket, sb)) {
		log_error(getLogger(), "No se pudo enviar el stream al NUCLEO");
		return FALSE;
	}
	free(sb);
	log_info(getLogger(), "Obtener siguiente PCB");

	log_debug(getLogger(), "Recibo la respuesta del NUCLEO y deserealizo");
	if ((sb = socketReceive(socketNucleo->ptrSocket)) == NULL) {
		log_error(getLogger(), "No se pudo recibir el stream del NUCLEO");
		return FALSE;
	}

	if (skc != NULL) {
		free(skc);
	}

	skc = unserializeKerCpu((Stream) sb->data);
	pcbProceso = skc->pcb;

	enviarPidPcb(skc->pcb.id);

	return TRUE;
}

void enviarPidPcb(int id) {
	espacioAsignado pag;
	StrCpuUmc*scu;
	scu = newStrCpuUmc(CPU_ID, CAMBIO_PROCESO_ACTIVO, pag, 0, 0, "h", id);
	SocketBuffer*buff = serializeCpuUmc(scu);
	if (!socketSend(socketUMC->ptrSocket, buff)) {
		log_error(getLogger(), "No se pudo enviar el ID del nuevo proceso activo al NUCLEO");
	}
}

int calcularOffset(pcb *pcbLoco) {
	arrayBidimensional aux;
	int contador = 0;
	while (contador < (pcbLoco->instruccionesTotales)) {
		if (pcbLoco->indiceDeCodigo[contador].comienzo
				== pcbLoco->programCounter) {
			aux.comienzo = pcbLoco->indiceDeCodigo[contador].comienzo;
			aux.longitud = pcbLoco->indiceDeCodigo[contador].longitud;
		}
		contador++;
	}
	return aux.longitud;
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

char* pedirInstruccion(pcb* pcbLoco) {

	int inicio = pcbLoco->indiceDeCodigo[pcbLoco->programCounter].comienzo;
	int offset = pcbLoco->indiceDeCodigo[pcbLoco->programCounter].longitud;
	char* instruccion = malloc(offset);
	instruccion[0] = '\0';
	StrCpuUmc* scu;
	StrUmcCpu* suc;

	int pagina = inicio / tamanioPaginaUmc;
	int inicioPag;
	if (pagina != 0)
		inicioPag = inicio % tamanioPaginaUmc;
	else
		inicioPag = inicio;
	int offsetPag;

	while (offset > 0) {

		if ((inicioPag + offset) > tamanioPaginaUmc) {
			offsetPag = tamanioPaginaUmc - inicioPag;
		} else {
			offsetPag = offset;
		}
		asignadoVacio->numDePag = pagina;
		asignadoVacio->bitUso = offsetPag;

		scu = newStrCpuUmc(CPU_ID, SOLICITAR_BYTES, *asignadoVacio, inicioPag,
				0,
				NULL, 0);
		buffer = serializeCpuUmc(scu);
		if (!socketSend(socketUMC->ptrSocket, buffer)) {
			log_error(getLogger(), "No se pudo enviar tu pedido a la UMC.");
		}

		if ((buffer = socketReceive(socketUMC->ptrSocket)) == NULL) {
			log_error(getLogger(),
					"No se pudo recibir la instruccion de la UMC");
			return FALSE;
		}

		suc = unserializeUmcCpu(buffer);

		strcat(instruccion, stringFromByteArray(suc->data, suc->dataLen));

		offset = offset - offsetPag;
		pagina++;
		inicioPag = 0;
	}
	return instruccion;
}

int pedirTamanioDePagina() {
	espacioAsignado aux;
	aux.IDPaginaInterno = 0;
	aux.bitDePresencia = 0;
	aux.bitModificado = 0;
	aux.bitUso = 0;
	aux.numDePag = 0;
	aux.pid = 0;
	aux.punteroAPagina = 0;

	scu = newStrCpuUmc(CPU_ID, TAMANIO_DE_MARCOS, aux, 0, 0, "HOLA", 0);
	buffer = serializeCpuUmc(scu);

	if (socketSend(socketUMC->ptrSocket, buffer)) {
		log_info(getLogger(), "Mensaje enviado a la UMC.");
	} else {
		log_error(getLogger(), "No se pudo enviar el mensaje a la UMC.");
		return FALSE;
	}

	buffer = socketReceive(socketUMC->ptrSocket);

	if (buffer == NULL) {
		log_error(getLogger(), "Error al recibir del cliente");
	}

	suc = unserializeUmcCpu(buffer);

	return (suc->dataLen);
}
