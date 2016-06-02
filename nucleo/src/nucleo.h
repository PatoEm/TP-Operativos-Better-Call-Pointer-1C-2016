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
#include <semaphore.h>
#include <commons/temporal.h>
#include <commons/config.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>
#include <parser/metadata_program.h>
#include <parser/parser.h>
#include <libreriasCompartidas/archivosYLogsYMas.h>
#include <libreriasCompartidas/socket.h>


#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define FAIL -1
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN ( 1024 * EVENT_SIZE )
//Variables Globales

	//Variables de lectura de archivo
   char*  puertoPropio;
   char*  cpuPort;
   int  quantum;
   int  quantumSleep;
   char**   idSemaforos;
   char**  viSemaforos;
   int cantSemaforos; //No se lee por config
   t_queue **colasSemaforos; //Estas no necesitan captura por archivo de config
   char**  idIO;
   char** retardoIO;
   int cantIO;	//No se lee por config

   char** idVariableCompartida;
   int cantVarCompartidas;
   int* variableCompartidaValor;
   char* ipUMC;
   char* UMCPort;
   int stackSize;
   int tamanioPaginas;

   //Otras Variables
   int idProgramas; //Contador de programa
   bool primeraLectura;


   //Sincronizacion
   pthread_mutex_t* mutexIO;
   pthread_mutex_t* mutexVariables;
   pthread_mutex_t mutexQuantum;
   pthread_mutex_t mutexSemaforosCompartidos;



   //Colas
   pthread_mutex_t mutexColaNew;
   t_queue *colaNew;

   pthread_mutex_t mutexColaReady;
   t_queue *colaReady;
   pthread_mutex_t mutexListaExec;
   t_list *listaExec;
   pthread_mutex_t mutexListaBlock;
  t_list *listaBlock;
   pthread_mutex_t mutexColaExit;
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

    }paginaDeStack;

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
   	paginaDeStack * indiceDelStack;
   	t_medatada_program* metaProgram;
   	estadoPrograma estado;

   }pcb ;


   //Estructuras auxiliares para el funcionamiento del nucleo (NO ES PARTE DE LA PCB)



   //Prototipos

   void setearValores(t_config * archivoConfig);
   void escuchoMuchasConexiones();


   pcb crearNuevoPcb(char * programaAnsisop, int tamanioArchivo);
   void moverAColaReady(pcb * programa);
   void moverAListaBlock(pcb* programa);
   void moverAColaExit(pcb* programa);
   void finalizarProcesosColaExit();
   void verificarModificacionesArchivoConfig();
   void funcionHiloQuantum();
   void entrada_salida(char * identificador, int cantidad, pcb *pcbPrograma);
   void ejecutarIO(int posicion, pcb* pcbDelPrograma, int retardo );
   int obtener_valor(char* identificador, pcb* pcbPrograma);




   int inicializarVariables();
   void buscarYEliminarPCBEnLista(t_list * lista, pcb* pcbLoco);
#endif /* NUCLEO_H_ */
