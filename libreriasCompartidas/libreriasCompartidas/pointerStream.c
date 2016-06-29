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
#include "pcb.h"
/*******************************************************/

//================================================================================================
/* Constructores
//==============================================================================================*/

/*******************************
 * Constructor Consola-Kernel
 ******************************/
StrConKer* newStrConKer(Char id, Char action, Byte* fileContent, Int32U fileContentLen){
	StrConKer* sconk = malloc(sizeof(StrConKer));
	sconk->id = id;
	sconk->action = action;
	sconk->fileContent = fileContent;
	sconk->fileContentLen = fileContentLen;
	return sconk;
}

/*******************************
 * Constructor Kernel-CPU
 ******************************/
StrKerCpu* newStrKerCpu(Char id, Char action, pcb pcb, Int8U quantum){
	StrKerCpu* skc = malloc(sizeof(StrKerCpu));
	skc->id = id;
	skc->action = action;
	skc->pcb = pcb;
	skc->quantum = quantum;
	return skc;
}

/*******************************
 * Constructor Kernel-UMC
 ******************************/
StrKerUmc* newStrKerUmc(Char id, Char action, Byte* data, Int32U size, Int32U pid, Int32U cantPage){
	StrKerUmc* sku = malloc(sizeof(StrKerUmc));
	sku->id = id;
	sku->action = action;
	sku->data = data;
	sku->size = size;
	sku->pid = pid;
	sku->cantPage = cantPage;
	return sku;
}

/*******************************
 * Constructor Kernel-Consola
 ******************************/
StrKerCon* newStrKerCon(Char id, Char action, Int32U logLen, Byte* log){
	StrKerCon* skcon = malloc(sizeof(StrKerCon));
	skcon->id = id;
	skcon->action = action;
	skcon->logLen = logLen;
	skcon->log = log;
	return skcon;
}

/*******************************
 * Constructor CPU-Kernel
 ******************************/
StrCpuKer* newStrCpuKer(Char id, Char action, pcb pcb, Int32U pid, Int32U logLen, Byte* log) {
	StrCpuKer* sck = malloc(sizeof(StrCpuKer));
	sck->id = id;
	sck->action = action;
	sck->pcb = pcb;
	sck->pid = pid;
	sck->logLen = logLen;
	sck->log = log;
	return sck;
}

/*******************************
 * Constructor CPU-UMC
 ******************************/
StrCpuUmc* newStrCpuUmc(Char id, Char action, espacioAsignado pageComienzo, Int32U offset, Int32U dataLen, Byte* data, Int32U pid){
	StrCpuUmc* scu = malloc(sizeof(StrCpuUmc));
	scu->id = id;
	scu->action = action;
	scu->pageComienzo = pageComienzo;
	scu->offset = offset;
	scu->dataLen = dataLen;
	scu->data = data;
	scu->pid = pid;
	return scu;
}

/*******************************
 * Constructor UMC-Kernel
 ******************************/
StrUmcKer* newStrUmcKer(Char id, Char action, Byte* data, Int32U size, Int32U pid, Int32U cantPage){
	StrUmcKer* suk = malloc(sizeof(StrUmcKer));
	suk->id = id;
	suk->action = action;
	suk->data = data;
	suk->size = size;
	suk->pid = pid;
	suk->cantPage = cantPage;
	return suk;
}

/*******************************
 * Constructor UMC-CPU
 ******************************/

StrUmcCpu* newStrUmcCpu(Char id, Char action, espacioAsignado pageComienzo, Int32U offset, Int32U dataLen, Byte* data, Int32U pid){
	StrUmcCpu* suc = malloc(sizeof(StrUmcCpu));
	suc->id = id;
	suc->action = action;
	suc->pageComienzo = pageComienzo;
	suc->offset = offset;
	suc->dataLen = dataLen;
	suc->data = data;
	suc->pid = pid;
	return suc;
}

/*******************************
 * Constructor UMC-Swap
 ******************************/
