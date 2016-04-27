/*
 * conexiones.c
 *
 *  Created on: 26/4/2016
 *      Author: utnso
 */
#include "conexiones.h"

void leerArchivoDeConfiguracion(const char * direccionArchivo) {

             t_config* archivoDeConfiguracion;
             char*configPath = direccionArchivo;

            if (verificarExistenciaDeArchivo(configPath) == FAIL)
		puts("[ERROR] Archivo de configuracion no encontrado");


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

