/*
 * swap.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#include "swap.h"

#define manejarError(msg) {perror(msg); abort();}



void leerArchivoDeConfiguracion() {

             t_config* archivoDeConfiguracion;
             char*configPath = "swap/configswap";

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
		umcPort = config_get_string_value(archivoConfig, "PUERTO_UMC");
		nombreSwap = config_get_string_value(archivoConfig, "NOMBRE_SWAP");
		paginas = config_get_string_value(archivoConfig, "CANTIDAD_PAGINAS");
		tamPagina = config_get_string_value(archivoConfig, "TAMANO_PAGINA");
		retCompactacion = config_get_string_value(archivoConfig, "RETARDO_COMPACTACION");

}