StrUmcSwa* newStrUmcSwa(Char id, Char action, espacioAsignado pageComienzo, Int32U cantPage, Byte* data, Int32U dataLen, Int32U pid){
	StrUmcSwa* sus = malloc(sizeof(StrUmcSwa));
	sus->id = id;
	sus->action = action;
	sus->pageComienzo = pageComienzo;
	sus->cantPage = cantPage;
	sus->data = data;
	sus->dataLen = dataLen;
	sus->pid = pid;
	return sus;
}

/*******************************
 * Constructor Swap-UMC
 ******************************/
StrSwaUmc* newStrSwaUmc(Char id, Char action, paginaAsignada pageComienzo, Int32U cantPage, Byte* data, Int32U dataLen, Int32U pid){
	StrSwaUmc* ssu = malloc(sizeof(StrUmcSwa));
	ssu->id = id;
	ssu->action = action;
	ssu->pageComienzo = pageComienzo;
	ssu->cantPage = cantPage;
	ssu->data = data;
	ssu->dataLen = dataLen;
	ssu->pid = pid;
	return ssu;
}

//================================================================================================



//================================================================================================
/* Sizes
//==============================================================================================*/

/*******************************
 * getSize Consola-Kernel
 ******************************/
Int32U getSizeConKer(StrConKer* sck){
	Int32U size = 0;
	size += sizeof(sck->id);
	size += sizeof(sck->action);
	size += sizeof(sck->fileContent);
	size += sizeof(sck->fileContentLen);
	return size;
}

/*******************************
 * getSize Kernel-CPU
 ******************************/
Int32U getSizeKerCpu(StrKerCpu* skc){
	Int32U size = 0;
	size += sizeof(skc->id);
	size += sizeof(skc->action);
	size += sizeof(skc->pcb);
	size += sizeof(skc->quantum);
	return size;
}

/*******************************
 * getSize Kernel-UMC
 ******************************/
Int32U getSizeKerUmc(StrKerUmc* sku){
	Int32U size = 0;
	size += sizeof(sku->id);
	size += sizeof(sku->action);
	size += sizeof(sku->data);
	size += sizeof(sku->size);
	size += sizeof(sku->pid);
	size += sizeof(sku->cantPage);
	return size;
}

/*******************************
 * getSize Kernel-Consola
 ******************************/
Int32U getSizeKerCon(StrKerCon* skc){
	Int32U size = 0;
	size += sizeof(skc->id);
	size += sizeof(skc->action);
	size += sizeof(skc->logLen);
	size += sizeof(skc->log);
	return size;
}

/*******************************
 * getSize CPU-Kernel
 ******************************/
Int32U getSizeCpuKer(StrCpuKer* sck){
	Int32U size = 0;
	size += sizeof(sck->id);
	size += sizeof(sck->action);
	size += sizeof(sck->pcb);
	size += sizeof(sck->pid);
	size += sizeof(sck->logLen);
	size += sizeof(sck->log);
	return size;
}

/*******************************
 * getSize CPU-UMC
 ******************************/
Int32U getSizeCpuUmc(StrCpuUmc* scu){
	Int32U size = 0;
	size += sizeof(scu->id);
	size += sizeof(scu->action);
	size += sizeof(scu->pageComienzo);
	size += sizeof(scu->offset);
	size += sizeof(scu->dataLen);
	size += sizeof(scu->data);
	size += sizeof(scu->pid);
	return size;
}

/*******************************
 * getSize UMC-Kernel
 ******************************/
Int32U getSizeUmcKer(StrUmcKer* suk){
	Int32U size = 0;
	size += sizeof(suk->id);
	size += sizeof(suk->action);
	size += sizeof(suk->data);
	size += sizeof(suk->size);
	size += sizeof(suk->pid);
	size += sizeof(suk->cantPage);
	return size;
}

/*******************************
 * getSize UMC-CPU
 ******************************/
