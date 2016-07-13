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
Int32U puertoNucleo;
String ipNucleo;
String ipUmc;
Int32U puertoUmc;
int tamanioPag;

/*****************************************
 * Socket & Streams
 ****************************************/
SocketClient* nucleoClient = NULL;
SocketClient* umcClient = NULL;

StrCpuKer* sck = NULL;
StrCpuUmc* scu = NULL;
StrUmcCpu* suc = NULL;
StrKerCpu* skc = NULL;

/*****************************************
 * PCB actual
 ****************************************/
pcb* pcbActual = NULL;

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
	// cargo variables de configuracion, me conecto al nucleo y a la umc

	iniciarFunciones();
	t_config* tConfig = NULL;
	puertoNucleo=(Int32U)malloc(sizeof(puertoNucleo));
	ipNucleo=(String)malloc(sizeof(ipNucleo));
	ipUmc=(String)malloc(sizeof(ipUmc));
	puertoUmc=(Int32U)malloc(sizeof(puertoUmc));


	if (loadConfig() && socketConnection()) {
		//tamanioPag = pedirTamanioDePagina();
		//tamanioPaginaUmc=tamanioPag;
		while (TRUE) {
			// devuelvo el pcb procesado y obtengo uno nuevo del nucleo
			if (!getNextPcb()) {
				return TRUE;
			}
			seguirEjecutando = TRUE;
			// proceso el pcb del nucleo
			Int8U quantum = skc->quantum;
			while (quantum > 0 && seguirEjecutando) {
				analizadorLinea(pedirInstruccion(pcbActual), &funciones,
						&funcionesDeKernel);
				moverProgramCounterPcb(pcbActual);
			}

			if (!seguirEjecutando) {
				if (pcbActual->instruccionesRestantes == 0) {
					sck = newStrCpuKer(CPU_ID, FINALIZAR_PROGRAMA, *pcbActual,
							0, 0, NULL, NULL, 0);
					buffer = serializeCpuKer(sck);
					if (!socketSend(umcClient->ptrSocket, buffer)) {
						puts("No se pudo enviar el buffer al nucleo.");
					}
				} else {
					sck = newStrCpuKer(CPU_ID, ABORTAR_PROGRAMA, *pcbActual, 0,
							0, NULL, NULL, 0);
					buffer = serializeCpuKer(sck);
					if (!socketSend(umcClient->ptrSocket, buffer)) {
						puts("No se pudo enviar el buffer al nucleo.");
					}
				}
			}

			if (quantum == 0) {
				sck = newStrCpuKer(CPU_ID, TERMINE_EL_QUANTUM, *pcbActual, 0, 0,
				NULL, NULL, 0);
				buffer = serializeCpuKer(sck);
				if (!socketSend(umcClient->ptrSocket, buffer)) {
					puts("No se pudo enviar el buffer al nucleo.");
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
}

/*****************************************
 * Implementacion de funciones
 ****************************************/

Boolean loadConfig() {
	String configFilePath = CONFIG_FILE;

	// Genero tabla de configuracion
	tConfig = config_create(configFilePath);
	if (tConfig == NULL) {
		printf(
				"ERROR: no se encuentra o falta el archivo de configuracion en la direccion '%s'.\n",
				configFilePath);
		return FALSE;
	}

	// Verifico consistencia, debe haber 4 campos
	if (config_keys_amount(tConfig) == PARAM_LENGTH) {

		// Verifico que los parametros del nucleo tengan sus valores OK
		if (config_has_property(tConfig, "PUERTO_NUCLEO")) {
			puertoNucleo = config_get_int_value(tConfig, "PUERTO_NUCLEO");
		} else {
			printf("ERROR: Falta un parametro PUERTO_NUCLEO. \n");
			return FALSE;
		}

		if (config_has_property(tConfig, "IP_NUCLEO")) {
			ipNucleo = config_get_string_value(tConfig, "IP_NUCLEO");
		} else {
			printf("ERROR: Falta un parametro IP_NUCLEO. \n");
			return FALSE;
		}
		// Verifico que los parametros de la UMC tengan sus valores OK
		if (config_has_property(tConfig, "PUERTO_UMC")) {
			puertoUmc = config_get_int_value(tConfig, "PUERTO_UMC");
		} else {
			printf("ERROR: Falta un parametro PUERTO_UMC. \n");
			return FALSE;
		}

		if (config_has_property(tConfig, "IP_UMC")) {
			ipUMC = config_get_string_value(tConfig, "IP_UMC");
		} else {
			printf("ERROR: Falta un parametro IP_UMC. \n");
			return FALSE;
		}

		printf("Archivo de config CPU leido\n============\n");
		printf("PUERTO_NUCLEO: %d\nIP_NUCLEO: %s\n", puertoNucleo, ipNucleo);
		printf("Puerto_UMC: %d\nIP_UMC: %s\n", puertoUmc, ipUmc);
		return TRUE;
	} else {
		printf(
				"ERROR: El archivo de configuracion del CPU no tiene todos los campos.\n");
		return FALSE;
	}
}

Boolean socketConnection() {
	// Conexion al nucleo
	nucleoClient = socketCreateClient();

	do {
		puts("**********************************");
		puts("Intentando conectar con el Nucleo.");
		printf("IP: %s, PUERTO: %d\n", ipNucleo, (int) puertoNucleo);
		sleep(3);
	} while (!socketConnect(nucleoClient, ipNucleo, puertoNucleo));

	if (handshake(nucleoClient, CPU_ID)) {
		puts("Handshake realizado con exito.");
	} else {
		puts("No se pudo realizar el handshake.");
		return FALSE;
	}

	//SocketClient* umcServer;
	espacioAsignado aux;
	aux.IDPaginaInterno=0;
	aux.bitDePresencia=0;
	aux.bitModificado=0;
	aux.bitUso=0;
	aux.numDePag=0;
	aux.pid=0;
	aux.punteroAPagina=0;
	umcClient=socketCreateClient();
		do {
			puts("**********************************");
			puts("Intentando conectar con la UMC ppal.");
			printf("IP: %s, PUERTO: %d\n", ipUMC, puertoUmc);
			sleep(3);
		} while (!socketConnect(umcClient, ipUMC, puertoUmc));
		//(Char id, Char action, espacioAsignado pageComienzo, Int32U offset, Int32U dataLen, Byte* data, Int32U pid)
		StrCpuUmc* out_umc_msg = newStrCpuUmc(CPU_ID, HANDSHAKE, aux, 0, 0, "", 0);
		SocketBuffer* sb = serializeCpuUmc(out_umc_msg);
		socketSend(umcClient->ptrSocket, sb);
		puts("Mensaje enviado a la UMC ppal.");
//
		sb = socketReceive(umcClient->ptrSocket);
		StrUmcCpu* in_umc_msg = unserializeUmcCpu(sb);

		int puertoNuevoUmc = in_umc_msg->dataLen;
//
		printf("Nuevo hilo UMC  es %d.\n", in_umc_msg->dataLen);
//


		umcClient=socketCreateClient();
			do {
				puts("**********************************");
				puts("Intentando conectar con el hilo de la umc.");
				printf("IP: %s, PUERTO: %d\n", ipUMC, puertoNuevoUmc);
				sleep(3);
			} while (!socketConnect(umcClient, ipUMC, puertoNuevoUmc));
			puts("Me conecte al puto hilo");

		tamanioPag=pedirTamanioDePagina();



		while(1){}

	return TRUE;
}

Boolean getNextPcb() {

	if (sck == NULL) {
		pcbActual = newEmptyPcb();
		sck = newStrCpuKer(CPU_ID, RECIBIR_NUEVO_PROGRAMA, *pcbActual, 0, 0, 0,
				NULL /*NOMBRE DISPOSITIVO*/, 0 /*LEN NOMBRE DISPOSITIVO*/);
	}
	puts("getNextPcb: Nuevo PCB vacio creado.");

	// serializo y armo el socket
	SocketBuffer* sb = serializeCpuKer(sck);
	// envio el socketBuffer
	if (!socketSend(nucleoClient->ptrSocket, sb)) {
		printf("No se pudo enviar el stream al nucleo");
	}
	free(sb);
	puts("Obtener siguiente PCB");

	// recibo la respuesta del nucleo y deserealizo
	if ((sb = socketReceive(nucleoClient->ptrSocket)) == NULL) {
		puts("No se pudo recibir el stream del nucleo");
		return FALSE;
	}

	if (skc != NULL) {
		free(skc);
	}

	skc = unserializeKerCpu((Stream) sb->data);
	*pcbActual = skc->pcb;

	enviarTamanioDePagina(skc->pcb.id);

	return TRUE;
}

void enviarTamanioDePagina(int id){
	espacioAsignado pag;
	StrCpuUmc*scu;
	scu=newStrCpuUmc(CPU_ID,TAMANIO_DE_MARCOS,pag,0,0,"h",id);
	SocketBuffer*buff= serializeCpuUmc(scu);
	socketSend(umcClient->ptrSocket,buff);

}

int calcularOffset(pcb *pcbLoco) {
	arrayBidimensional aux;
	int contador;
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
	char* instruccion = "";
	int inicio = pcbLoco->programCounter;
	int offset = calcularOffset(pcbLoco);

	StrCpuUmc* scu;
	StrUmcCpu* suc;

	int pagina = inicio / tamanioPag;

	int inicioPag = inicio % pagina;
	int offsetPag;
	espacioAsignado auxiliar;

	while (offset > 0) {

		if ((inicioPag + offset) > tamanioPag) {
			offsetPag = tamanioPag - inicioPag;
		} else {
			offsetPag = offset;
		}
		auxiliar.numDePag = pagina;
		scu = newStrCpuUmc(CPU_ID, SOLICITAR_BYTES, auxiliar, offsetPag, 0,
		NULL, 0);
		buffer = serializeCpuUmc(scu);
		if (!socketSend(umcClient, buffer)) {
			puts("No se pudo enviar tu pedido a la umc.");
		}

		buffer = socketReceive(umcClient);
		suc = unserializeUmcCpu(buffer);

		strcat(instruccion, stringFromByteArray(suc->data, suc->dataLen));

		offset = offset - offsetPag;
		pagina++;
		inicioPag = 0;
	}
	return instruccion;
}


int pedirTamanioDePagina(){
	//(Char id, Char action, espacioAsignado pageComienzo, Int32U offset, Int32U dataLen, Byte* data, Int32U pid)
	espacioAsignado aux;
		aux.IDPaginaInterno=0;
		aux.bitDePresencia=0;
		aux.bitModificado=0;
		aux.bitUso=0;
		aux.numDePag=0;
		aux.pid=0;
		aux.punteroAPagina=0;

	scu = newStrCpuUmc(CPU_ID, TAMANIO_DE_MARCOS, aux, 0, 0, "HOLA", 0);
	buffer=serializeCpuUmc(scu);
	socketSend(umcClient->ptrSocket,buffer);

	buffer = socketReceive(umcClient->ptrSocket);

	if (buffer == NULL)
		puts("Error al recibir del cliente");

	suc = unserializeUmcCpu(buffer);

	return (suc->dataLen);
}
