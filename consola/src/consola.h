/*
 * consola.h
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#ifndef CONSOLA_H_
#define CONSOLA_H_


#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/config.h>
#include <commons/log.h>
#include <libreriasCompartidas/socket.h>
#include <libreriasCompartidas/conexiones.h>

// define
#define FAIL -1
#define IMPRIMIR 3
#define IMPRIMIRTEXTO 4

//variables
char* ipNucleo;
char* nucleoPort;
long tamanio;
char* buffer;


//prototipos
//void leerArchivoDeConfiguracion();
//int verificarExistenciaDeArchivo(char* rutaArchivoConfig);
//void setearValores(t_config * archivoConfig);
long tamanioArchivo(char * direccionArchivo);
char * leerProgramaAnsisop(char * direccionArchivo);

#endif /* CONSOLA_H_ */
