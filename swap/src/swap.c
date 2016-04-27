/*
 * swap.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#include "swap.h"

#define manejarError(msg) {perror(msg); abort();}


void setearValores(t_config * archivoConfig) {
		umcPort = config_get_string_value(archivoConfig, "PUERTO_UMC");
		nombreSwap = config_get_string_value(archivoConfig, "NOMBRE_SWAP");
		paginas = config_get_string_value(archivoConfig, "CANTIDAD_PAGINAS");
		tamPagina = config_get_string_value(archivoConfig, "TAMANO_PAGINA");
		retCompactacion = config_get_string_value(archivoConfig, "RETARDO_COMPACTACION");

}

