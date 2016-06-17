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

/* aca van los defines de actions*/

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
	//Int32U tid; duda si lo dejo o no
} __attribute__((packed)) StrConKer;


/*************************
 * Stream Kernel-CPU
 ************************/
typedef struct strKerCpu {
	Char id;
	Char action;
	pcb pcb;
	Int8U quantum;
} __attribute__((packed)) StrKerCpu;


/*************************
 * Stream Kernel-UMC
************************/
typedef struct strKerUmc {
	Char id;
	Char action;
	Byte *data;
	Int32U size;
	Int32U pid;
	Int32U address;
} __attribute__((packed)) StrKerUmc;


/*************************
 * Stream Kernel-Consola
 ************************/
typedef struct strKerCon {
	Char id;
	Char action;
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
	Int32U address;
	Int32U pid;
	Int32U logLen;
	Byte* log;
} __attribute__((packed)) StrCpuKer;


/*************************
 * Stream CPU-UMC
 ************************/
typedef struct strCpuUmc {
	Char id;
	Char action;
	Int32U address;
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
	Int32U address;
	Byte* data;
	Int32U dataLen;
} __attribute__((packed)) StrUmcKer;


/*************************
 * Stream UCM-CPU
 ************************/
typedef struct strUmcCpu {
	Char id;
	Char action;
	Int32U address;
	Int32U dataLen;
	Byte* data;
} __attribute__((packed)) StrUmcCpu;


/*************************
 * Stream Umc-Swap (POR DEFINIR CON LOS MEMORY-BOYS)
 ************************
typedef struct strUmcSwa {
	Char id;
	Char action;
} __attribute__((packed)) StrUmcSwa;
*/

////////////////////////////////////////////////////////

/***********************************************/
/* Constructores
***********************************************/
StrConKer* newStrConKer(Char, Char, Byte*, Int32U);
		//Int32U); por si decidimos dejarlo
StrKerCpu* newStrKerCpu(Char, Char, pcb, Int8U);
StrKerUmc* newStrKerUmc(Char, Char, Byte*, Int32U, Int32U, Int32U);
StrKerCon* newStrKerCon(Char, Char , Int32U, Byte*);
StrCpuKer* newStrCpuKer(Char, Char, pcb, Int32U, Int32U, Int32U, Byte*);
StrCpuUmc* newStrCpuUmc(Char, Char, Int32U, Int32U, Byte*, Int32U);
StrUmcKer* newStrUmcKer(Char, Char, Int32U, Byte*, Int32U);
StrUmcCpu* newStrUmcCpu(Char, Char, Int32U, Int32U, Byte*);
//StrUmcSwa* newStrUmcSwa(Char, Char);

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

//Int32U getSizeUmcSwa(StrUmcSwa*);

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

//SocketBuffer* serializeUmcSwa(StrUmcSwa* umcSwa);

/***********************************************/

/***********************************************/
/* Unserialize
***********************************************/
SocketBuffer* unserializeConKer(Stream conKer);

SocketBuffer* unserializeKerCpu(Stream kerCpu);
SocketBuffer* unserializeKerUmc(Stream kerUmc);
SocketBuffer* unserializeKerCon(Stream kerCon);

SocketBuffer* unserializeCpuKer(Stream cpuKer);
SocketBuffer* unserializeCpuUmc(Stream cpuUmc);

SocketBuffer* unserializeUmcKer(Stream umcKer);
SocketBuffer* unserializeUmcCpu(Stream umcCpu);

//SocketBuffer* unserializeUmcSwa(Stream umcSwa);

/***********************************************/

/***********************************************/
/* Para Handshake
***********************************************/
Char getStreamId(Stream);

/***********************************************/
/* bitarrayToSocketBuffer
***********************************************/
SocketBuffer* bitarrayToSocketBuffer(t_bitarray*);

#endif /* LIBRERIASCOMPARTIDAS_POINTERSTREAM_H_ */
