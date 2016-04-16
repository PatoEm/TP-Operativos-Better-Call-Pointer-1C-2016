/*
 * nucleo.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#include "nucleo.h"
#define manejarError(msg) {perror(msg); abort();}

void leerArchivoDeConfiguracion() {

             t_config* archivoDeConfiguracion;
             char*configPath = "nucleo/confignucleo";

            if (verificarExistenciaDeArchivo(configPath) == FAIL)
		manejarError("[ERROR] Archivo de configuracion no encontrado");


             archivoDeConfiguracion = config_create(configPath);
             setearValores( archivoDeConfiguracion);
	         config_destroy( archivoDeConfiguracion);

}

int verificarExistenciaDeArchivo(char* rutaArchivoConfig) {
	FILE * archivoConfig = fopen(rutaArchivoConfig, "r");
	if (archivoConfig!=NULL){
		fclose(archivoConfig);
		return 1;
	}
	return FAIL;
}

void setearValores(t_config * archivoConfig) {
		progPort = config_get_string_value(archivoConfig, "PUERTO_PROG");
		cpuPort= config_get_string_value(archivoConfig, "PUERTO_CPU");
		quantum= config_get_int_value(archivoConfig, "QUANTUM");
		quantumSleep = config_get_int_value(archivoConfig, "QUANTUM_SLEEP");
		idSemaforos = config_get_array_value(archivoConfig,"SEM_IDS");
		viSemaforos = config_get_array_value(archivoConfig,"SEM_INIT");
		idIO = config_get_array_value(archivoConfig,"IO_IDS");
		retardoIO = config_get_array_value(archivoConfig,"IO_SLEEP");
		idVariableCompartida = config_get_array_value(archivoConfig,"SHARED_VARS");

}
