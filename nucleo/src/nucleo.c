/*
 * nucleo.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#include "nucleo.h"
#define manejarError(msg) {perror(msg); abort();}


void setearValores(t_config * archivoConfig) {
		puertoPropio = config_get_string_value(archivoConfig, "PUERTO_PROPIO");
		cpuPort= config_get_string_value(archivoConfig, "PUERTO_CPU");
		ipUMC= config_get_string_value(archivoConfig, "IP_UMC");
		UMCPort=config_get_string_value(archivoConfig, "PUERTO_UMC");
		quantum= config_get_int_value(archivoConfig, "QUANTUM");
		quantumSleep = config_get_int_value(archivoConfig, "QUANTUM_SLEEP");
		idSemaforos = config_get_array_value(archivoConfig,"SEM_IDS");
		viSemaforos = config_get_array_value(archivoConfig,"SEM_INIT");
		idIO = config_get_array_value(archivoConfig,"IO_IDS");
		retardoIO = config_get_array_value(archivoConfig,"IO_SLEEP");
		idVariableCompartida = config_get_array_value(archivoConfig,"SHARED_VARS");

}

