/*
 * conexiones.c
 *
 *  Created on: 26/4/2016
 *      Author: utnso
 */
#include "conexiones.h"

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

