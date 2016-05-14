/*
 * nucleo.h
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#ifndef NUCLEO_H_
#define NUCLEO_H_


#include <stdio.h>
#include <stdlib.h>
#include <commons/temporal.h>
#include <commons/config.h>
#include <commons/config.h>
#include <commons/log.h>
#include <libreriasCompartidas/archivosYLogsYMas.h>
#include <libreriasCompartidas/socket.h>
#include <parser/metadata_program.h>
#include <parser/parser.h>
#define FAIL -1

//variables


   char*  puertoPropio;
   char*  cpuPort;
   int  quantum;
   int  quantumSleep;
   char**   idSemaforos;
   char**  viSemaforos;
   char**  idIO;
   char** retardoIO;
   char** idVariableCompartida;
   char* ipUMC;
   char* UMCPort;
   int tamanioPaginas;
   int idProgramas;





   typedef struct{

   	int comienzo;
   	int longitud;
   }arrayBidimensional;

   typedef struct{
   	t_puntero_instruccion direccion;
   	char etiqueta;

   }etiquetaLoca;

   typedef struct{
   	int id;
   	int tamanio;
   	t_puntero_instruccion programCounter;
   	int paginasDeCodigo;
   	arrayBidimensional * indiceDeCodigo;
   	int indiceDeEtiquetas;
   	int indiceDelStack;
   	t_medatada_program* metaProgram;


   }pcb ;


   pcb crearNuevoPcb(char * programaAnsisop, int tamanioArchivo);
   void crearIndiceEtiquetas(t_medatada_program* metadata);

#endif /* NUCLEO_H_ */
