/*
 * swap.h
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#ifndef SWAP_H_
#define SWAP_H_


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
char* puerto_Escucha;
char* nombreSwap;
char* paginas;
char* tamPagina;
char* retCompactacion;

//prototipos
void leerArchivoDeConfiguracion();
int verificarExistenciaDeArchivo(char* rutaArchivoConfig);
void setearValores(t_config * archivoConfig);
#endif /* SWAP_H_ */