Int32U getSizeUmcCpu(StrUmcCpu* suc){
	Int32U size = 0;
	size += sizeof(suc->id);
	size += sizeof(suc->action);
	size += sizeof(suc->pageComienzo);
	size += sizeof(suc->offset);
	size += sizeof(suc->dataLen);
	size += sizeof(suc->data);
	size += sizeof(suc->pid);
	return size;
}

/*******************************
 * getSize UMC-Swap
 ******************************/
Int32U getSizeUmcSwa(StrUmcSwa* sus){
	Int32U size = 0;
	size += sizeof(sus->id);
	size += sizeof(sus->action);
	size += sizeof(sus->pageComienzo);
	size += sizeof(sus->cantPage);
	size += sizeof(sus->data);
	size += sizeof(sus->dataLen);
	size += sizeof(sus->pid);
	return size;
}

/*******************************
 * getSize Swap-UMC
 ******************************/
Int32U getSizeSwaUmc(StrSwaUmc* ssu){
	Int32U size = 0;
	size += sizeof(ssu->id);
	size += sizeof(ssu->action);
	size += sizeof(ssu->pageComienzo);
	size += sizeof(ssu->cantPage);
	size += sizeof(ssu->data);
	size += sizeof(ssu->dataLen);
	size += sizeof(ssu->pid);
	return size;
}

//================================================================================================


//================================================================================================
/* Serialization
//================================================================================================

/ ******************************
* Serialization Consola-Kernel
* *****************************/
SocketBuffer* serializeConKer(StrConKer* sconk){
	Int8U size = getSizeConKer(sconk);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &sconk->id;
	memcpy(ptrData, ptrByte, sizeof(sconk->id));
	ptrData += sizeof(sconk->id);

	ptrByte = (Byte*) &sconk->action;
	memcpy(ptrData, ptrByte, sizeof(sconk->action));
	ptrData += sizeof(sconk->action);

	ptrByte = (Byte*) &sconk->fileContent;
	memcpy(ptrData, ptrByte, sizeof(sconk->fileContent));
	ptrData += sizeof(sconk->fileContent);

	ptrByte = (Byte*) &sconk->fileContentLen;
	memcpy(ptrData, ptrByte, sizeof(sconk->fileContentLen));
	ptrData += sizeof(sconk->fileContentLen);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}


/***************************
 * Serialization Kernel-CPU
 **************************/
SocketBuffer* serializeKerCpu(StrKerCpu* skc){
	Int8U size = getSizeKerCpu(skc);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &skc->id;
	memcpy(ptrData, ptrByte, sizeof(skc->id));
	ptrData += sizeof(skc->id);

	ptrByte = (Byte*) &skc->action;
	memcpy(ptrData, ptrByte, sizeof(skc->action));
	ptrData += sizeof(skc->action);

	ptrByte = (Byte*) &skc ->pcb;
	memcpy(ptrData,ptrByte, sizeof(skc->pcb));
	ptrData += sizeof(skc->pcb);

	ptrByte = (Byte*) &skc->quantum;
	memcpy(ptrData, ptrByte, sizeof(skc->quantum));
	ptrData += sizeof(skc->quantum);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}


/***************************
 * Serialization Kernel-UMC
***************************/
SocketBuffer* serializeKerUmc(StrKerUmc* sku){
	Int32U size = getSizeKerUmc(sku);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &sku->id;
	memcpy(ptrData, ptrByte, sizeof(sku->id));
	ptrData += sizeof(sku->id);

	ptrByte = (Byte*) &sku->action;
	memcpy(ptrData, ptrByte, sizeof(sku->action));
	ptrData += sizeof(sku->action);

	ptrByte = (Byte*) &sku->data;
	memcpy(ptrData, ptrByte, sizeof(sku->data));
	ptrData += sizeof(sku->data);

	ptrByte = (Byte*) &sku->size;
	memcpy(ptrData, ptrByte, sizeof(size));
	ptrData += sizeof(sku->size);

	ptrByte = (Byte*) &sku->pid;
	memcpy(ptrData, ptrByte, sizeof(sku->pid));
	ptrData += sizeof(sku->pid);

	ptrByte = (Byte*) &sku->cantPage;
	memcpy(ptrData, ptrByte, sizeof(sku->cantPage));
	ptrData += sizeof(sku->cantPage);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}


