/*
 * cpu.h
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_

/*******************************************************
 * DEPENDENCIAS
 *******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/config.h>
#include <parser/parser.h>
#include <parser/metadata_program.h>
#include <commons/log.h>
#include <pthread.h>
#include <libreriasCompartidas/archivosYLogsYMas.h>
#include <libreriasCompartidas/pcb.h>
#include <libreriasCompartidas/pointerSocketes.h>
#include <commons/collections/list.h>
#include <libreriasCompartidas/pcb.h>
#include <signal.h>
#include <libreriasCompartidas/espacioAsignado.h>
#include <sys/types.h>
#include <unistd.h>

#define FAIL -1
#define LOGGER_FILE "cpu_log.txt"
//======================================================

/*******************************************************
 * VARIABLES GLOBALES
 *******************************************************/
String ipUMC;
Int32U umcPort;
char* etiquetaMod;
Int32U nucleoPort;
String ipNucleo;
int clienteNucleo;
t_list* coreList;
SocketClient* socketNucleo;
SocketClient* socketUMC;
int tamanioPaginaUmc;
pcb *pcbProceso;
bool seguirEjecutando; //1 SI 0 NO
espacioAsignado * asignadoVacio;
bool finalizoCorrectamente;
bool saltoDeLinea;
bool devolverPCB;
int stackSize;
bool elAbortador;

//======================================================

/*******************************************************
 * TIPOS DE DATOS
 *******************************************************/

//typedef struct core {
//	bool online;
//	bool busy;
//	int socket;
//} t_core;

//======================================================
// Prototipo de funcion para loggear
t_log* getLogger();

/*******************************************************
 * FUNCIONES AnSISOP
 *******************************************************/

/*
 * definirVariable
 */
t_puntero definirVariable(t_nombre_variable identificador_variable);

/*
 * obtenerPosicionVariable
 */
t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable);

/*
 * dereferenciar
 */
t_valor_variable dereferenciar(t_puntero direccion_variable);

/*
 * asignar
 */
void asignar(t_puntero direccion_variable, t_valor_variable valor);

/*
 * obtenerValorCompartida
 */
t_valor_variable obtenerValorCompartida(t_nombre_compartida variable);

/*
 * 	asignarValorCompartida
 */
t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor);

/*
 * 	irAlLabel
 */
void irAlLabel(t_nombre_etiqueta etiqueta);

/*
 * llamarConRetorno
 */
void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar);

/*
 * retornar
 *
 */
void retornar(t_valor_variable retorno);

/*
 * imprimir
 */
void imprimir(t_valor_variable valor_mostrar);

/*
 * imprimirTexto
 */
int imprimirTexto(char* texto);

/*
 * entradaSalida
 */
void entradaSalida(t_nombre_dispositivo dispositivo, int tiempo);

/*
 * wait
 */
void wait(t_nombre_semaforo identificador_semaforo);

/*
 * signal
 */
void signale(t_nombre_semaforo identificador_semaforo);


bool espacioMemoriaVacio(int tamanio, char*bytes);


void abortarCPU();


char* sinEspacioAlFinal(char* linea, int tamanio);


void gestionoSIGINT();
//======================================================

AnSISOP_funciones funciones;
AnSISOP_kernel funcionesDeKernel;

void iniciarFunciones();

String intToStr(Int32U integer);

// OTRAS

bool esperarConfirmacion(SocketClient*);

String stringFromByteArray(Byte* data, Int32U size);

#endif /* CPU_H_ */
