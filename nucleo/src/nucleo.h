/*
 * nucleo.h
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#ifndef NUCLEO_H_
#define NUCLEO_H_

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <commons/temporal.h>
#include <commons/config.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/queue.h>
#include <parser/metadata_program.h>
#include <parser/parser.h>
#include <libreriasCompartidas/archivosYLogsYMas.h>
#include <libreriasCompartidas/socket.h>


#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define FAIL -1
#define EVENT_SIZE ( sizeof (struct inotify_event) + 24 )
#define BUF_LEN ( 1024 * EVENT_SIZE )
//Variables Globales

	//Variables de lectura de archivo
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

   //Colas
   t_queue *colaNew;
   t_queue *colaReady;
   t_queue *colaExec;
   t_queue *colaBlock;
   t_queue *colaExit;

   //Estructuras PCB

   typedef struct{
   	int comienzo;
   	int longitud;
   }arrayBidimensional;

   typedef struct {
 	   int id;
 	   int pag;
 	   int off;
 	   int size;
    }variables;
    typedef struct{
 	   int pag;
 	   int off;
 	   int size;
    }argumentos;

    typedef struct {
 	   int pos;
 	   argumentos args;
 	   variables vars;
 	   int retPos;
 	   variables retVars;

    }indiceDeStack;

   typedef enum {
        NEW=0,
        READY=1,
        EXEC=2,
		BLOCK=3,
        EXIT=4
    }estadoPrograma;


   typedef struct{
   	int id;
   	int tamanioArchivoOriginal;
   	t_puntero_instruccion programCounter;
   	int paginasDeCodigo;
   	arrayBidimensional * indiceDeCodigo;
   	char * indiceDeEtiquetas;
   	indiceDeStack * indiceDelStack;
   	t_medatada_program* metaProgram;
   	estadoPrograma estado;

   }pcb ;

   //Prototipos

   void setearValores(t_config * archivoConfig);
   void escuchoMuchasConexiones();


   pcb crearNuevoPcb(char * programaAnsisop, int tamanioArchivo);
   void moverAColaReady(pcb * programa);
   void moverAColaBlock(pcb* programa);
   void moverAColaExit(pcb* programa);
   void finalizarProcesosColaExit();
   void verificarModificacionesArchivoConfig();


#endif /* NUCLEO_H_ */
