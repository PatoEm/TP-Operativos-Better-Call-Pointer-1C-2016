/*
 * umc.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */
#include "umc.h"
#define manejarError(msg) {perror(msg); abort();}


void leerArchivoDeConfiguracion() {

             t_config* archivoDeConfiguracion;
             char*configPath = "umc/configumc";

            if (verificarExistenciaDeArchivo(configPath) == FAIL)
		manejarError("[ERROR] Archivo de configuracion no encontrado");


             archivoDeConfiguracion = config_create(configPath);
             setearValores(archivoDeConfiguracion);
	         config_destroy(archivoDeConfiguracion);

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
		puertoTCP = config_get_string_value(archivoConfig, "PUERTO");
		puertoNucleo = config_get_string_value(archivoConfig, "PUERTO_NUCLEO");
		puertoCPU = config_get_string_value(archivoConfig, "PUERTO_CPU");
		ip_Swap = config_get_string_value(archivoConfig, "IP_SWAP");
		puerto_Swap = config_get_int_value(archivoConfig, "PUERTO_SWAP");
		marcos = config_get_int_value(archivoConfig, "MARCOS");
		marco_Size = config_get_array_value(archivoConfig,"MARCO_SIZE");
		marco_x_proc = config_get_array_value(archivoConfig,"MARCO_X_PROC");
		entradas_TLB = config_get_array_value(archivoConfig,"ENTRADAS_TLB");
		espera = config_get_array_value(archivoConfig,"RETARDO");

}
