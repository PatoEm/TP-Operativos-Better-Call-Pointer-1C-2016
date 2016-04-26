/*
 * consola.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */
#include "consola.h"
#define manejarError(msg) {perror(msg); abort();}



void setearValores(t_config * archivoConfig) {
		ipNucleo = config_get_string_value(archivoConfig, "IP_NUCLEO");
		nucleoPort= config_get_string_value(archivoConfig, "PUERTO_NUCLEO");
}