/******************************
 * Serialization Kernel-Consola
 *****************************/
SocketBuffer* serializeKerCon(StrKerCon* skcon){
	Int8U size = getSizeKerCon(skcon);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &skcon->id;
	memcpy(ptrData, ptrByte, sizeof(skcon->id));
	ptrData += sizeof(skcon->id);

	ptrByte = (Byte*) &skcon->action;
	memcpy(ptrData, ptrByte, sizeof(skcon->action));
	ptrData += sizeof(skcon->action);

	ptrByte = (Byte*) &skcon->logLen;
	memcpy(ptrData, ptrByte, sizeof(skcon->logLen));
	ptrData += sizeof(skcon->logLen);

	ptrByte = (Byte*) &skcon->log;
	memcpy(ptrData, ptrByte, sizeof(skcon->log));
	ptrData += sizeof(skcon->log);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}


/***************************
 * Serialization CPU-Kernel
 **************************/
SocketBuffer* serializeCpuKer(StrCpuKer* sck){
	Int8U size = getSizeCpuKer(sck);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &sck->id;
	memcpy(ptrData, ptrByte, sizeof(sck->id));
	ptrData += sizeof(sck->id);

	ptrByte = (Byte*) &sck->action;
	memcpy(ptrData, ptrByte, sizeof(sck->action));
	ptrData += sizeof(sck->action);

	ptrByte = (Byte*) &sck->pcb;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb));
	ptrData += sizeof(sck->pcb);

	ptrByte = (Byte*) &sck->pid;
	memcpy(ptrData, ptrByte, sizeof(sck->pid));
	ptrData += sizeof(sck->pid);

	ptrByte = (Byte*) &sck->logLen;
	memcpy(ptrData, ptrByte, sizeof(sck->logLen));
	ptrData += sizeof(sck->logLen);

	ptrByte = (Byte*) &sck->log;
	memcpy(ptrData, ptrByte, sizeof(sck->log));
	ptrData += sizeof(sck->log);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}


/*************************
 * Serialization CPU-UMC
 ************************/
SocketBuffer* serializeCpuUmc(StrCpuUmc* scu){
	Int8U size = getSizeCpuUmc(scu);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &scu->id;
	memcpy(ptrData, ptrByte, sizeof(scu->id));
	ptrData += sizeof(scu->id);

	ptrByte = (Byte*) &scu->action;
	memcpy(ptrData, ptrByte, sizeof(scu->action));
	ptrData += sizeof(scu->action);

	ptrByte = (Byte*) &scu->pageComienzo;
	memcpy(ptrData, ptrByte, sizeof(scu->pageComienzo));
	ptrData += sizeof(scu->pageComienzo);

	ptrByte = (Byte*) &scu->offset;
	memcpy(ptrData, ptrByte, sizeof(scu->offset));
	ptrData += sizeof(scu->offset);

	ptrByte = (Byte*) &scu->dataLen;
	memcpy(ptrData, ptrByte, sizeof(scu->dataLen));
	ptrData += sizeof(scu->dataLen);

	ptrByte = (Byte*) &scu->data;
	memcpy(ptrData, ptrByte, sizeof(scu->data));
	ptrData += sizeof(scu->data);

	ptrByte = (Byte*) &scu->pid;
	memcpy(ptrData, ptrByte, sizeof(scu->pid));
	ptrData += sizeof(scu->pid);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}


/***************************
 * Serialization UMC-Kernel
 ***************************/
