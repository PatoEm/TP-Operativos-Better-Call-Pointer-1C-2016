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
StrConKer* newStrConKer(Char, Char, Byte*, Int32U, Int32U){

}

/*******************************
 * Constructor Kernel-CPU
 ******************************/
StrKerCpu* newStrKerCpu(Char, Char, Tcb, Int8U){

}

/*******************************
 * Constructor Kernel-UMC
 ******************************/
StrKerUmc* newStrKerUmc(Char, Char, Byte*, Int32U, Int32U, Int32U){

}

/*******************************
 * Constructor Kernel-Consola
 ******************************/
StrKerCon* newStrKerCon(Char, Char, Int32U){

}

/*******************************
 * Constructor CPU-Kernel
 ******************************/
StrCpuKer* newStrCpuKer(Char, Char, Tcb, Int32U, Int32U){

}

/*******************************
 * Constructor CPU-UMC
 ******************************/
StrCpuUmc* newStrCpuUmc(Char, Char, Int32U, Int32U, Byte*, Int32U){

}

/*******************************
 * Constructor UMC-Kernel
 ******************************/
StrUmcKer* newStrUmcKer(Char, Char, Int32U, Byte*, Int32U){

}

/*******************************
 * Constructor UMC-CPU
 ******************************/
StrUmcCpu* newStrUmcCpu(Char, Char, Int32U, Int32U, Byte*){

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

}

/*******************************
 * getSize Kernel-CPU
 ******************************/
Int32U getSizeKerCpu(StrKerCpu* skc){

}

/*******************************
 * getSize Kernel-UMC
 ******************************/
Int32U getSizeKerUmc(StrKerUmc* sku){

}

/*******************************
 * getSize Kernel-Consola
 ******************************/
Int32U getSizeKerCon(StrKerCon* skc){

}

/*******************************
 * getSize CPU-Kernel
 ******************************/
Int32U getSizeCpuKer(StrCpuKer* sck){

}

/*******************************
 * getSize CPU-UMC
 ******************************/
Int32U getSizeCpuUmc(StrCpuUmc* scu){

}

/*******************************
 * getSize UMC-Kernel
 ******************************/
Int32U getSizeUmcKer(StrUmcKer* suk){

}

/*******************************
 * getSize UMC-CPU
 ******************************/
Int32U getSizeUmcCpu(StrUmcCpu* suc){

}

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
SocketBuffer* serializeConKer(StrConKer*){

}


/***************************
 * Serialization Kernel-CPU
 **************************/
SocketBuffer* serializeKerCpu(StrKerCpu*){

}


/***************************
 * Serialization Kernel-UMC
***************************/
SocketBuffer* serializeKerUmc(StrKerUmc*){

}


/******************************
 * Serialization Kernel-Consola
 *****************************/
SocketBuffer* serializeKerCon(StrKerCon*){

}


/***************************
 * Serialization CPU-Kernel
 **************************/
SocketBuffer* serializeCpuKer(StrCpuKer*){

}


/*************************
 * Serialization CPU-UMC
 ************************/
SocketBuffer* serializeCpuUmc(StrCpuUmc*){

}


/***************************
 * Serialization UMC-Kernel
 ***************************/
SocketBuffer* serializeUmcKer(StrUmcKer*){

}


/*************************
 * Serialization UCM-CPU
 ************************/
SocketBuffer* serializeUmcCpu(StrUmcCpu*){

}

//================================================================================================


//================================================================================================
/* Unserialization
//================================================================================================

/***********************************************/
/* Unserialize Consola -Kernel
***********************************************/
SocketBuffer* unserializeConKer(Stream){

}

/***********************************************/
/* Unserialize Kernel-CPU
***********************************************/
SocketBuffer* unserializeKerCpu(Stream){

}

/***********************************************/
/* Unserialize Kernel-UMC
***********************************************/
SocketBuffer* unserializeKerUmc(Stream){

}

/***********************************************/
/* Unserialize Kernel-Consola
***********************************************/
SocketBuffer* unserializeKerCon(Stream){

}

/***********************************************/
/* Unserialize CPU-Kernel
***********************************************/
SocketBuffer* unserializeCpuKer(Stream){

}

/***********************************************/
/* Unserialize CPU-UMC
***********************************************/
SocketBuffer* unserializeCpuUmc(Stream){

}

/***********************************************/
/* Unserialize UMC-Kernel
***********************************************/
SocketBuffer* unserializeUmcKer(Stream){

}

/***********************************************/
/* Unserialize UMC-CPU
***********************************************/
SocketBuffer* unserializeUmcCpu(Stream){

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
