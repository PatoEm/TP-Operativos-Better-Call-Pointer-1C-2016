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
StrConKer* newStrConKer(Char id, Char action, Byte* fileContent, Int32U fileContentLen){
		//, Int32U tid){
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
StrKerCpu* newStrKerCpu(Char id, Char action, Int8U quantum){
		//Pcb Pcb, Int8U quantum){
	StrKerCpu* skc = malloc(sizeof(StrKerCpu));
	skc->id = id;
	skc->action = action;
	//skc->pcb = pcb;
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
	sku->address = address;
	return sku;
}

/*******************************
 * Constructor Kernel-Consola
 ******************************/
StrKerCon* newStrKerCon(Char id, Char action){
		//, Int32U tid){
	StrKerCon* skcon = malloc(sizeof(StrKerCon));
	skcon->id = id;
	skcon->action = action;
	//skcon->tid = tid;
	return skcon;
}

/*******************************
 * Constructor CPU-Kernel
 ******************************/
StrCpuKer* newStrCpuKer(Char id, Char action, Int32U address, Int32U tid) {
		//Tcb tcb, Int32U address, Int32U tid){
	StrCpuKer* sck = malloc(sizeof(StrCpuKer));
	sck->id = id;
	sck->action = action;
	//sck->tcb = tcb;
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
	//size += sizeof(skc->pcb);
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
	size += sizeof(sku->address);
	return size;
}

/*******************************
 * getSize Kernel-Consola
 ******************************/
Int32U getSizeKerCon(StrKerCon* skc){
	Int32U size = 0;
	size += sizeof(skc->id);
	size += sizeof(skc->action);
	return size;
}

/*******************************
 * getSize CPU-Kernel
 ******************************/
Int32U getSizeCpuKer(StrCpuKer* sck){
	Int32U size = 0;
	size += sizeof(sck->id);
	size += sizeof(sck->action);
	//size += sizeof(sck->pcb);
	size += sizeof(sck->address);
	size += sizeof(sck->tid);
	return size;
}

/*******************************
 * getSize CPU-UMC
 ******************************/
Int32U getSizeCpuUmc(StrCpuUmc* scu){
	Int32U size = 0;
	size += sizeof(scu->id);
	size += sizeof(scu->action);
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
	size += sizeof(suk->address);
	size += sizeof(suk->data);
	size += sizeof(suk->dataLen);
	return size;
}

/*******************************
 * getSize UMC-CPU
 ******************************/
Int32U getSizeUmcCpu(StrUmcCpu* suc){
	Int32U size = 0;
	size += sizeof(suc->id);
	size += sizeof(suc->action);
	size += sizeof(suc->address);
	size += sizeof(suc->dataLen);
	size += sizeof(suc->data);
	return size;
}

/*******************************
 * getSize UMC-Swap
 ******************************/
//Int32U getSizeUmcSwa(StrUmcSwa*);

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
/*
	ptrByte = (Byte*) &skc ->tcb;
	memcpy(ptrData,ptrByte, sizeof(skc->tcb));
	ptrData += sizeof(skc->tcb);
*/
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

	ptrByte = (Byte*) &sku->address;
	memcpy(ptrData, ptrByte, sizeof(sku->address));
	ptrData += sizeof(sku->address);

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
/*
	ptrByte = (Byte*) &sck->pcb;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb));
	ptrData += sizeof(sck->pcb);
*/
	ptrByte = (Byte*) &sck->address;
	memcpy(ptrData, ptrByte, sizeof(sck->address));
	ptrData += sizeof(sck->address);

	ptrByte = (Byte*) &sck->tid;
	memcpy(ptrData, ptrByte, sizeof(sck->tid));
	ptrData += sizeof(sck->tid);

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

	ptrByte = (Byte*) &scu->address;
	memcpy(ptrData, ptrByte, sizeof(scu->address));
	ptrData += sizeof(scu->address);

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
	Int8U size = getSizeCpuUmc(suk);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &suk->id;
	memcpy(ptrData, ptrByte, sizeof(suk->id));
	ptrData += sizeof(suk->id);

	ptrByte = (Byte*) &suk->action;
	memcpy(ptrData, ptrByte, sizeof(suk->action));
	ptrData += sizeof(suk->action);

	ptrByte = (Byte*) &suk->address;
	memcpy(ptrData, ptrByte, sizeof(suk->address));
	ptrData += sizeof(suk->address);

	ptrByte = (Byte*) &suk->data;
	memcpy(ptrData, ptrByte, sizeof(suk->data));
	ptrData += sizeof(suk->data);

	ptrByte = (Byte*) &suk->dataLen;
	memcpy(ptrData, ptrByte, sizeof(suk->dataLen));
	ptrData += sizeof(suk->dataLen);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}


/*************************
 * Serialization UMC-CPU
 ************************/