SocketBuffer* serializeUmcKer(StrUmcKer* suk){
	Int8U size = getSizeUmcKer(suk);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &suk->id;
	memcpy(ptrData, ptrByte, sizeof(suk->id));
	ptrData += sizeof(suk->id);

	ptrByte = (Byte*) &suk->action;
	memcpy(ptrData, ptrByte, sizeof(suk->action));
	ptrData += sizeof(suk->action);

	ptrByte = (Byte*) &suk->data;
	memcpy(ptrData, ptrByte, sizeof(suk->data));
	ptrData += sizeof(suk->data);

	ptrByte = (Byte*) &suk->size;
	memcpy(ptrData, ptrByte, sizeof(suk->size));
	ptrData += sizeof(suk->size);

	ptrByte = (Byte*) &suk->pid;
	memcpy(ptrData, ptrByte, sizeof(suk->pid));
	ptrData += sizeof(suk->pid);

	ptrByte = (Byte*) &suk->cantPage;
	memcpy(ptrData, ptrByte, sizeof(suk->cantPage));
	ptrData += sizeof(suk->cantPage);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}


/*************************
 * Serialization UMC-CPU
 ************************/
SocketBuffer* serializeUmcCpu(StrUmcCpu* suc){
	Int8U size = getSizeUmcCpu(suc);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &suc->id;
	memcpy(ptrData, ptrByte, sizeof(suc->id));
	ptrData += sizeof(suc->id);

	ptrByte = (Byte*) &suc->action;
	memcpy(ptrData, ptrByte, sizeof(suc->action));
	ptrData += sizeof(suc->action);

	ptrByte = (Byte*) &suc->pageComienzo;
	memcpy(ptrData, ptrByte, sizeof(suc->pageComienzo));
	ptrData += sizeof(suc->pageComienzo);

	ptrByte = (Byte*) &suc->offset;
	memcpy(ptrData, ptrByte, sizeof(suc->offset));
	ptrData += sizeof(suc->offset);

	ptrByte = (Byte*) &suc->dataLen;
	memcpy(ptrData, ptrByte, sizeof(suc->dataLen));
	ptrData += sizeof(suc->dataLen);

	ptrByte = (Byte*) &suc->data;
	memcpy(ptrData, ptrByte, sizeof(suc->data));
	ptrData += sizeof(suc->data);

	ptrByte = (Byte*) &suc->pid;
	memcpy(ptrData, ptrByte, sizeof(suc->pid));
	ptrData += sizeof(suc->pid);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

/*************************
 * Serialization UMC-SWAP
 ************************/
SocketBuffer* serializeUmcSwa(StrUmcSwa* sus){
	Int8U size = getSizeUmcSwa(sus);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &sus->id;
	memcpy(ptrData, ptrByte, sizeof(sus->id));
	ptrData += sizeof(sus->id);

	ptrByte = (Byte*) &sus->action;
	memcpy(ptrData, ptrByte, sizeof(sus->action));
	ptrData += sizeof(sus->action);

	ptrByte = (Byte*) &sus->pageComienzo;
	memcpy(ptrData, ptrByte, sizeof(sus->pageComienzo));
	ptrData += sizeof(sus->pageComienzo);

	ptrByte = (Byte*) &sus->cantPage;
	memcpy(ptrData, ptrByte, sizeof(sus->cantPage));
	ptrData += sizeof(sus->cantPage);

	ptrByte = (Byte*) &sus->data;
	memcpy(ptrData, ptrByte, sizeof(sus->data));
	ptrData += sizeof(sus->data);

	ptrByte = (Byte*) &sus->dataLen;
	memcpy(ptrData, ptrByte, sizeof(sus->dataLen));
	ptrData += sizeof(sus->dataLen);

	ptrByte = (Byte*) &sus->pid;
	memcpy(ptrData, ptrByte, sizeof(sus->pid));
	ptrData += sizeof(sus->pid);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

/*************************
 * Serialization SWAP-UMC
 ************************/
SocketBuffer* serializeSwaUmc(StrSwaUmc* ssu){
	Int8U size = getSizeSwaUmc(ssu);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &ssu->id;
	memcpy(ptrData, ptrByte, sizeof(ssu->id));
	ptrData += sizeof(ssu->id);

	ptrByte = (Byte*) &ssu->action;
	memcpy(ptrData, ptrByte, sizeof(ssu->action));
	ptrData += sizeof(ssu->action);

	ptrByte = (Byte*) &ssu->pageComienzo;
	memcpy(ptrData, ptrByte, sizeof(ssu->pageComienzo));
	ptrData += sizeof(ssu->pageComienzo);

	ptrByte = (Byte*) &ssu->cantPage;
	memcpy(ptrData, ptrByte, sizeof(ssu->cantPage));
	ptrData += sizeof(ssu->cantPage);

	ptrByte = (Byte*) &ssu->data;
	memcpy(ptrData, ptrByte, sizeof(ssu->data));
	ptrData += sizeof(ssu->data);

	ptrByte = (Byte*) &ssu->dataLen;
	memcpy(ptrData, ptrByte, sizeof(ssu->dataLen));
	ptrData += sizeof(ssu->dataLen);

	ptrByte = (Byte*) &ssu->pid;
	memcpy(ptrData, ptrByte, sizeof(ssu->pid));
	ptrData += sizeof(ssu->pid);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

//================================================================================================


//================================================================================================
/* Unserialization
//================================================================================================

/ ***********************************************
* Unserialize Consola -Kernel
* **********************************************/
SocketBuffer* unserializeConKer(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	Byte* fileContent = NULL;
	Int32U fileContentLen;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&fileContent, ptrByte, sizeof(fileContent));
	ptrByte += sizeof(fileContent);
	memcpy(&fileContentLen, ptrByte, sizeof(fileContentLen));
	ptrByte += sizeof(fileContentLen);

	//free(dataSerialized);
	return newStrConKer(id, action, fileContent, fileContentLen);
}

/***********************************************/
/* Unserialize Kernel-CPU
***********************************************/
SocketBuffer* unserializeKerCpu(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	pcb pcb;
	Int8U quantum;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&pcb, ptrByte, sizeof(pcb));
	ptrByte += sizeof(pcb);
	memcpy(&quantum, ptrByte, sizeof(quantum));
	ptrByte += sizeof(quantum);

	free(dataSerialized);
	return newStrKerCpu(id, action, pcb, quantum);
}

/***********************************************/
/* Unserialize Kernel-UMC
***********************************************/
SocketBuffer* unserializeKerUmc(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	Byte* data = NULL;
	Int32U size;
	Int32U pid;
	Int32U cantPage;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&data, ptrByte, sizeof(data));
	ptrByte += sizeof(data);
	memcpy(&size, ptrByte, sizeof(size));
	ptrByte += sizeof(size);
	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);
	memcpy(&cantPage, ptrByte, sizeof(cantPage));
	ptrByte += sizeof(cantPage);

	free(dataSerialized);
	return newStrKerUmc(id, action, data, size, pid, cantPage);
}

