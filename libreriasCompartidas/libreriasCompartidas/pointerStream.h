/*
 * pointerStream.h
 *
 *  Created on: 1/6/2016
 *      Author: utnso
 */

#ifndef LIBRERIASCOMPARTIDAS_POINTERSTREAM_H_
#define LIBRERIASCOMPARTIDAS_POINTERSTREAM_H_

/*******************************************************
 * Dependencias
 *******************************************************/
#include "pointerTipos.h"
#include "pointerSocketes.h"
#include "pcb.h"
#include <stdlib.h>
#include <commons/bitarray.h>
#include "espacioAsignado.h"
#include "paginaAsignada.h"
////////////////////////////////////////////////////////

/*******************************************************
 * IDs (Duenio del stream)
 ******************************************************/
#define CONSOLA_ID 0
#define KERNEL_ID 1
#define CPU_ID 2
#define UMC_ID 3
#define SWAP_ID 4
////////////////////////////////////////////////////////

/*******************************************************
 * Acciones
 *******************************************************/
#define FAIL -1
#define HANDSHAKE 5
#define STD_OUTPUT 6
#define PRIMER_PCB 7
#define IMPRIMIR 8
#define IMPRIMIRTEXTO 9
#define CERRARCONSOLA 10
#define ARCHIVO_ANSISOP 11
#define RECIBIR_NUEVO_PROGRAMA 12
#define LEER_UNA_PAGINA 13
#define ESCRIBIR_UNA_PAGINA 14
#define ELIMINAR_PROCESO 15
#define PROGRAMA_NO_RECIBIDO 16
#define PAGINA_NO_LEIDA 17
#define PAGINA_NO_ESCRITA 18
#define PROCESO_NO_ELIMINADO 19
#define INICIALIZAR_PROGRAMA 20
#define PROGRAMA_NO_INICIALIZADO 21
#define FINALIZAR_PROGRAMA 22
#define CAMBIO_PROCESO_ACTIVO 23
#define CIERRE_CONEXION_CPU 24
#define SOLICITAR_BYTES 25
#define PROGRAMA_RECIBIDO 26
#define WAIT_SEM_ANSISOP 27
#define SIGNAL_SEM_ANSISOP 28
#define ENTRADA_SALIDA 29
#define OBTENER_VALOR_COMPARTIDA 30
#define PROGRAMA_BLOQUEADO 31
#define ENVIO_PCB 32
#define ASIGNAR_VALOR_COMPARTIDA 33
#define ALMACENAR_BYTES 34
#define ABORTAR_PROGRAMA 35
#define TAMANIO_DE_MARCOS 36
#define WAIT_REALIZADO 37
#define TERMINE_EL_QUANTUM 38


#define TODO_PIOLA 69



////////////////////////////////////////////////////////

/*******************************************************
 * Estructuras de Streaming
 *******************************************************/

typedef Byte* Stream;

/*************************
 * Stream Consola-Kernel
 ************************/
typedef struct strConKer {
	Char id;
	Char action;
	Byte* fileContent;
	Int32U fileContentLen;
} __attribute__((packed)) StrConKer;


/*************************
 * Stream Kernel-CPU
 ************************/
typedef struct strKerCpu {
	Char id;
	Char action;
	pcb pcb;
	Int32U quantum;
	Int32U quantumSleep;
	Byte* data;
	Int32U dataLen;
	Byte* nombreDispositivo;
	Int32U lenNomDispositivo;
} __attribute__((packed)) StrKerCpu;


/*************************
 * Stream Kernel-UMC
************************/
typedef struct strKerUmc {
	Char id;
	Char action;
	Byte* data;
	Int32U size;
	Int32U pid;
	Int32U cantPage;
	Int32U pagina;
	Int32U offset;
	Int32U tamanio;
} __attribute__((packed)) StrKerUmc;


/*************************
 * Stream Kernel-Consola
 ************************/
typedef struct strKerCon {
	Char id;
	Char action;
	Int32U valorImp;
	Int32U logLen;
	Byte* log;
} __attribute__((packed)) StrKerCon;


/*************************
 * Stream CPU-Kernel
 ************************/
typedef struct strCpuKer {
	Char id;
	Char action;
	pcb pcb;
	Int32U pid;
	Int32U logLen;
	Byte* log;
	Byte* nombreDispositivo;
	Int32U lenNomDispositivo;
} __attribute__((packed)) StrCpuKer;


/*************************
 * Stream CPU-UMC
 ************************/
typedef struct strCpuUmc {
	Char id;
	Char action;
	espacioAsignado pageComienzo;
	Int32U offset;
	Int32U dataLen;
	Byte* data;
	Int32U pid;
} __attribute__((packed)) StrCpuUmc;


/*************************
 * Stream UMC-Kernel
 ************************/
typedef struct strUmcKer {
	Char id;
	Char action;
	Byte* data;
	Int32U size;
	Int32U pid;
	Int32U cantPage;
} __attribute__((packed)) StrUmcKer;


/*************************
 * Stream UCM-CPU
 ************************/
typedef struct strUmcCpu {
	Char id;
	Char action;
	espacioAsignado pageComienzo;
	Int32U offset;
	Int32U dataLen;
	Byte* data;
	Int32U pid;
} __attribute__((packed)) StrUmcCpu;


