/*
 * conexiones.h
 *
 *  Created on: 26/4/2016
 *      Author: utnso
 */

#ifndef LIBRERIASCOMPARTIDAS_CONEXIONES_H_
#define LIBRERIASCOMPARTIDAS_CONEXIONES_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/temporal.h>
#include <commons/config.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/config.h>
#define FAIL -1

void leerArchivoDeConfiguracion();

int verificarExistenciaDeArchivo(char* rutaArchivoConfig);

void setearValores(t_config * archivoConfig);

#endif /* LIBRERIASCOMPARTIDAS_CONEXIONES_H_ */