/***********************************************/
/* Unserialize Kernel-Consola
***********************************************/
SocketBuffer* unserializeKerCon(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	Int32U logLen;
	Byte* log = NULL;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&logLen, ptrByte, sizeof(logLen));
	ptrByte += sizeof(logLen);
	memcpy(&log, ptrByte, sizeof(log));
	ptrByte += sizeof(log);

	free(dataSerialized);
	return newStrKerCon(id, action, logLen, log);
}

/***********************************************/
/* Unserialize CPU-Kernel
***********************************************/
SocketBuffer* unserializeCpuKer(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	pcb pcb;
	Int32U pid;
	Int32U logLen;
	Byte* log = NULL;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&pcb, ptrByte, sizeof(pcb));
	ptrByte += sizeof(pcb);
	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);
	memcpy(&logLen, ptrByte, sizeof(logLen));
	ptrByte += sizeof(logLen);
	memcpy(&log, ptrByte, sizeof(log));
	ptrByte += sizeof(log);

	free(dataSerialized);
	return newStrCpuKer(id, action, pcb, pid, logLen, log);
}

/***********************************************/
/* Unserialize CPU-UMC
***********************************************/
SocketBuffer* unserializeCpuUmc(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	espacioAsignado pageComienzo;
	Int32U offset;
	Int32U dataLen;
	Byte* data = NULL;
	Int32U pid;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&pageComienzo, ptrByte, sizeof(pageComienzo));
	ptrByte += sizeof(pageComienzo);
	memcpy(&offset, ptrByte, sizeof(offset));
	ptrByte += sizeof(offset);
	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);
	memcpy(&data, ptrByte, sizeof(data));
	ptrByte += sizeof(data);
	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);

	free(dataSerialized);
	return newStrCpuUmc(id, action, pageComienzo, offset, dataLen, data, pid);
}

