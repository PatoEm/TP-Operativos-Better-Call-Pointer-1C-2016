/*
 * consola.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */
#include "consola.h"
#define manejarError(msg) {perror(msg); abort();}



void leerArchivoDeConfiguracion() {

             t_config* archivoDeConfiguracion;
             char*configPath = "consola/configconsola";

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
		ipNucleo = config_get_string_value(archivoConfig, "IP_NUCLEO");
		nucleoPort= config_get_string_value(archivoConfig, "PUERTO_NUCLEO");
}
