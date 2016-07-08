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
#define CONFIG_FILE "configcpu"
#define PARAM_LENGTH 4
#define PUERTO_KERNEL "PUERTO_KERNEL"
#define IP_KERNEL "IP_KERNEL"
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


	SocketClient* nucleo =  socketCreateClient();

	if (/* loadFunctionDictionary(&functionsDictionary) && */ loadConfig() && socketConnection()) {
		while (TRUE){
			if(!getNextPcb()) {
				return TRUE;
			}

			if(!processPcb()) {
				return TRUE;
			}
		}

	}
	config_destroy(tConfig);
	while(1){
		puts("Esperando..");
		sleep(10);
	}
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
			printf("ERROR: Falta un parametro PUERTO_KERNEL. \n");
			return FALSE;
		}

		if (config_has_property(tConfig, IP_KERNEL)) {
			ipKernel = config_get_string_value(tConfig, IP_KERNEL);
		} else {
			printf("ERROR: Falta un parametro IP_KERNEL. \n");
			return FALSE;
		}
		// Verifico que los parametros de la UMC tengan sus valores OK
		if (config_has_property(tConfig, PUERTO_UMC)) {
			puertoUmc = config_get_int_value(tConfig, PUERTO_UMC);
		} else {
			printf("ERROR: Falta un parametro PUERTO_UMC. \n");
			return FALSE;
		}

		if (config_has_property(tConfig, IP_UMC)) {
			ipUmc = config_get_string_value(tConfig, IP_UMC);
		} else {
			printf("ERROR: Falta un parametro IP_UMC. \n");
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

	do {
		puts("**********************************");
		puts("Intentando conectar con el Nucleo.");
		printf("IP: %s, Puerto: %d\n", ipKernel, (int)puertoKernel);
		sleep(3);
	} while(!socketConnect(kernelClient, ipKernel, puertoKernel));

	if(handshake(kernelClient, CPU_ID)){
		puts("Handshake realizado con exito.");
	} else {
		puts("No se pudo realizar el handshake.");
		return FALSE;
	}

	// Conexion a la UMC
	umcClient = socketCreateClient();

	do {
		puts("**********************************");
		puts("Intentando conectar con la UMC.");
		printf("IP: %s, Puerto: %d\n", ipUmc, (int)puertoUmc);
		sleep(3);
	} while(!socketConnect(umcClient, ipUmc, puertoUmc));

	if(handshake(umcClient, CPU_ID)){
		puts("Handshake realizado con exito.");
	} else {
		puts("No se pudo realizar el handshake.");
		return FALSE;
	}

	return TRUE;
}

Boolean getNextPcb() {
	if (sck == NULL) {
		pcbActual = newEmptyPcb();
		sck = newStrCpuKer(CPU_ID, PRIMER_PCB, *pcbActual, 0, 0, 0);
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
