/*
 * cpu.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */
#include "cpu.h"
#define manejarError(msg) {perror(msg); abort();}





void setearValores(t_config * archivoConfig) {
		ipUMC = config_get_string_value(archivoConfig, "IP_UMC");
		umcPort = config_get_string_value(archivoConfig, "PUERTO_UMC");
		ipNucleo = config_get_string_value(archivoConfig, "IP_NUCLEO");
		nucleoPort = config_get_string_value(archivoConfig, "PUERTO_NUCLEO");

}