/*************************
 * Stream Umc-Swap
 ************************/
typedef struct strUmcSwa {
	Char id;
	Char action;
	espacioAsignado pageComienzo;
	Int32U cantPage;
	Byte* data;
	Int32U dataLen;
	Int32U pid;
} __attribute__((packed)) StrUmcSwa;


/*************************
 * Stream Swap-Umc
 ************************/
typedef struct strSwaUmc {
	Char id;
	Char action;
	paginaAsignada pageComienzo;
	Int32U cantPage;
	Byte* data;
	Int32U dataLen;
	Int32U pid;
} __attribute__((packed)) StrSwaUmc;

////////////////////////////////////////////////////////

/***********************************************/
/* Constructores
***********************************************/
StrConKer* newStrConKer(Char id, Char action, Byte* fileContent, Int32U fileContentLen);

StrKerCpu* newStrKerCpu(Char id, Char action, pcb pcb, Int32U quantum,Int32U quantumSleep, Byte* data, Int32U dataLen, Byte* nombreDispositivo, Int32U lenNomDispositivo);
StrKerUmc* newStrKerUmc(Char id, Char action, Byte* data, Int32U size, Int32U pid, Int32U cantPage, Int32U pagina, Int32U offset, Int32U tamanio);
StrKerCon* newStrKerCon(Char id, Char action,Int32U valorImp ,Int32U logLen, Byte* log);

StrCpuKer* newStrCpuKer(Char id, Char action, pcb pcb, Int32U pid, Int32U logLen, Byte* log, Byte* nombreDispositivo, Int32U lenNomDispositivo);
StrCpuUmc* newStrCpuUmc(Char id, Char action, espacioAsignado pageComienzo, Int32U offset, Int32U dataLen, Byte* data, Int32U pid);

StrUmcKer* newStrUmcKer(Char id, Char action, Byte* data, Int32U size, Int32U pid, Int32U cantPage);
StrUmcCpu* newStrUmcCpu(Char id, Char action, espacioAsignado pageComienzo, Int32U offset, Int32U dataLen, Byte* data, Int32U pid);
StrUmcSwa* newStrUmcSwa(Char id, Char action, espacioAsignado pageComienzo, Int32U cantPage, Byte* data, Int32U dataLen, Int32U pid);

StrSwaUmc* newStrSwaUmc(Char id, Char action, paginaAsignada pageComienzo, Int32U cantPage, Byte* data, Int32U dataLen, Int32U pid);
/***********************************************/

/***********************************************/
/* Sizes
***********************************************/
Int32U getSizeConKer(StrConKer* sconk);

Int32U getSizeKerCpu(StrKerCpu* skc);
Int32U getSizeKerUmc(StrKerUmc* sku);
Int32U getSizeKerCon(StrKerCon* skcon);

Int32U getSizeCpuKer(StrCpuKer* sck);
Int32U getSizeCpuUmc(StrCpuUmc* scu);

Int32U getSizeUmcKer(StrUmcKer* suk);
Int32U getSizeUmcCpu(StrUmcCpu* suc);
Int32U getSizeUmcSwa(StrUmcSwa* sus);

Int32U getSizeSwaUmc(StrSwaUmc* ssu);
/***********************************************/
/* Serialize
***********************************************/
SocketBuffer* serializeConKer(StrConKer* conKer);

SocketBuffer* serializeKerCpu(StrKerCpu* kerCpu);
SocketBuffer* serializeKerUmc(StrKerUmc* kerUmc);
SocketBuffer* serializeKerCon(StrKerCon* kerCon);

SocketBuffer* serializeCpuKer(StrCpuKer* cpuKer);
SocketBuffer* serializeCpuUmc(StrCpuUmc* cpuUmc);

SocketBuffer* serializeUmcKer(StrUmcKer* umcKer);
SocketBuffer* serializeUmcCpu(StrUmcCpu* umcCpu);
SocketBuffer* serializeUmcSwa(StrUmcSwa* umcSwa);

SocketBuffer* serializeSwaUmc(StrSwaUmc* swaUmc);
/***********************************************/

/***********************************************/
/* Unserialize
***********************************************/
StrConKer* unserializeConKer(Stream conKer);

StrKerCpu* unserializeKerCpu(Stream kerCpu);
StrKerUmc* unserializeKerUmc(Stream kerUmc);
StrKerCon* unserializeKerCon(Stream kerCon);

StrCpuKer* unserializeCpuKer(Stream cpuKer);
StrCpuUmc* unserializeCpuUmc(Stream cpuUmc);

StrUmcKer* unserializeUmcKer(Stream umcKer);
StrUmcCpu* unserializeUmcCpu(Stream umcCpu);
StrUmcSwa* unserializeUmcSwa(Stream umcSwa);

StrSwaUmc* unserializeSwaUmc(Stream swaUmc);
/***********************************************/

/***********************************************/
/* Para Handshake
***********************************************/
Char getStreamId(Stream stream);

/***********************************************/
/* bitarrayToSocketBuffer
***********************************************/
SocketBuffer* bitarrayToSocketBuffer(t_bitarray* bitarray);

#endif /* LIBRERIASCOMPARTIDAS_POINTERSTREAM_H_ */
