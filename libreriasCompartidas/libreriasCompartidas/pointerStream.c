/*
 * pointerStream.c
 *
 *  Created on: 1/6/2016
 *      Author: utnso
 */

/*******************************************************
 * Dependencias
 *******************************************************/
#include "pointerStream.h"
#include "pointerSocketes.h"
#include "pointerTipos.h"
/*******************************************************/

//================================================================================================
/* Constructores
//==============================================================================================*/

/*******************************
 * Constructor Consola-Kernel
 ******************************/
StrConKer* newStrConKer(Char id, Char action, Byte* fileContent, Int32U fileContentLen, Int32U tid){
	StrConKer* sconk = malloc(sizeof(StrConKer));
	sconk->id = id;
	sconk->action = action;
	sconk->fileContent = fileContent;
	sconk->fileContentLen = fileContentLen;
	//sconk->tid = tid;
	return sconk;
}

/*******************************
 * Constructor Kernel-CPU
 ******************************/
StrKerCpu* newStrKerCpu(Char id, Char action, Tcb tcb, Int8U quantum){
	StrKerCpu* skc = malloc(sizeof(StrKerCpu));
	skc->id = id;
	skc->action = action;
	skc->tcb = tcb;
	skc->quantum = quantum;
	return skc;
}

/*******************************
 * Constructor Kernel-UMC
 ******************************/
StrKerUmc* newStrKerUmc(Char id, Char action, Byte *data, Int32U size, Int32U pid, Int32U address){
	StrKerUmc* sku = malloc(sizeof(StrKerUmc));
	sku->id = id;
	sku->action = action;
	sku->data = data;
	sku->size = size;
	sku->pid = pid;
	sku->address;
	return sku;
}

/*******************************
 * Constructor Kernel-Consola
 ******************************/
StrKerCon* newStrKerCon(Char id, Char action, Int32U tid){
	StrKerCon* skcon = malloc(sizeof(StrKerCon));
	skcon->id = id;
	skcon->action = action;
	skcon->tid = tid;
	return skcon;
}

/*******************************
 * Constructor CPU-Kernel
 ******************************/
StrCpuKer* newStrCpuKer(Char id, Char action, Tcb tcb, Int32U address, Int32U tid){
	StrCpuKer* sck = malloc(sizeof(StrCpuKer));
	sck->id = id;
	sck->action = action;
	sck->tcb = tcb;
	sck->address = address;
	sck->tid = tid;
	return sck;
}

/*******************************
 * Constructor CPU-UMC
 ******************************/
StrCpuUmc* newStrCpuUmc(Char id, Char action, Int32U address, Int32U dataLen, Byte* data, Int32U pid){
	StrCpuUmc* scu = malloc(sizeof(StrCpuUmc));
	scu->id = id;
	scu-> action = action;
	scu->address = address;
	scu->dataLen = dataLen;
	scu->data = data;
	scu->pid = pid;
	return scu;
}

/*******************************
 * Constructor UMC-Kernel
 ******************************/
StrUmcKer* newStrUmcKer(Char id, Char action, Int32U address, Byte* data, Int32U dataLen){
	StrUmcKer* suk = malloc(sizeof(StrUmcKer));
	suk->id = id;
	suk->action = action;
	suk->address = address;
	suk->data = data;
	suk->dataLen = dataLen;
	return suk;
}

/*******************************
 * Constructor UMC-CPU
 ******************************/
StrUmcCpu* newStrUmcCpu(Char id, Char action, Int32U address, Int32U dataLen, Byte* data){
	StrUmcCpu* suc = malloc(sizeof(StrUmcCpu));
	suc->id = id;
	suc->action = action;
	suc->address = address;
	suc->dataLen = dataLen;
	suc->data = data;
	return suc;
}

/*******************************
 * Constructor UMC-Swap
 ******************************/
//StrUmcSwa* newStrUmcSwa(Char, Char);

//================================================================================================



//================================================================================================
/* Sizes
//==============================================================================================*/

/*******************************
 * getSize Consola-Kernel
 ******************************/
/*Int32U getSizeConKer(StrConKer* sck){

}
*/
/*******************************
 * getSize Kernel-CPU
 ******************************/
/*Int32U getSizeKerCpu(StrKerCpu* skc){

}
*/
/*******************************
 * getSize Kernel-UMC
 ******************************/
