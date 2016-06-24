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
#include "commons/collections/dictionary.h"
#include "libreriasCompartidas/pointerStream.h"
#include "libreriasCompartidas/pointerSocketes.h"
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/*****************************************
 * Constantes
 ****************************************/
#define CONFIG_FILE "configcpu.txt"
#define PARAM_LENGTH 4
#define PUERTO_KERNEL "PUERTO_KERNEL"
#define IP_KERNEL "127.0.0.1"
#define PUERTO_UMC "PUERTO_UMC"
#define IP_UMC "IP_UMC"

/*****************************************
 * Configuracion
 ****************************************/
t_config* tConfig = NULL;
Int32U puertoKernel;
String ipKernel;
Int32U puertoUmc;
String ipUmc;

/*****************************************
 * Socket & Streams
 ****************************************/
SocketClient* kernelClient = NULL;
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
 * Informacion para las funciones
 ****************************************/
t_dictionary* functionsDictionary = NULL;

/*****************************************
 * Funciones del main
 ****************************************/
Boolean loadConfig();
Boolean socketConnection();
Boolean getNextPcb();
Boolean processPcb();

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

//==============================================================================================

//	leerArchivoDeConfiguracion("configcpu");

//==============================================================================================

	SocketClient* nucleo =  socketCreateClient();

	do {
		puts("Intentando conectar con el Nucleo.");
		printf("IP: %s, Puerto: %s\n", IP_KERNEL, PUERTO_KERNEL);
		sleep(3);
	} while(!socketConnect(nucleo, IP_KERNEL, (Int32U) 2020));

	if(handshake(nucleo, CPU_ID)){
		puts("Handshake realizado con exito.");
	} else {
		puts("No se pudo realizar el handshake.");
		return -1;
	}

//	getNextPcb();

	while(1){
		puts("Esperando..");
		sleep(10);
	}

//	if (loadFunctionDictionary(&functionsDictionary) && loadConfig() && socketConnection()) {
//		while (TRUE){
//			if(!getNextPcb()) {
//				return TRUE;
//			}
//
//			if(!processPcb()) {
//				return TRUE;
//			}
//		}
//
//	}
//	config_destroy(tConfig);
	return FALSE;
}

/*****************************************
 * Implementacion de funciones
 ****************************************/

Boolean loadConfig() {
	String configFilePath = CONFIG_FILE;

	// Genero tabla de configuracion
	tConfig = config_create(configFilePath);
	if (tConfig == NULL) {
		printf("ERROR: no se encuentra o falta el archivo de configuracion en la direccion '%s'.\n",configFilePath);
		return FALSE;
	}

	// Verifico consistencia, debe haber 4 campos
	if (config_keys_amount(tConfig) == PARAM_LENGTH) {

		// Verifico que los parametros del Kernel tengan sus valores OK
		if (config_has_property(tConfig, PUERTO_KERNEL)) {
			puertoKernel = config_get_int_value(tConfig, PUERTO_KERNEL);
		} else {
			printf("ERROR: Falta un parametro. \n");
			return FALSE;
		}

		if (config_has_property(tConfig, IP_KERNEL)) {
			ipKernel = config_get_string_value(tConfig, IP_KERNEL);
		} else {
			printf("ERROR: Falta un parametro. \n");
			return FALSE;
		}
		// Verifico que los parametros de la UMC tengan sus valores OK
		if (config_has_property(tConfig, PUERTO_UMC)) {
			puertoUmc = config_get_int_value(tConfig, PUERTO_UMC);
		} else {
			printf("ERROR: Falta un parametro. \n");
			return FALSE;
		}

		if (config_has_property(tConfig, IP_UMC)) {
			ipUmc = config_get_string_value(tConfig, IP_UMC);
		} else {
			printf("ERROR: Falta un parametro. \n");
			return FALSE;
		}

		printf("Archivo de config CPU leido\n============\n");
		printf("Puerto_Kernel: %d\nIP_Kernel: %s\n", puertoKernel, ipKernel);
		printf("Puerto_UMC: %d\nIP_UMC: %s\n", puertoUmc, ipUmc);
		return TRUE;
	} else {
		printf("ERROR: El archivo de configuracion del CPU no tiene todos los campos.\n");
		return FALSE;
	}
}

Boolean socketConnection() {
	// Conexion al kernel
	kernelClient = socketCreateClient();
	printf("Conectando al kernel");
	if (!socketConnect(kernelClient, ipKernel, puertoKernel)) {
		printf("No se pudo conectar al kernel en la direccion %s puerto %d \n", ipKernel, puertoKernel);
		return FALSE;
	}
	printf("Haciendo HANDSHAKE con el kernel");
	if (handshake(kernelClient, CPU_ID)) {
		printf("CPU-KER: Handshake realizado con exito");
	} else {
		printf("CPU-KER: No se pudo realizar el handshake");
		return FALSE;
	}

	// Conexion a la UMC
	umcClient = socketCreateClient();
	printf("Conectando a la UMC");
	if (!socketConnect(umcClient, ipUmc, puertoUmc)) {
		printf("No se pudo conectar a la UMC en la direccion %s puerto %d \n", ipUmc, puertoUmc);
		return FALSE;
	}
	printf("Haciendo HANDSHAKE con la UMC");
	if (handshake(umcClient, CPU_ID)) {
		printf("CPU-UMC: Handshake realizado con exito");
	} else {
		printf("CPU-UMC: No se pudo realizar el handshake");
		return FALSE;
	}
	return TRUE;
}

Boolean getNextPcb() {
	if (sck == NULL) {
		pcbActual = newEmptyPcb();
		sck = newStrCpuKer(CPU_ID, /*PRIMER_PCB*/ 7, *pcbActual, 0, 0, 0);
	}
	puts("getNextPcb: Nuevo PCB vacio creado.");
	// serializo y armo el socket
	SocketBuffer* sb = serializeCpuKer(sck);
	if (!socketSend(kernelClient->ptrSocket, sb)) {
		printf("No se pudo enviar el stream al kernel");
	}
	free(sb);
	printf("Obtener siguiente PCB");

	if ((sb = socketReceive(kernelClient->ptrSocket)) == NULL) {
		printf("No se pudo recibir el stream del kernel");
		return FALSE;
	}

	skc = unserializeKerCpu((Stream)sb->data);
	//*pcbActual = skc->pcb;

	return TRUE;
}

Boolean processPcb() {
	return TRUE;
}
