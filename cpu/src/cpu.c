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

t_puntero definirVariable(t_nombre_variable variable){

	printf("defino una variable");

}

t_puntero obtenerPosicionvariable(t_nombre_variable variable){

	printf("devuelvo la posición de una variable");

}

t_valor_variable dereferenciar(t_nombre_variable variable){

	printf("dereferencio una variable");

}

void asignar(t_puntero puntero, t_valor_variable variable){

	printf("asigno una variable");

}