/*Int32U getSizeKerUmc(StrKerUmc* sku){

}
*/
/*******************************
 * getSize Kernel-Consola
 ******************************/
/*Int32U getSizeKerCon(StrKerCon* skc){

}
*/
/*******************************
 * getSize CPU-Kernel
 ******************************/
/*Int32U getSizeCpuKer(StrCpuKer* sck){

}
*/
/*******************************
 * getSize CPU-UMC
 ******************************/
/*Int32U getSizeCpuUmc(StrCpuUmc* scu){

}
*/
/*******************************
 * getSize UMC-Kernel
 ******************************/
/*Int32U getSizeUmcKer(StrUmcKer* suk){

}
*/
/*******************************
 * getSize UMC-CPU
 ******************************/
/*Int32U getSizeUmcCpu(StrUmcCpu* suc){

}
*/
/*******************************
 * getSize UMC-Swap
 ******************************/
//Int32U getSizeUmcSwa(StrUmcSwa*);

//================================================================================================


//================================================================================================
/* Serialization
//================================================================================================

/*******************************
 * Serialization Consola-Kernel
 ******************************/
/*SocketBuffer* serializeConKer(StrConKer*){

}
*/

/***************************
 * Serialization Kernel-CPU
 **************************/
/*SocketBuffer* serializeKerCpu(StrKerCpu*){

}
*/

/***************************
 * Serialization Kernel-UMC
***************************/
/*SocketBuffer* serializeKerUmc(StrKerUmc*){

}
*/

/******************************
 * Serialization Kernel-Consola
 *****************************/
/*SocketBuffer* serializeKerCon(StrKerCon*){

}
*/

/***************************
 * Serialization CPU-Kernel
 **************************/
/*SocketBuffer* serializeCpuKer(StrCpuKer*){

}
*/

/*************************
 * Serialization CPU-UMC
 ************************/
/*SocketBuffer* serializeCpuUmc(StrCpuUmc*){

}
*/

/***************************
 * Serialization UMC-Kernel
 ***************************/
/*SocketBuffer* serializeUmcKer(StrUmcKer*){

}
*/

/*************************
 * Serialization UCM-CPU
 ************************/
/*SocketBuffer* serializeUmcCpu(StrUmcCpu*){

}
*/
//================================================================================================


//================================================================================================
/* Unserialization
//================================================================================================

/***********************************************/
/* Unserialize Consola -Kernel
***********************************************/
SocketBuffer* unserializeConKer(Stream conKer){

}
/***********************************************/
/* Unserialize Kernel-CPU
***********************************************/
SocketBuffer* unserializeKerCpu(Stream kerCpu){

}
/***********************************************/
/* Unserialize Kernel-UMC
***********************************************/
SocketBuffer* unserializeKerUmc(Stream kerUmc){

}
/***********************************************/
/* Unserialize Kernel-Consola
***********************************************/
SocketBuffer* unserializeKerCon(Stream kerCon){

}
/***********************************************/
/* Unserialize CPU-Kernel
***********************************************/
SocketBuffer* unserializeCpuKer(Stream cpuKer){

}
/***********************************************/
/* Unserialize CPU-UMC
***********************************************/
SocketBuffer* unserializeCpuUmc(Stream cpuUmc){

}
/***********************************************/
/* Unserialize UMC-Kernel
***********************************************/
SocketBuffer* unserializeUmcKer(Stream umcKer){

}
/***********************************************/
/* Unserialize UMC-CPU
***********************************************/
SocketBuffer* unserializeUmcCpu(Stream umcCpu){

}
/***********************************************/
/* Unserialize UMC-Swap
***********************************************/
//SocketBuffer* unserializeUmcSwa(Stream);


//================================================================================================


/***********************************************/
/* Para Handshake
************************************************/
Char getStreamId(Stream dataSerialized) {
	Char id;
	Stream ptrByte = dataSerialized;
	memcpy(&id, ptrByte, sizeof(id));
	return id;
}

/***********************************************/
/* bitarrayToSocketBuffer
************************************************/
SocketBuffer* bitarrayToSocketBuffer(t_bitarray* barray){
	SocketBuffer* sb = malloc(sizeof(SocketBuffer));
	Byte* ptrByte = (Byte*) barray->bitarray;

	Int32U i;
	for (i = 0; i < barray->size; i++) {
		sb->data[i] = *ptrByte;
		ptrByte++;
	}
	sb->size = barray->size;
	return sb;
}
