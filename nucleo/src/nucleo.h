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
#include <libreriasCompartidas/pointerSocketes.h>
#include <libreriasCompartidas/pcb.h>
#include <libreriasCompartidas/pointerStream.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>


#define FAIL -1
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN ( 1024 * EVENT_SIZE )





//Variables Globales

//Variables de lectura de archivo
char* puertoPropio;
char* cpuPort;
int quantum;
int quantumSleep;
char** idSemaforos;
char** viSemaforos;
int cantSemaforos; //No se lee por config
char** idIO;
char** retardoIO;
int cantIO;	//No se lee por config

char** idVariableCompartida;
int cantVarCompartidas;
int* variableCompartidaValor;
char* ipUMC;
char* UMCPort;
int stackSize;
int tamanioPaginas;

SocketClient * umcServer;

//Otras Variables
int idProgramas; //Contador de programa
bool primeraLectura;
t_log* nucleolog;

//Sincronizacion
pthread_mutex_t** mutexIO;
pthread_mutex_t** mutexVariables;
pthread_mutex_t* mutexQuantum;
sem_t ** semaforosAnsisop;
pthread_mutex_t* mutexColaNew;
pthread_mutex_t* mutexColaReady;
pthread_mutex_t* mutexColaExit;
pthread_mutex_t* mutexListaExec;
pthread_mutex_t* mutexListaBlock;

//Colas y listas

//t_queue *colaNew;

//t_queue *colaReady;

t_list *listaNew;

t_list *listaReady;

t_list *listaExec;

t_list *listaBlock;

t_list *listaExit;

//t_queue *colaExit;


//Estructuras auxiliares para el funcionamiento del nucleo (NO ES PARTE DE LA PCB)
typedef struct  {

	char* identificador;
	int cantidad;
	pcb* pcbLoca;

} atributosIO;

typedef struct  {
	Socket* cpuSocket;
	char* identificador;
	pcb* pcbLoca;

} atributosWait;

//Prototipos

void setearValores(t_config * );
pcb* crearNuevoPcb(Socket*, char * , int ) ;
void moverAColaReady(pcb * );
void moverAListaBlock(pcb* );
void moverAListaExec(pcb* );
void moverAColaExit(pcb* );
void finalizarProcesosColaExit();
void escuchoMuchasConexiones();
void funcionHiloQuantum();
void verificarModificacionesArchivoConfig() ;
void entrada_salida(char * , int , pcb *);
void ejecutarIO(int , pcb* , int ) ;
int obtener_valor(char*);
void grabar_valor(char* , int );
void waitAnsisop(char * , pcb *,Socket*);
void signalAnsisop(char*);
int inicializarVariables();
void crearHilos();
void enviarPcbACpu(Socket *);
void *buscarYEliminarPCBEnLista(t_list *, pcb* );
void funcionHiloIO(atributosIO);
void funcionHiloWait(atributosWait );

#endif /* NUCLEO_H_ */
