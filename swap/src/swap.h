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
#include <libreriasCompartidas/archivosYLogsYMas.h>
#include <libreriasCompartidas/socket.h>
#define FAIL -1
#define RECIBIRTAMANIO 1

//variables
char* ipUMC;
char* umcPort;
char* puerto_Escucha;
char* nombreSwap;
char* paginas;
char* tamPagina;
char* retCompactacion;

/*
 *
 * typedef struct espacioLibre{
 * 	int inicio;
 * 	int swap_tamanio;
 * };
 *
 * typedef struct espacioAsignado[
 * 	int pid;
 * 	int inicio;
 * 	int swap_tamanio;
 * };
 *
 * t_lista * listaEspacioLibre;
 * t_lista * listaEspacioAsignado;
 */


#endif /* SWAP_H_ */