SocketBuffer* serializeUmcCpu(StrUmcCpu* suc){
	Int8U size = getSizeCpuUmc(suc);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &suc->id;
	memcpy(ptrData, ptrByte, sizeof(suc->id));
	ptrData += sizeof(suc->id);

	ptrByte = (Byte*) &suc->action;
	memcpy(ptrData, ptrByte, sizeof(suc->action));
	ptrData += sizeof(suc->action);

	ptrByte = (Byte*) &suc->address;
	memcpy(ptrData, ptrByte, sizeof(suc->address));
	ptrData += sizeof(suc->address);

	ptrByte = (Byte*) &suc->dataLen;
	memcpy(ptrData, ptrByte, sizeof(suc->dataLen));
	ptrData += sizeof(suc->dataLen);

	ptrByte = (Byte*) &suc->data;
	memcpy(ptrData, ptrByte, sizeof(suc->data));
	ptrData += sizeof(suc->data);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

/*************************
 * Serialization UMC-SWAP
 ************************/
//SocketBuffer* serializeUmcSwa(StrUmcSwa*){

	//t_bitarray* barray = bitarray_create((char*) data, size);
	//return bitarrayToSocketBuffer(barray);

//}

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
	//Int32U tid;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&fileContent, ptrByte, sizeof(fileContent));
	ptrByte += sizeof(fileContent);
	memcpy(&fileContentLen, ptrByte, sizeof(fileContentLen));
	ptrByte += sizeof(fileContentLen);
/*
	memcpy(&tid, ptrByte, sizeof(tid));
	ptrByte += sizeof(tid);
*/
	free(dataSerialized);
	return newStrConKer(id, action, fileContent, fileContentLen);
}
/***********************************************/
/* Unserialize Kernel-CPU
***********************************************/
SocketBuffer* unserializeKerCpu(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	//pcb pcb;
	Int8U quantum;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
/*
	memcpy(&pcb, ptrByte, sizeof(pcb));
	ptrByte += sizeof(pcb);
*/
	memcpy(&quantum, ptrByte, sizeof(quantum));
	ptrByte += sizeof(quantum);

	free(dataSerialized);
	return newStrKerCpu(id, action, quantum);
}
/***********************************************/
/* Unserialize Kernel-UMC
***********************************************/
SocketBuffer* unserializeKerUmc(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	Byte *data;
	Int32U size;
	Int32U pid;
	Int32U address;

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
	memcpy(&address, ptrByte, sizeof(address));
	ptrByte += sizeof(address);

	free(dataSerialized);
	return newStrKerUmc(id, action, data, size, pid, address);

}
/***********************************************/
/* Unserialize Kernel-Consola
***********************************************/
SocketBuffer* unserializeKerCon(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	//Int32U tid;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

	free(dataSerialized);
	return newStrKerCon(id, action);
}
/***********************************************/
/* Unserialize CPU-Kernel
***********************************************/
SocketBuffer* unserializeCpuKer(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	//Pcb pcb;
	Int32U address;
	Int32U tid;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
/*
	memcpy(&pcb, ptrByte, sizeof(pcb));
	ptrByte += sizeof(pcb);
*/
	memcpy(&address, ptrByte, sizeof(address));
	ptrByte += sizeof(address);
	memcpy(&tid, ptrByte, sizeof(tid));
	ptrByte += sizeof(tid);

	free(dataSerialized);
	return newStrCpuKer(id, action, address, tid);
}
/***********************************************/
/* Unserialize CPU-UMC
***********************************************/
SocketBuffer* unserializeCpuUmc(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	Int32U address;
	Int32U dataLen;
	Byte* data;
	Int32U pid;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&address, ptrByte, sizeof(address));
	ptrByte += sizeof(address);
	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);
	memcpy(&data, ptrByte, sizeof(data));
	ptrByte += sizeof(data);
	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);

	free(dataSerialized);
	return newStrCpuUmc(id, action, address, dataLen, data, pid);
}
/***********************************************/
/* Unserialize UMC-Kernel
***********************************************/
SocketBuffer* unserializeUmcKer(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	Int32U address;
	Byte* data;
	Int32U dataLen;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&address, ptrByte, sizeof(address));
	ptrByte += sizeof(address);
	memcpy(&data, ptrByte, sizeof(data));
	ptrByte += sizeof(data);
	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);

	free(dataSerialized);
	return newStrUmcKer(id, action, address, data, dataLen);
}
/***********************************************/
/* Unserialize UMC-CPU
***********************************************/
SocketBuffer* unserializeUmcCpu(Stream dataSerialized){
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	Int32U address;
	Int32U dataLen;
	Byte* data;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);
	memcpy(&address, ptrByte, sizeof(address));
	ptrByte += sizeof(address);
	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);
	memcpy(&data, ptrByte, sizeof(data));
	ptrByte += sizeof(data);

	free(dataSerialized);
	return newStrUmcCpu(id, action, address, dataLen, data);
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
	Stream ptrByte = dataSerialized;
	Char id;
	memcpy(&id, ptrByte, sizeof(id));
	free(dataSerialized);
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