/***********************************************/
/* Unserialize UMC-Kernel
***********************************************/
SocketBuffer* unserializeUmcKer(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	Byte* data = NULL;
	Int32U size;
	Int32U pid;
	Int32U cantPage;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&data, ptrByte, sizeof(data));
	ptrByte += sizeof(data);
	memcpy(&size, ptrByte, sizeof(size));
	ptrByte += sizeof(size);
	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);
	memcpy(&cantPage, ptrByte, sizeof(cantPage));
	ptrByte += sizeof(cantPage);

	free(dataSerialized);
	return newStrUmcKer(id, action, data, size, pid, cantPage);
}

/***********************************************/
/* Unserialize UMC-CPU
***********************************************/
SocketBuffer* unserializeUmcCpu(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	espacioAsignado pageComienzo;
	Int32U offset;
	Int32U dataLen;
	Byte* data = NULL;
	Int32U pid;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&pageComienzo, ptrByte, sizeof(pageComienzo));
	ptrByte += sizeof(pageComienzo);
	memcpy(&offset, ptrByte, sizeof(offset));
	ptrByte += sizeof(offset);
	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);
	memcpy(&data, ptrByte, sizeof(data));
	ptrByte += sizeof(data);
	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);

	free(dataSerialized);
	return newStrUmcCpu(id, action, pageComienzo, offset, dataLen, data, pid);
}

/***********************************************/
/* Unserialize UMC-Swap
***********************************************/
SocketBuffer* unserializeUmcSwa(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	espacioAsignado pageComienzo;
	Int32U cantPage;
	Byte* data = NULL;
	Int32U dataLen;
	Int32U pid;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&pageComienzo, ptrByte, sizeof(pageComienzo));
	ptrByte += sizeof(pageComienzo);
	memcpy(&cantPage, ptrByte, sizeof(cantPage));
	ptrByte += sizeof(cantPage);
	memcpy(&data, ptrByte, sizeof(data));
	ptrByte += sizeof(data);
	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);
	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);

	free(dataSerialized);
	return newStrUmcSwa(id, action, pageComienzo, cantPage, data, dataLen, pid);
}

/***********************************************/
/* Unserialize Swap-UMC
***********************************************/
SocketBuffer* unserializeSwaUmc(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	paginaAsignada pageComienzo;
	Int32U cantPage;
	Byte* data = NULL;
	Int32U dataLen;
	Int32U pid;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&pageComienzo, ptrByte, sizeof(pageComienzo));
	ptrByte += sizeof(pageComienzo);
	memcpy(&cantPage, ptrByte, sizeof(cantPage));
	ptrByte += sizeof(cantPage);
	memcpy(&data, ptrByte, sizeof(data));
	ptrByte += sizeof(data);
	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);
	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);

	free(dataSerialized);
	return newStrSwaUmc(id, action, pageComienzo, cantPage, data, dataLen, pid);
}
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
