/*
 * cpu.h
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/config.h>
#include <commons/log.h>
#include <libreriasCompartidas/socket.h>

#define FAIL -1

//variables
char* ipUMC;
char* umcPort;

//prototipos
void leerArchivoDeConfiguracion();
int verificarExistenciaDeArchivo(char* rutaArchivoConfig);
void setearValores(t_config * archivoConfig);

#endif /* CPU_H_ */
