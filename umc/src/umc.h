
#ifndef UMC_H_
#define UMC_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/config.h>
#include <commons/log.h>
#include <libreriasCompartidas/archivosYLogsYMas.h>
#include <libreriasCompartidas/socket.h>
#include <pthread.h>
#define FAIL -1

//parametros
char* puertoEscucha;
char* ip_Swap;
char* puerto_Swap;
char* marcos;
char* marco_Size;
char* marco_x_proc;
char* entradas_TLB;
char* espera;

//estructuras para los hilos de CPU y nucleo

typedef struct{
	//en estos campos van las cosas que recibe el hilo para laburar con el nucleo
}t_nucleo;

typedef struct{
	//en estos campos van las cosas que recibe el hilo para laburar con el CPU
}t_CPU;

//Prototipos
char * reservarMemoria(int , int );

void liberarMemoria(char * );

void escuchoMuchasConexiones();

void meHablaKernel();

void meHablaCPU();

void meHablaKernelPrueba ();

void meHablaCPUPrueba ();

#endif /* UMC_H_ */
