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
StrConKer* newStrConKer(Char id, Char action, Byte* fileContent,
		Int32U fileContentLen) {
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
StrKerCpu* newStrKerCpu(Char id, Char action, pcb pcb, Int32U quantum,
		Int32U quantumSleep, Byte* data, Int32U dataLen,
		Byte* nombreDispositivo, Int32U lenNomDispositivo) {
	StrKerCpu* skc = malloc(sizeof(StrKerCpu));
	skc->id = id;
	skc->action = action;
	skc->pcb = pcb;
	skc->quantum = quantum;
	skc->quantumSleep = quantumSleep;
	skc->data = data;
	skc->dataLen = dataLen;
	skc->nombreDispositivo = nombreDispositivo;
	skc->lenNomDispositivo = lenNomDispositivo;
	return skc;
}

/*******************************
 * Constructor Kernel-UMC
 ******************************/
StrKerUmc* newStrKerUmc(Char id, Char action, Byte* data, Int32U size,
		Int32U pid, Int32U cantPage, Int32U pagina, Int32U offset,
		Int32U tamanio) {
	StrKerUmc* sku = malloc(sizeof(StrKerUmc));
	sku->id = id;
	sku->action = action;
	sku->data = data;
	sku->size = size;
	sku->pid = pid;
	sku->cantPage = cantPage;
	sku->pagina = pagina;
	sku->offset = offset;
	sku->tamanio = tamanio;
	return sku;
}

/*******************************
 * Constructor Kernel-Consola
 ******************************/
StrKerCon* newStrKerCon(Char id, Char action, Int32U logLen, Byte* log) {
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
StrCpuKer* newStrCpuKer(Char id, Char action, pcb pcb, Int32U pid,
		Int32U logLen, Byte* log, Byte* nombreDispositivo,
		Int32U lenNomDispositivo) {
	StrCpuKer* sck = malloc(sizeof(StrCpuKer));
	sck->id = id;
	sck->action = action;
	sck->pcb = pcb;
	sck->pid = pid;
	sck->logLen = logLen;
	sck->log = log;
	sck->nombreDispositivo = nombreDispositivo;
	sck->lenNomDispositivo = lenNomDispositivo;
	return sck;
}

/*******************************
 * Constructor CPU-UMC
 ******************************/
StrCpuUmc* newStrCpuUmc(Char id, Char action, espacioAsignado pageComienzo,
		Int32U offset, Int32U dataLen, Byte* data, Int32U pid) {
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
StrUmcKer* newStrUmcKer(Char id, Char action, Byte* data, Int32U size,
		Int32U pid, Int32U cantPage) {
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

StrUmcCpu* newStrUmcCpu(Char id, Char action, espacioAsignado pageComienzo,
		Int32U offset, Int32U dataLen, Byte* data, Int32U pid) {
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
StrUmcSwa* newStrUmcSwa(Char id, Char action, espacioAsignado pageComienzo,
		Int32U cantPage, Byte* data, Int32U dataLen, Int32U pid) {
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
StrSwaUmc* newStrSwaUmc(Char id, Char action, paginaAsignada pageComienzo,
		Int32U cantPage, Byte* data, Int32U dataLen, Int32U pid) {
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
Int32U getSizeConKer(StrConKer* sck) {
	Int32U size = 0;
	size += sizeof(sck->id);
	size += sizeof(sck->action);
	size += sizeof(sck->fileContentLen);
	size += sck->fileContentLen;
	return size;
}

/*******************************
 * getSize Kernel-CPU
 ******************************/
Int32U getSizeKerCpu(StrKerCpu* skc) {
	Int32U size = 0;
	size += sizeof(skc->id);
	size += sizeof(skc->action);
	// size del pcb
	size += sizeof(skc->pcb.consola);
	size += sizeof(skc->pcb.id);
	size += sizeof(skc->pcb.tamanioArchivoOriginal);
	size += sizeof(skc->pcb.programCounter);
	size += sizeof(skc->pcb.paginasDeCodigo);
	size += sizeof(arrayBidimensional) * (skc->pcb.instruccionesTotales);
	size += sizeof(skc->pcb.indiceDeCodigoSize);
	size += sizeof(skc->pcb.indiceDeEtiquetasSize);
	size += sizeof(skc->pcb.etiquetaSize);
	size += skc->pcb.etiquetaSize;
	size += sizeof(skc->pcb.instruccionesTotales);
	size += sizeof(skc->pcb.instruccionesRestantes);
	size += sizeof(skc->pcb.cantElementsStack);
	size += sizeof(skc->pcb.estado);

	int sumadorLoco =0;
	int i;
	paginaDeStack *aux;

	if(skc->pcb.indiceDelStack->elements_count!=0){

		for(i=0; i<(skc->pcb.indiceDelStack->elements_count);i++){

		aux=list_get(skc->pcb.indiceDelStack,i);

		sumadorLoco+=20;
		sumadorLoco+=(aux->args->elements_count)*12;
		sumadorLoco+=(aux->vars->elements_count)*13;
		sumadorLoco+=(aux->retVars->elements_count)*13;



		}
	}


	//size += ((skc->pcb.indiceDelStack->elements_count) * (46)); //Hardcodeando a lo piola
	// fin size pcb
	size += sumadorLoco;
	size += sizeof(skc->quantum);
	size += sizeof(skc->quantumSleep);
	size += sizeof(skc->dataLen);
	size += skc->dataLen;
	size += sizeof(skc->lenNomDispositivo);
	size += skc->lenNomDispositivo;
	return size;
}

/*******************************
 * getSize Kernel-UMC
 ******************************/
Int32U getSizeKerUmc(StrKerUmc* sku) {
	Int32U size = 0;
	size += sizeof(sku->id);
	size += sizeof(sku->action);
	size += sizeof(sku->pid);
	size += sizeof(sku->cantPage);
	size += sizeof(sku->pagina);
	size += sizeof(sku->offset);
	size += sizeof(sku->tamanio);
	size += sizeof(sku->size);
	size += sku->size;
	return size;
}

/*******************************
 * getSize Kernel-Consola
 ******************************/
Int32U getSizeKerCon(StrKerCon* skc) {
	Int32U size = 0;
	size += sizeof(skc->id);
	size += sizeof(skc->action);
	size += sizeof(skc->logLen);
	size += skc->logLen;
	return size;
}

/*******************************
 * getSize CPU-Kernel
 ******************************/
Int32U getSizeCpuKer(StrCpuKer* sck) {
	Int32U size = 0;
	size += sizeof(sck->id);
	size += sizeof(sck->action);
	// size del pcb
	size += sizeof(sck->pcb.consola);
	size += sizeof(sck->pcb.id);
	size += sizeof(sck->pcb.tamanioArchivoOriginal);
	size += sizeof(sck->pcb.programCounter);
	size += sizeof(sck->pcb.paginasDeCodigo);
	size += sizeof(arrayBidimensional) * (sck->pcb.instruccionesTotales);
	size += sizeof(sck->pcb.indiceDeCodigoSize);
	size += sizeof(sck->pcb.indiceDeEtiquetasSize);
	size += sizeof(sck->pcb.etiquetaSize);
	size += sck->pcb.etiquetaSize;
	size += sizeof(sck->pcb.instruccionesTotales);
	size += sizeof(sck->pcb.instruccionesRestantes);
	size += sizeof(sck->pcb.cantElementsStack);
	size += sizeof(sck->pcb.estado);

	//size += ((sck->pcb.indiceDelStack->elements_count) * (46)); //Hardcodeando a lo groso


	int sumadorLoco =0;
	int i;
	paginaDeStack *aux;

	if(sck->pcb.indiceDelStack->elements_count!=0){

		for(i=0; i<(sck->pcb.indiceDelStack->elements_count);i++){

		aux=list_get(sck->pcb.indiceDelStack,i);

		sumadorLoco+=20;
		sumadorLoco+=(aux->args->elements_count)*12;
		sumadorLoco+=(aux->vars->elements_count)*13;
		sumadorLoco+=(aux->retVars->elements_count)*13;



		}
	}


	// fin size pcb
	size += sizeof(sck->pid);
	size += sizeof(sck->logLen);
	size += sck->logLen;
	size += sizeof(sck->lenNomDispositivo);
	size += sck->lenNomDispositivo;
	return size;
}

/*******************************
 * getSize CPU-UMC
 ******************************/
Int32U getSizeCpuUmc(StrCpuUmc* scu) {
	Int32U size = 0;
	size += sizeof(scu->id);
	size += sizeof(scu->action);
	size += sizeof(scu->pageComienzo.IDPaginaInterno);
	size += sizeof(scu->pageComienzo.pid);
	size += sizeof(scu->pageComienzo.numDePag);
	size += sizeof(scu->pageComienzo.bitUso);
	size += sizeof(scu->pageComienzo.bitModificado);
	size += sizeof(scu->pageComienzo.punteroAPagina);
	size += sizeof(scu->pageComienzo.bitDePresencia);
	size += sizeof(scu->offset);
	size += sizeof(scu->pid);
	size += sizeof(scu->dataLen);
	size += scu->dataLen;
	return size;
}

/*******************************
 * getSize UMC-Kernel
 ******************************/
Int32U getSizeUmcKer(StrUmcKer* suk) {
	Int32U size = 0;
	size += sizeof(suk->id);
	size += sizeof(suk->action);
	size += sizeof(suk->pid);
	size += sizeof(suk->cantPage);
	size += sizeof(suk->size);
	size += suk->size;
	return size;
}

/*******************************
 * getSize UMC-CPU
 ******************************/
Int32U getSizeUmcCpu(StrUmcCpu* suc) {
	Int32U size = 0;
	size += sizeof(suc->id);
	size += sizeof(suc->action);
	size += sizeof(suc->pageComienzo.IDPaginaInterno);
	size += sizeof(suc->pageComienzo.pid);
	size += sizeof(suc->pageComienzo.numDePag);
	size += sizeof(suc->pageComienzo.bitUso);
	size += sizeof(suc->pageComienzo.bitModificado);
	size += sizeof(suc->pageComienzo.punteroAPagina);
	size += sizeof(suc->pageComienzo.bitDePresencia);
	size += sizeof(suc->offset);
	size += sizeof(suc->pid);
	size += sizeof(suc->dataLen);
	size += suc->dataLen;
	return size;
}

/*******************************
 * getSize UMC-Swap
 ******************************/
Int32U getSizeUmcSwa(StrUmcSwa* sus) {
	Int32U size = 0;
	size += sizeof(sus->id);
	size += sizeof(sus->action);
	size += sizeof(sus->pageComienzo.IDPaginaInterno);
	size += sizeof(sus->pageComienzo.pid);
	size += sizeof(sus->pageComienzo.numDePag);
	size += sizeof(sus->pageComienzo.bitUso);
	size += sizeof(sus->pageComienzo.bitModificado);
	size += sizeof(sus->pageComienzo.punteroAPagina);
	size += sizeof(sus->pageComienzo.bitDePresencia);
	size += sizeof(sus->cantPage);
	size += sizeof(sus->pid);
	size += sizeof(sus->dataLen);
	size += sus->dataLen;
	return size;
}

/*******************************
 * getSize Swap-UMC
 ******************************/
Int32U getSizeSwaUmc(StrSwaUmc* ssu) {
	Int32U size = 0;
	size += sizeof(ssu->id);
	size += sizeof(ssu->action);
	size += sizeof(ssu->pageComienzo.IDPaginaInterno);
	size += sizeof(ssu->pageComienzo.pid);
	size += sizeof(ssu->pageComienzo.bitLectura);
	size += sizeof(ssu->pageComienzo.numDePag);
	size += sizeof(ssu->cantPage);
	size += sizeof(ssu->pid);
	size += sizeof(ssu->dataLen);
	size += ssu->dataLen;
	return size;
}

//================================================================================================

//================================================================================================
/* Serialization
 //================================================================================================

 / ******************************
 * Serialization Consola-Kernel
 * *****************************/
SocketBuffer* serializeConKer(StrConKer* sconk) {
	Int32U size = getSizeConKer(sconk);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &sconk->id;
	memcpy(ptrData, ptrByte, sizeof(sconk->id));
	ptrData += sizeof(sconk->id);

	ptrByte = (Byte*) &sconk->action;
	memcpy(ptrData, ptrByte, sizeof(sconk->action));
	ptrData += sizeof(sconk->action);

	ptrByte = (Byte*) &sconk->fileContentLen;
	memcpy(ptrData, ptrByte, sizeof(sconk->fileContentLen));
	ptrData += sizeof(sconk->fileContentLen);

	ptrByte = (Byte*) sconk->fileContent;
	memcpy(ptrData, ptrByte, sconk->fileContentLen);
	ptrData += sconk->fileContentLen;

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

/***************************
 * Serialization Kernel-CPU
 **************************/
SocketBuffer* serializeKerCpu(StrKerCpu* skc) {
	Int32U size = getSizeKerCpu(skc);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &skc->id;
	memcpy(ptrData, ptrByte, sizeof(skc->id));
	ptrData += sizeof(skc->id);

	ptrByte = (Byte*) &skc->action;
	memcpy(ptrData, ptrByte, sizeof(skc->action));
	ptrData += sizeof(skc->action);

	// comienzo a serializar el pcb
	ptrByte = (Byte*) &skc->pcb.consola;
	memcpy(ptrData, ptrByte, sizeof(skc->pcb.consola));
	ptrData += sizeof(skc->pcb.consola);

	ptrByte = (Byte*) &skc->pcb.id;
	memcpy(ptrData, ptrByte, sizeof(skc->pcb.id));
	ptrData += sizeof(skc->pcb.id);

	ptrByte = (Byte*) &skc->pcb.tamanioArchivoOriginal;
	memcpy(ptrData, ptrByte, sizeof(skc->pcb.tamanioArchivoOriginal));
	ptrData += sizeof(skc->pcb.tamanioArchivoOriginal);

	ptrByte = (Byte*) &skc->pcb.programCounter;
	memcpy(ptrData, ptrByte, sizeof(skc->pcb.programCounter));
	ptrData += sizeof(skc->pcb.programCounter);

	ptrByte = (Byte*) &skc->pcb.paginasDeCodigo;
	memcpy(ptrData, ptrByte, sizeof(skc->pcb.paginasDeCodigo));
	ptrData += sizeof(skc->pcb.paginasDeCodigo);

	ptrByte = (Byte*) &skc->pcb.indiceDeCodigoSize;
	memcpy(ptrData, ptrByte, sizeof(skc->pcb.indiceDeCodigoSize));
	ptrData += sizeof(skc->pcb.indiceDeCodigoSize);

	ptrByte = (Byte*) skc->pcb.indiceDeCodigo;
	memcpy(ptrData, ptrByte, (skc->pcb.indiceDeCodigoSize));
	ptrData += (skc->pcb.indiceDeCodigoSize);

	ptrByte = (Byte*) &skc->pcb.indiceDeEtiquetasSize;
	memcpy(ptrData, ptrByte, sizeof(skc->pcb.indiceDeEtiquetasSize));
	ptrData += sizeof(skc->pcb.indiceDeEtiquetasSize);

	ptrByte = (Byte*) &skc->pcb.etiquetaSize;
	memcpy(ptrData, ptrByte, sizeof(skc->pcb.etiquetaSize));
	ptrData += sizeof(skc->pcb.etiquetaSize);

	ptrByte = (Byte*) skc->pcb.indiceDeEtiquetas;
	memcpy(ptrData, ptrByte, (skc->pcb.etiquetaSize));
	ptrData += skc->pcb.etiquetaSize;

	ptrByte = (Byte*) &skc->pcb.instruccionesTotales;
	memcpy(ptrData, ptrByte, sizeof(skc->pcb.instruccionesTotales));
	ptrData += sizeof(skc->pcb.instruccionesTotales);

	ptrByte = (Byte*) &skc->pcb.instruccionesRestantes;
	memcpy(ptrData, ptrByte, sizeof(skc->pcb.instruccionesRestantes));
	ptrData += sizeof(skc->pcb.instruccionesRestantes);

	skc->pcb.cantElementsStack = (skc->pcb.indiceDelStack->elements_count);

	ptrByte = (Byte*) &skc->pcb.cantElementsStack;
	memcpy(ptrData, ptrByte, sizeof(skc->pcb.cantElementsStack));
	ptrData += sizeof(skc->pcb.cantElementsStack);

	int i;
	int j;
	paginaDeStack *aux = crearPaginaDeStackVaciaPiola();
	argumentos *auxArgs;
	variables *auxVars;
	variablesRetorno * auxVarsRet;
	if ((skc->pcb.indiceDelStack->elements_count) != 0) {
		for (i = 0; i < (skc->pcb.indiceDelStack->elements_count); i++) {
			aux = list_get(skc->pcb.indiceDelStack, i);

			// ------------------------------------------------------------------
			ptrByte = (Byte*) &aux->pos;
			memcpy(ptrData, ptrByte, sizeof(aux->pos));
			ptrData += sizeof(aux->pos);

			ptrByte = (Byte*) &aux->retPos;
			memcpy(ptrData, ptrByte, sizeof(aux->retPos));
			ptrData += sizeof(aux->retPos);
//--------------------------------------------------------------------------------------------
			aux->cantArgs = aux->args->elements_count;

			ptrByte = (Byte*) &aux->cantArgs;
			memcpy(ptrData, ptrByte, sizeof(aux->cantArgs));
			ptrData += sizeof(aux->cantArgs);

			if (aux->cantArgs != 0) {
				for (j = 0; j < aux->cantArgs; j++) {
					auxArgs = list_get(aux->args, j);

					ptrByte = (Byte*) &auxArgs->pagArg;
					memcpy(ptrData, ptrByte, sizeof(auxArgs->pagArg));
					ptrData += sizeof(auxArgs->pagArg);

					ptrByte = (Byte*) &auxArgs->offArgs;
					memcpy(ptrData, ptrByte, sizeof(auxArgs->offArgs));
					ptrData += sizeof(auxArgs->offArgs);

					ptrByte = (Byte*) &auxArgs->sizeArgs;
					memcpy(ptrData, ptrByte, sizeof(auxArgs->sizeArgs));
					ptrData += sizeof(auxArgs->sizeArgs);

				}
			}

//-------------------------------------------------------------------------------------------------
			aux->cantVars = aux->vars->elements_count;

			ptrByte = (Byte*) &aux->cantVars;
			memcpy(ptrData, ptrByte, sizeof(aux->cantVars));
			ptrData += sizeof(aux->cantVars);

			if (aux->cantVars != 0) {
				for (j = 0; j < aux->cantVars; j++) {
					auxVars = list_get(aux->vars, j);

					ptrByte = (Byte*) &auxVars->idVar;
					memcpy(ptrData, ptrByte, sizeof(auxVars->idVar));
					ptrData += sizeof(auxVars->idVar);

					ptrByte = (Byte*) &auxVars->pagVar;
					memcpy(ptrData, ptrByte, sizeof(auxVars->pagVar));
					ptrData += sizeof(auxVars->pagVar);

					ptrByte = (Byte*) &auxVars->offVar;
					memcpy(ptrData, ptrByte, sizeof(auxVars->offVar));
					ptrData += sizeof(auxVars->offVar);

					ptrByte = (Byte*) &auxVars->sizeVar;
					memcpy(ptrData, ptrByte, sizeof(auxVars->sizeVar));
					ptrData += sizeof(auxVars->sizeVar);

				}
			}

//--------------------------------------------------------------------------------------
			aux->cantRetvars = aux->retVars->elements_count;

			ptrByte = (Byte*) &aux->cantRetvars;
			memcpy(ptrData, ptrByte, sizeof(aux->cantRetvars));
			ptrData += sizeof(aux->cantRetvars);

			if (aux->cantRetvars != 0) {
				for (j = 0; j < aux->cantRetvars; j++) {
					auxVarsRet = list_get(aux->retVars, j);

					ptrByte = (Byte*) &auxVarsRet->idVarRet;
					memcpy(ptrData, ptrByte, sizeof(auxVarsRet->idVarRet));
					ptrData += sizeof(auxVarsRet->idVarRet);

					ptrByte = (Byte*) &auxVarsRet->pagVarRet;
					memcpy(ptrData, ptrByte, sizeof(auxVarsRet->pagVarRet));
					ptrData += sizeof(auxVarsRet->pagVarRet);

					ptrByte = (Byte*) &auxVarsRet->offVarRet;
					memcpy(ptrData, ptrByte, sizeof(auxVarsRet->offVarRet));
					ptrData += sizeof(auxVarsRet->offVarRet);

					ptrByte = (Byte*) &auxVarsRet->sizeVarRet;
					memcpy(ptrData, ptrByte, sizeof(auxVarsRet->sizeVarRet));
					ptrData += sizeof(auxVarsRet->sizeVarRet);

				}
			}

		}
	}

	ptrByte = (Byte*) &skc->pcb.estado;
	memcpy(ptrData, ptrByte, sizeof(skc->pcb.estado));
	ptrData += sizeof(skc->pcb.estado);
	// termino de serializar el pcb

	ptrByte = (Byte*) &skc->quantum;
	memcpy(ptrData, ptrByte, sizeof(skc->quantum));
	ptrData += sizeof(skc->quantum);

	ptrByte = (Byte*) &skc->quantumSleep;
	memcpy(ptrData, ptrByte, sizeof(skc->quantumSleep));
	ptrData += sizeof(skc->quantumSleep);

	ptrByte = (Byte*) &skc->dataLen;
	memcpy(ptrData, ptrByte, sizeof(skc->dataLen));
	ptrData += sizeof(skc->dataLen);

	ptrByte = (Byte*) skc->data;
	memcpy(ptrData, ptrByte, skc->dataLen);
	ptrData += skc->dataLen;

	ptrByte = (Byte*) &skc->lenNomDispositivo;
	memcpy(ptrData, ptrByte, sizeof(skc->lenNomDispositivo));
	ptrData += sizeof(skc->lenNomDispositivo);

	ptrByte = (Byte*) skc->nombreDispositivo;
	memcpy(ptrData, ptrByte, skc->lenNomDispositivo);
	ptrData += skc->lenNomDispositivo;

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

/***************************
 * Serialization Kernel-UMC
 ***************************/
SocketBuffer* serializeKerUmc(StrKerUmc* sku) {
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

	ptrByte = (Byte*) &sku->size;
	memcpy(ptrData, ptrByte, sizeof(sku->size));
	ptrData += sizeof(sku->size);

	ptrByte = (Byte*) sku->data;
	memcpy(ptrData, ptrByte, sku->size);
	ptrData += sku->size;

	ptrByte = (Byte*) &sku->pid;
	memcpy(ptrData, ptrByte, sizeof(sku->pid));
	ptrData += sizeof(sku->pid);

	ptrByte = (Byte*) &sku->cantPage;
	memcpy(ptrData, ptrByte, sizeof(sku->cantPage));
	ptrData += sizeof(sku->cantPage);

	ptrByte = (Byte*) &sku->pagina;
	memcpy(ptrData, ptrByte, sizeof(sku->pagina));
	ptrData += sizeof(sku->pagina);

	ptrByte = (Byte*) &sku->offset;
	memcpy(ptrData, ptrByte, sizeof(sku->offset));
	ptrData += sizeof(sku->offset);

	ptrByte = (Byte*) &sku->tamanio;
	memcpy(ptrData, ptrByte, sizeof(sku->tamanio));
	ptrData += sizeof(sku->tamanio);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

/******************************
 * Serialization Kernel-Consola
 *****************************/
SocketBuffer* serializeKerCon(StrKerCon* skcon) {
	Int32U size = getSizeKerCon(skcon);
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

	ptrByte = (Byte*) skcon->log;
	memcpy(ptrData, ptrByte, skcon->logLen);
	ptrData += skcon->logLen;

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

/***************************
 * Serialization CPU-Kernel
 **************************/
SocketBuffer* serializeCpuKer(StrCpuKer* sck) {
	Int32U size = getSizeCpuKer(sck);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &sck->id;
	memcpy(ptrData, ptrByte, sizeof(sck->id));
	ptrData += sizeof(sck->id);

	ptrByte = (Byte*) &sck->action;
	memcpy(ptrData, ptrByte, sizeof(sck->action));
	ptrData += sizeof(sck->action);

	// comienzo a serializar el pcb
	ptrByte = (Byte*) &sck->pcb.consola;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb.consola));
	ptrData += sizeof(sck->pcb.consola);

	ptrByte = (Byte*) &sck->pcb.id;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb.id));
	ptrData += sizeof(sck->pcb.id);

	ptrByte = (Byte*) &sck->pcb.tamanioArchivoOriginal;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb.tamanioArchivoOriginal));
	ptrData += sizeof(sck->pcb.tamanioArchivoOriginal);

	ptrByte = (Byte*) &sck->pcb.programCounter;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb.programCounter));
	ptrData += sizeof(sck->pcb.programCounter);

	ptrByte = (Byte*) &sck->pcb.paginasDeCodigo;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb.paginasDeCodigo));
	ptrData += sizeof(sck->pcb.paginasDeCodigo);

	ptrByte = (Byte*) &sck->pcb.indiceDeCodigoSize;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb.indiceDeCodigoSize));
	ptrData += sizeof(sck->pcb.indiceDeCodigoSize);

	ptrByte = (Byte*) sck->pcb.indiceDeCodigo;
	memcpy(ptrData, ptrByte, (sck->pcb.indiceDeCodigoSize));
	ptrData += (sck->pcb.indiceDeCodigoSize);

	ptrByte = (Byte*) &sck->pcb.indiceDeEtiquetasSize;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb.indiceDeEtiquetasSize));
	ptrData += sizeof(sck->pcb.indiceDeEtiquetasSize);

	ptrByte = (Byte*) &sck->pcb.etiquetaSize;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb.etiquetaSize));
	ptrData += sizeof(sck->pcb.etiquetaSize);

	ptrByte = (Byte*) sck->pcb.indiceDeEtiquetas;
	memcpy(ptrData, ptrByte, (sck->pcb.etiquetaSize));
	ptrData += sck->pcb.etiquetaSize;

	ptrByte = (Byte*) &sck->pcb.instruccionesTotales;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb.instruccionesTotales));
	ptrData += sizeof(sck->pcb.instruccionesTotales);

	ptrByte = (Byte*) &sck->pcb.instruccionesRestantes;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb.instruccionesRestantes));
	ptrData += sizeof(sck->pcb.instruccionesRestantes);

	sck->pcb.cantElementsStack = (sck->pcb.indiceDelStack->elements_count);

	ptrByte = (Byte*) &sck->pcb.cantElementsStack;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb.cantElementsStack));
	ptrData += sizeof(sck->pcb.cantElementsStack);

	int i;
	int j;
	paginaDeStack *aux = crearPaginaDeStackVaciaPiola();
	argumentos *auxArgs;
	variables *auxVars;
	variablesRetorno * auxVarsRet;
	if ((sck->pcb.indiceDelStack->elements_count) != 0) {
		for (i = 0; i < (sck->pcb.indiceDelStack->elements_count); i++) {
			aux = list_get(sck->pcb.indiceDelStack, i);

			// ------------------------------------------------------------------
			ptrByte = (Byte*) &aux->pos;
			memcpy(ptrData, ptrByte, sizeof(aux->pos));
			ptrData += sizeof(aux->pos);

			ptrByte = (Byte*) &aux->retPos;
			memcpy(ptrData, ptrByte, sizeof(aux->retPos));
			ptrData += sizeof(aux->retPos);
			//--------------------------------------------------------------------------------------------
			aux->cantArgs = aux->args->elements_count;

			ptrByte = (Byte*) &aux->cantArgs;
			memcpy(ptrData, ptrByte, sizeof(aux->cantArgs));
			ptrData += sizeof(aux->cantArgs);

			if (aux->cantArgs != 0) {
				for (j = 0; j < aux->cantArgs; j++) {
					auxArgs = list_get(aux->args, j);

					ptrByte = (Byte*) &auxArgs->pagArg;
					memcpy(ptrData, ptrByte, sizeof(auxArgs->pagArg));
					ptrData += sizeof(auxArgs->pagArg);

					ptrByte = (Byte*) &auxArgs->offArgs;
					memcpy(ptrData, ptrByte, sizeof(auxArgs->offArgs));
					ptrData += sizeof(auxArgs->offArgs);

					ptrByte = (Byte*) &auxArgs->sizeArgs;
					memcpy(ptrData, ptrByte, sizeof(auxArgs->sizeArgs));
					ptrData += sizeof(auxArgs->sizeArgs);

				}
			}

			//-------------------------------------------------------------------------------------------------
			aux->cantVars = aux->vars->elements_count;

			ptrByte = (Byte*) &aux->cantVars;
			memcpy(ptrData, ptrByte, sizeof(aux->cantVars));
			ptrData += sizeof(aux->cantVars);

			if (aux->cantVars != 0) {
				for (j = 0; j < aux->cantVars; j++) {
					auxVars = list_get(aux->vars, j);

					ptrByte = (Byte*) &auxVars->idVar;
					memcpy(ptrData, ptrByte, sizeof(auxVars->idVar));
					ptrData += sizeof(auxVars->idVar);

					ptrByte = (Byte*) &auxVars->pagVar;
					memcpy(ptrData, ptrByte, sizeof(auxVars->pagVar));
					ptrData += sizeof(auxVars->pagVar);

					ptrByte = (Byte*) &auxVars->offVar;
					memcpy(ptrData, ptrByte, sizeof(auxVars->offVar));
					ptrData += sizeof(auxVars->offVar);

					ptrByte = (Byte*) &auxVars->sizeVar;
					memcpy(ptrData, ptrByte, sizeof(auxVars->sizeVar));
					ptrData += sizeof(auxVars->sizeVar);

				}
			}

			//--------------------------------------------------------------------------------------
			aux->cantRetvars = aux->retVars->elements_count;

			ptrByte = (Byte*) &aux->cantRetvars;
			memcpy(ptrData, ptrByte, sizeof(aux->cantRetvars));
			ptrData += sizeof(aux->cantRetvars);

			if (aux->cantRetvars != 0) {
				for (j = 0; j < aux->cantRetvars; j++) {
					auxVarsRet = list_get(aux->retVars, j);

					ptrByte = (Byte*) &auxVarsRet->idVarRet;
					memcpy(ptrData, ptrByte, sizeof(auxVarsRet->idVarRet));
					ptrData += sizeof(auxVarsRet->idVarRet);

					ptrByte = (Byte*) &auxVarsRet->pagVarRet;
					memcpy(ptrData, ptrByte, sizeof(auxVarsRet->pagVarRet));
					ptrData += sizeof(auxVarsRet->pagVarRet);

					ptrByte = (Byte*) &auxVarsRet->offVarRet;
					memcpy(ptrData, ptrByte, sizeof(auxVarsRet->offVarRet));
					ptrData += sizeof(auxVarsRet->offVarRet);

					ptrByte = (Byte*) &auxVarsRet->sizeVarRet;
					memcpy(ptrData, ptrByte, sizeof(auxVarsRet->sizeVarRet));
					ptrData += sizeof(auxVarsRet->sizeVarRet);

				}
			}

		}
	}

	ptrByte = (Byte*) &sck->pcb.estado;
	memcpy(ptrData, ptrByte, sizeof(sck->pcb.estado));
	ptrData += sizeof(sck->pcb.estado);
	// termino de serializar el pcb

	ptrByte = (Byte*) &sck->pid;
	memcpy(ptrData, ptrByte, sizeof(sck->pid));
	ptrData += sizeof(sck->pid);

	ptrByte = (Byte*) &sck->logLen;
	memcpy(ptrData, ptrByte, sizeof(sck->logLen));
	ptrData += sizeof(sck->logLen);

	ptrByte = (Byte*) sck->log;
	memcpy(ptrData, ptrByte, sck->logLen);
	ptrData += sck->logLen;

	ptrByte = (Byte*) &sck->lenNomDispositivo;
	memcpy(ptrData, ptrByte, sizeof(sck->lenNomDispositivo));
	ptrData += sizeof(sck->lenNomDispositivo);

	ptrByte = (Byte*) sck->nombreDispositivo;
	memcpy(ptrData, ptrByte, sck->lenNomDispositivo);
	ptrData += sck->lenNomDispositivo;

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

/*************************
 * Serialization CPU-UMC
 ************************/
SocketBuffer* serializeCpuUmc(StrCpuUmc* scu) {
	Int32U size = getSizeCpuUmc(scu);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &scu->id;
	memcpy(ptrData, ptrByte, sizeof(scu->id));
	ptrData += sizeof(scu->id);

	ptrByte = (Byte*) &scu->action;
	memcpy(ptrData, ptrByte, sizeof(scu->action));
	ptrData += sizeof(scu->action);

	// comienzo a serializar el espacioAsignado pageComienzo
	ptrByte = (Byte*) &scu->pageComienzo.IDPaginaInterno;
	memcpy(ptrData, ptrByte, sizeof(scu->pageComienzo.IDPaginaInterno));
	ptrData += sizeof(scu->pageComienzo.IDPaginaInterno);

	ptrByte = (Byte*) &scu->pageComienzo.pid;
	memcpy(ptrData, ptrByte, sizeof(scu->pageComienzo.pid));
	ptrData += sizeof(scu->pageComienzo.pid);

	ptrByte = (Byte*) &scu->pageComienzo.numDePag;
	memcpy(ptrData, ptrByte, sizeof(scu->pageComienzo.numDePag));
	ptrData += sizeof(scu->pageComienzo.numDePag);

	ptrByte = (Byte*) &scu->pageComienzo.bitUso;
	memcpy(ptrData, ptrByte, sizeof(scu->pageComienzo.bitUso));
	ptrData += sizeof(scu->pageComienzo.bitUso);

	ptrByte = (Byte*) &scu->pageComienzo.bitModificado;
	memcpy(ptrData, ptrByte, sizeof(scu->pageComienzo.bitModificado));
	ptrData += sizeof(scu->pageComienzo.bitModificado);

	ptrByte = (Byte*) &scu->pageComienzo.punteroAPagina;
	memcpy(ptrData, ptrByte, sizeof(scu->pageComienzo.punteroAPagina));
	ptrData += sizeof(scu->pageComienzo.punteroAPagina);

	ptrByte = (Byte*) &scu->pageComienzo.bitDePresencia;
	memcpy(ptrData, ptrByte, sizeof(scu->pageComienzo.bitDePresencia));
	ptrData += sizeof(scu->pageComienzo.bitDePresencia);
	// termino de serializar el espacioAsignado pageComienzo

	ptrByte = (Byte*) &scu->offset;
	memcpy(ptrData, ptrByte, sizeof(scu->offset));
	ptrData += sizeof(scu->offset);

	ptrByte = (Byte*) &scu->dataLen;
	memcpy(ptrData, ptrByte, sizeof(scu->dataLen));
	ptrData += sizeof(scu->dataLen);

	ptrByte = (Byte*) scu->data;
	memcpy(ptrData, ptrByte, scu->dataLen);
	ptrData += scu->dataLen;

	ptrByte = (Byte*) &scu->pid;
	memcpy(ptrData, ptrByte, sizeof(scu->pid));
	ptrData += sizeof(scu->pid);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

/***************************
 * Serialization UMC-Kernel
 ***************************/
SocketBuffer* serializeUmcKer(StrUmcKer* suk) {
	Int32U size = getSizeUmcKer(suk);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &suk->id;
	memcpy(ptrData, ptrByte, sizeof(suk->id));
	ptrData += sizeof(suk->id);

	ptrByte = (Byte*) &suk->action;
	memcpy(ptrData, ptrByte, sizeof(suk->action));
	ptrData += sizeof(suk->action);

	ptrByte = (Byte*) &suk->size;
	memcpy(ptrData, ptrByte, sizeof(suk->size));
	ptrData += sizeof(suk->size);

	ptrByte = (Byte*) suk->data;
	memcpy(ptrData, ptrByte, suk->size);
	ptrData += suk->size;

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
SocketBuffer* serializeUmcCpu(StrUmcCpu* suc) {
	Int32U size = getSizeUmcCpu(suc);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &suc->id;
	memcpy(ptrData, ptrByte, sizeof(suc->id));
	ptrData += sizeof(suc->id);

	ptrByte = (Byte*) &suc->action;
	memcpy(ptrData, ptrByte, sizeof(suc->action));
	ptrData += sizeof(suc->action);

	// comienzo a serializar el espacioAsignado pageComienzo
	ptrByte = (Byte*) &suc->pageComienzo.IDPaginaInterno;
	memcpy(ptrData, ptrByte, sizeof(suc->pageComienzo.IDPaginaInterno));
	ptrData += sizeof(suc->pageComienzo.IDPaginaInterno);

	ptrByte = (Byte*) &suc->pageComienzo.pid;
	memcpy(ptrData, ptrByte, sizeof(suc->pageComienzo.pid));
	ptrData += sizeof(suc->pageComienzo.pid);

	ptrByte = (Byte*) &suc->pageComienzo.numDePag;
	memcpy(ptrData, ptrByte, sizeof(suc->pageComienzo.numDePag));
	ptrData += sizeof(suc->pageComienzo.numDePag);

	ptrByte = (Byte*) &suc->pageComienzo.bitUso;
	memcpy(ptrData, ptrByte, sizeof(suc->pageComienzo.bitUso));
	ptrData += sizeof(suc->pageComienzo.bitUso);

	ptrByte = (Byte*) &suc->pageComienzo.bitModificado;
	memcpy(ptrData, ptrByte, sizeof(suc->pageComienzo.bitModificado));
	ptrData += sizeof(suc->pageComienzo.bitModificado);

	ptrByte = (Byte*) &suc->pageComienzo.punteroAPagina;
	memcpy(ptrData, ptrByte, sizeof(suc->pageComienzo.punteroAPagina));
	ptrData += sizeof(suc->pageComienzo.punteroAPagina);

	ptrByte = (Byte*) &suc->pageComienzo.bitDePresencia;
	memcpy(ptrData, ptrByte, sizeof(suc->pageComienzo.bitDePresencia));
	ptrData += sizeof(suc->pageComienzo.bitDePresencia);
	// termino de serializar espacioAsignado pageComienzo

	ptrByte = (Byte*) &suc->offset;
	memcpy(ptrData, ptrByte, sizeof(suc->offset));
	ptrData += sizeof(suc->offset);

	ptrByte = (Byte*) &suc->dataLen;
	memcpy(ptrData, ptrByte, sizeof(suc->dataLen));
	ptrData += sizeof(suc->dataLen);

	ptrByte = (Byte*) suc->data;
	memcpy(ptrData, ptrByte, suc->dataLen);
	ptrData += suc->dataLen;

	ptrByte = (Byte*) &suc->pid;
	memcpy(ptrData, ptrByte, sizeof(suc->pid));
	ptrData += sizeof(suc->pid);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

/*************************
 * Serialization UMC-SWAP
 ************************/
SocketBuffer* serializeUmcSwa(StrUmcSwa* sus) {
	Int32U size = getSizeUmcSwa(sus);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &sus->id;
	memcpy(ptrData, ptrByte, sizeof(sus->id));
	ptrData += sizeof(sus->id);

	ptrByte = (Byte*) &sus->action;
	memcpy(ptrData, ptrByte, sizeof(sus->action));
	ptrData += sizeof(sus->action);

	// comienzo a serializar el espacioAsignado pageComienzo
	ptrByte = (Byte*) &sus->pageComienzo.IDPaginaInterno;
	memcpy(ptrData, ptrByte, sizeof(sus->pageComienzo.IDPaginaInterno));
	ptrData += sizeof(sus->pageComienzo.IDPaginaInterno);

	ptrByte = (Byte*) &sus->pageComienzo.pid;
	memcpy(ptrData, ptrByte, sizeof(sus->pageComienzo.pid));
	ptrData += sizeof(sus->pageComienzo.pid);

	ptrByte = (Byte*) &sus->pageComienzo.numDePag;
	memcpy(ptrData, ptrByte, sizeof(sus->pageComienzo.numDePag));
	ptrData += sizeof(sus->pageComienzo.numDePag);

	ptrByte = (Byte*) &sus->pageComienzo.bitUso;
	memcpy(ptrData, ptrByte, sizeof(sus->pageComienzo.bitUso));
	ptrData += sizeof(sus->pageComienzo.bitUso);

	ptrByte = (Byte*) &sus->pageComienzo.bitModificado;
	memcpy(ptrData, ptrByte, sizeof(sus->pageComienzo.bitModificado));
	ptrData += sizeof(sus->pageComienzo.bitModificado);

	ptrByte = (Byte*) &sus->pageComienzo.punteroAPagina;
	memcpy(ptrData, ptrByte, sizeof(sus->pageComienzo.punteroAPagina));
	ptrData += sizeof(sus->pageComienzo.punteroAPagina);

	ptrByte = (Byte*) &sus->pageComienzo.bitDePresencia;
	memcpy(ptrData, ptrByte, sizeof(sus->pageComienzo.bitDePresencia));
	ptrData += sizeof(sus->pageComienzo.bitDePresencia);
	// termino de serializar el espacioAsignado pageComienzo

	ptrByte = (Byte*) &sus->cantPage;
	memcpy(ptrData, ptrByte, sizeof(sus->cantPage));
	ptrData += sizeof(sus->cantPage);

	ptrByte = (Byte*) &sus->dataLen;
	memcpy(ptrData, ptrByte, sizeof(sus->dataLen));
	ptrData += sizeof(sus->dataLen);

	ptrByte = (Byte*) sus->data;
	memcpy(ptrData, ptrByte, sus->dataLen);
	ptrData += sus->dataLen;

	ptrByte = (Byte*) &sus->pid;
	memcpy(ptrData, ptrByte, sizeof(sus->pid));
	ptrData += sizeof(sus->pid);

	t_bitarray* barray = bitarray_create((char*) data, size);
	return bitarrayToSocketBuffer(barray);
}

/*************************
 * Serialization SWAP-UMC
 ************************/
SocketBuffer* serializeSwaUmc(StrSwaUmc* ssu) {
	Int32U size = getSizeSwaUmc(ssu);
	Stream data = malloc(size);
	Stream ptrData = data;
	Byte* ptrByte;

	ptrByte = (Byte*) &ssu->id;
	memcpy(ptrData, ptrByte, sizeof(ssu->id));
	ptrData += sizeof(ssu->id);

	ptrByte = (Byte*) &ssu->action;
	memcpy(ptrData, ptrByte, sizeof(ssu->action));
	ptrData += sizeof(ssu->action);

	// comienzo de serializar el paginaAsignada pageComienzo
	ptrByte = (Byte*) &ssu->pageComienzo.IDPaginaInterno;
	memcpy(ptrData, ptrByte, sizeof(ssu->pageComienzo.IDPaginaInterno));
	ptrData += sizeof(ssu->pageComienzo.IDPaginaInterno);

	ptrByte = (Byte*) &ssu->pageComienzo.pid;
	memcpy(ptrData, ptrByte, sizeof(ssu->pageComienzo.pid));
	ptrData += sizeof(ssu->pageComienzo.pid);

	ptrByte = (Byte*) &ssu->pageComienzo.bitLectura;
	memcpy(ptrData, ptrByte, sizeof(ssu->pageComienzo.bitLectura));
	ptrData += sizeof(ssu->pageComienzo.bitLectura);

	ptrByte = (Byte*) &ssu->pageComienzo.numDePag;
	memcpy(ptrData, ptrByte, sizeof(ssu->pageComienzo.numDePag));
	ptrData += sizeof(ssu->pageComienzo.numDePag);
	// termino de serializar el paginaAsignada pageComienzo

	ptrByte = (Byte*) &ssu->cantPage;
	memcpy(ptrData, ptrByte, sizeof(ssu->cantPage));
	ptrData += sizeof(ssu->cantPage);

	ptrByte = (Byte*) &ssu->dataLen;
	memcpy(ptrData, ptrByte, sizeof(ssu->dataLen));
	ptrData += sizeof(ssu->dataLen);

	ptrByte = (Byte*) ssu->data;
	memcpy(ptrData, ptrByte, ssu->dataLen);
	ptrData += ssu->dataLen;

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
StrConKer* unserializeConKer(Stream dataSerialized) {
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	Byte* fileContent = NULL;
	Int32U fileContentLen;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);

	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

	memcpy(&fileContentLen, ptrByte, sizeof(fileContentLen));
	ptrByte += sizeof(fileContentLen);

	fileContent = malloc(fileContentLen);
	memcpy(fileContent, ptrByte, fileContentLen);
	ptrByte += fileContentLen;

	//fileContent[fileContentLen] = '\0';

	free(dataSerialized);
	return newStrConKer(id, action, fileContent, fileContentLen);
}

/***********************************************/
/* Unserialize Kernel-CPU
 ***********************************************/
StrKerCpu* unserializeKerCpu(Stream dataSerialized) {
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	pcb pcb;
	Int32U quantum;
	Int32U quantumSleep;
	Byte* data = NULL;
	Int32U dataLen;
	Byte* nombreDispositivo = NULL;
	Int32U lenNomDispositivo;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);

	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

	// comienzo a deserealizar el pcb
	memcpy(&pcb.consola, ptrByte, sizeof(pcb.consola));
	ptrByte += sizeof(pcb.consola);

	memcpy(&pcb.id, ptrByte, sizeof(pcb.id));
	ptrByte += sizeof(pcb.id);

	memcpy(&pcb.tamanioArchivoOriginal, ptrByte,
			sizeof(pcb.tamanioArchivoOriginal));
	ptrByte += sizeof(pcb.tamanioArchivoOriginal);

	memcpy(&pcb.programCounter, ptrByte, sizeof(pcb.programCounter));
	ptrByte += sizeof(pcb.programCounter);

	memcpy(&pcb.paginasDeCodigo, ptrByte, sizeof(pcb.paginasDeCodigo));
	ptrByte += sizeof(pcb.paginasDeCodigo);

	memcpy(&pcb.indiceDeCodigoSize, ptrByte, sizeof(pcb.indiceDeCodigoSize));
	ptrByte += sizeof(pcb.indiceDeCodigoSize);

	pcb.indiceDeCodigo = malloc(pcb.indiceDeCodigoSize);
	memcpy(pcb.indiceDeCodigo, ptrByte, pcb.indiceDeCodigoSize);
	ptrByte += pcb.indiceDeCodigoSize;

	memcpy(&pcb.indiceDeEtiquetasSize, ptrByte,
			sizeof(pcb.indiceDeEtiquetasSize));
	ptrByte += sizeof(pcb.indiceDeEtiquetasSize);

	memcpy(&pcb.etiquetaSize, ptrByte, sizeof(pcb.etiquetaSize));
	ptrByte += sizeof(pcb.etiquetaSize);

	pcb.indiceDeEtiquetas = malloc(pcb.etiquetaSize);
	memcpy(pcb.indiceDeEtiquetas, ptrByte, (pcb.etiquetaSize));
	ptrByte += pcb.etiquetaSize;

	//pcb.indiceDeEtiquetas[pcb.etiquetaSize] = '\0';

	memcpy(&pcb.instruccionesTotales, ptrByte,
			sizeof(pcb.instruccionesTotales));
	ptrByte += sizeof(pcb.instruccionesTotales);

	memcpy(&pcb.instruccionesRestantes, ptrByte,
			sizeof(pcb.instruccionesRestantes));
	ptrByte += sizeof(pcb.instruccionesRestantes);

	memcpy(&pcb.cantElementsStack, ptrByte, sizeof(pcb.cantElementsStack));
	ptrByte += sizeof(pcb.cantElementsStack);

	pcb.indiceDelStack = list_create();
	int i;
	int j;
	paginaDeStack* aux;
	argumentos * auxArgs;
	variables * auxVars;
	variablesRetorno * auxVarsRet;
	for (i = 0; i < pcb.cantElementsStack; i++) {
		aux = crearPaginaDeStackVaciaPiola();

		// ------------------------------------------------------------------
		memcpy(&aux->pos, ptrByte, sizeof(aux->pos));
		ptrByte += sizeof(aux->pos);

		memcpy(&aux->retPos, ptrByte, sizeof(aux->retPos));
		ptrByte += sizeof(aux->retPos);

		// ------------------------------------------------------------------

		memcpy(&aux->cantArgs, ptrByte, sizeof(aux->cantArgs));
		ptrByte += sizeof(aux->cantArgs);

		aux->args = list_create();

		for (j = 0; j < aux->cantArgs; j++) {
			auxArgs = malloc(sizeof(argumentos)); //todo

			memcpy(&auxArgs->pagArg, ptrByte, sizeof(auxArgs->pagArg));
			ptrByte += sizeof(auxArgs->pagArg);

			memcpy(&auxArgs->offArgs, ptrByte, sizeof(auxArgs->offArgs));
			ptrByte += sizeof(auxArgs->offArgs);

			memcpy(&auxArgs->sizeArgs, ptrByte, sizeof(auxArgs->sizeArgs));
			ptrByte += sizeof(auxArgs->sizeArgs);

			list_add(aux->args, auxArgs);
		}

		memcpy(&aux->cantVars, ptrByte, sizeof(aux->cantVars));
		ptrByte += sizeof(aux->cantVars);


		aux->vars = list_create();

		for (j = 0; j < aux->cantVars; j++) {
			auxVars = malloc(sizeof(variables)); //todo

			memcpy(&auxVars->idVar, ptrByte, sizeof(auxVars->idVar));
			ptrByte += sizeof(auxVars->idVar);

			memcpy(&auxVars->pagVar, ptrByte, sizeof(auxVars->pagVar));
			ptrByte += sizeof(auxVars->pagVar);

			memcpy(&auxVars->offVar, ptrByte, sizeof(auxVars->offVar));
			ptrByte += sizeof(auxVars->offVar);

			memcpy(&auxVars->sizeVar, ptrByte, sizeof(auxVars->sizeVar));
			ptrByte += sizeof(auxVars->sizeVar);

			list_add(aux->vars, auxVars);
		}

		memcpy(&aux->cantRetvars, ptrByte, sizeof(aux->cantRetvars));
		ptrByte += sizeof(aux->cantRetvars);

		aux->retVars = list_create();

		for (j = 0; j < aux->cantRetvars; j++) {
			auxVarsRet = malloc(sizeof(variablesRetorno)); //todo

			memcpy(&auxVarsRet->idVarRet, ptrByte, sizeof(auxVarsRet->idVarRet));
			ptrByte += sizeof(auxVarsRet->idVarRet);

			memcpy(&auxVarsRet->pagVarRet, ptrByte, sizeof(auxVarsRet->pagVarRet));
			ptrByte += sizeof(auxVarsRet->pagVarRet);

			memcpy(&auxVarsRet->offVarRet, ptrByte, sizeof(auxVarsRet->offVarRet));
			ptrByte += sizeof(auxVarsRet->offVarRet);

			memcpy(&auxVarsRet->sizeVarRet, ptrByte, sizeof(auxVarsRet->sizeVarRet));
			ptrByte += sizeof(auxVarsRet->sizeVarRet);

			list_add(aux->retVars, auxVarsRet);
		}



		list_add(pcb.indiceDelStack, aux);
	}

	memcpy(&pcb.estado, ptrByte, sizeof(pcb.estado));
	ptrByte += sizeof(pcb.estado);
	// termino de deserealizar el pcb

	memcpy(&quantum, ptrByte, sizeof(quantum));
	ptrByte += sizeof(quantum);

	memcpy(&quantumSleep, ptrByte, sizeof(quantumSleep));
	ptrByte += sizeof(quantumSleep);

	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);
	data = malloc(dataLen);
	memcpy(data, ptrByte, dataLen);
	ptrByte += dataLen;

	//data[dataLen] = '\0';

	memcpy(&lenNomDispositivo, ptrByte, sizeof(lenNomDispositivo));
	ptrByte += sizeof(lenNomDispositivo);
	nombreDispositivo = malloc(lenNomDispositivo);

	memcpy(nombreDispositivo, ptrByte, lenNomDispositivo);
	ptrByte += lenNomDispositivo;

	//nombreDispositivo[lenNomDispositivo] = '\0';

	free(dataSerialized);
	return newStrKerCpu(id, action, pcb, quantum, quantumSleep, data, dataLen,
			nombreDispositivo, lenNomDispositivo);
}

/***********************************************/
/* Unserialize Kernel-UMC
 ***********************************************/
StrKerUmc* unserializeKerUmc(Stream dataSerialized) {
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	Byte* data = NULL;
	Int32U size;
	Int32U pid;
	Int32U cantPage;
	Int32U pagina;
	Int32U offset;
	Int32U tamanio;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

	memcpy(&size, ptrByte, sizeof(size));
	ptrByte += sizeof(size);
	data = malloc(size);
	memcpy(data, ptrByte, size);
	ptrByte += size;

	//data[size] = '\0';

	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);

	memcpy(&cantPage, ptrByte, sizeof(cantPage));
	ptrByte += sizeof(cantPage);

	memcpy(&pagina, ptrByte, sizeof(pagina));
	ptrByte += sizeof(pagina);

	memcpy(&offset, ptrByte, sizeof(offset));
	ptrByte += sizeof(offset);

	memcpy(&tamanio, ptrByte, sizeof(tamanio));
	ptrByte += sizeof(tamanio);

	free(dataSerialized);
	return newStrKerUmc(id, action, data, size, pid, cantPage, pagina, offset,
			tamanio);
}

/***********************************************/
/* Unserialize Kernel-Consola
 ***********************************************/
StrKerCon* unserializeKerCon(Stream dataSerialized) {
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
	log = malloc(logLen);
	memcpy(log, ptrByte, logLen);
	ptrByte += logLen;

	//log[logLen] = '\0';

	free(dataSerialized);
	return newStrKerCon(id, action, logLen, log);
}

/***********************************************/
/* Unserialize CPU-Kernel
 ***********************************************/
StrCpuKer* unserializeCpuKer(Stream dataSerialized) {
	Stream ptrByte = dataSerialized;
	Char id;
	Char action;
	pcb pcb;
	Int32U pid;
	Int32U logLen;
	Byte* log = NULL;
	Byte* nombreDispositivo = NULL;
	Int32U lenNomDispositivo;

	memcpy(&id, ptrByte, sizeof(id));
	ptrByte += sizeof(id);
	memcpy(&action, ptrByte, sizeof(action));
	ptrByte += sizeof(action);

	// comienzo a deserealizar el pcb
	memcpy(&pcb.consola, ptrByte, sizeof(pcb.consola));
	ptrByte += sizeof(pcb.consola);
	memcpy(&pcb.id, ptrByte, sizeof(pcb.id));
	ptrByte += sizeof(pcb.id);
	memcpy(&pcb.tamanioArchivoOriginal, ptrByte,
			sizeof(pcb.tamanioArchivoOriginal));
	ptrByte += sizeof(pcb.tamanioArchivoOriginal);
	memcpy(&pcb.programCounter, ptrByte, sizeof(pcb.programCounter));
	ptrByte += sizeof(pcb.programCounter);
	memcpy(&pcb.paginasDeCodigo, ptrByte, sizeof(pcb.paginasDeCodigo));
	ptrByte += sizeof(pcb.paginasDeCodigo);

	memcpy(&pcb.indiceDeCodigoSize, ptrByte, sizeof(pcb.indiceDeCodigoSize));
	ptrByte += sizeof(pcb.indiceDeCodigoSize);

	pcb.indiceDeCodigo = malloc(pcb.indiceDeCodigoSize);
	memcpy(pcb.indiceDeCodigo, ptrByte, pcb.indiceDeCodigoSize);
	ptrByte += pcb.indiceDeCodigoSize;

	memcpy(&pcb.indiceDeEtiquetasSize, ptrByte,
			sizeof(pcb.indiceDeEtiquetasSize));
	ptrByte += sizeof(pcb.indiceDeEtiquetasSize);

	memcpy(&pcb.etiquetaSize, ptrByte, sizeof(pcb.etiquetaSize));
	ptrByte += sizeof(pcb.etiquetaSize);

	pcb.indiceDeEtiquetas = malloc(pcb.etiquetaSize);
	memcpy(pcb.indiceDeEtiquetas, ptrByte, (pcb.etiquetaSize));
	ptrByte += pcb.etiquetaSize;

	//pcb.indiceDeEtiquetas[pcb.etiquetaSize] = '\0';

	memcpy(&pcb.instruccionesTotales, ptrByte,
			sizeof(pcb.instruccionesTotales));
	ptrByte += sizeof(pcb.instruccionesTotales);
	memcpy(&pcb.instruccionesRestantes, ptrByte,
			sizeof(pcb.instruccionesRestantes));
	ptrByte += sizeof(pcb.instruccionesRestantes);

	memcpy(&pcb.cantElementsStack, ptrByte, sizeof(pcb.cantElementsStack));
	ptrByte += sizeof(pcb.cantElementsStack);



	pcb.indiceDelStack = list_create();
	int i;
	int j;
	paginaDeStack* aux;
	argumentos * auxArgs;
	variables * auxVars;
	variablesRetorno * auxVarsRet;
	for (i = 0; i < pcb.cantElementsStack; i++) {
		aux = crearPaginaDeStackVaciaPiola();

		// ------------------------------------------------------------------
		memcpy(&aux->pos, ptrByte, sizeof(aux->pos));
		ptrByte += sizeof(aux->pos);

		memcpy(&aux->retPos, ptrByte, sizeof(aux->retPos));
		ptrByte += sizeof(aux->retPos);

		// ------------------------------------------------------------------

		memcpy(&aux->cantArgs, ptrByte, sizeof(aux->cantArgs));
		ptrByte += sizeof(aux->cantArgs);

		aux->args = list_create();

		for (j = 0; j < aux->cantArgs; j++) {
			auxArgs = malloc(sizeof(argumentos)); //todo

			memcpy(&auxArgs->pagArg, ptrByte, sizeof(auxArgs->pagArg));
			ptrByte += sizeof(auxArgs->pagArg);

			memcpy(&auxArgs->offArgs, ptrByte, sizeof(auxArgs->offArgs));
			ptrByte += sizeof(auxArgs->offArgs);

			memcpy(&auxArgs->sizeArgs, ptrByte, sizeof(auxArgs->sizeArgs));
			ptrByte += sizeof(auxArgs->sizeArgs);

			list_add(aux->args, auxArgs);
		}

		memcpy(&aux->cantVars, ptrByte, sizeof(aux->cantVars));
		ptrByte += sizeof(aux->cantVars);


		aux->vars = list_create();

		for (j = 0; j < aux->cantVars; j++) {
			auxVars = malloc(sizeof(variables)); //todo

			memcpy(&auxVars->idVar, ptrByte, sizeof(auxVars->idVar));
			ptrByte += sizeof(auxVars->idVar);

			memcpy(&auxVars->pagVar, ptrByte, sizeof(auxVars->pagVar));
			ptrByte += sizeof(auxVars->pagVar);

			memcpy(&auxVars->offVar, ptrByte, sizeof(auxVars->offVar));
			ptrByte += sizeof(auxVars->offVar);

			memcpy(&auxVars->sizeVar, ptrByte, sizeof(auxVars->sizeVar));
			ptrByte += sizeof(auxVars->sizeVar);

			list_add(aux->vars, auxVars);
		}

		memcpy(&aux->cantRetvars, ptrByte, sizeof(aux->cantRetvars));
		ptrByte += sizeof(aux->cantRetvars);

		aux->retVars = list_create();

		for (j = 0; j < aux->cantRetvars; j++) {
			auxVarsRet = malloc(sizeof(variablesRetorno)); //todo

			memcpy(&auxVarsRet->idVarRet, ptrByte, sizeof(auxVarsRet->idVarRet));
			ptrByte += sizeof(auxVarsRet->idVarRet);

			memcpy(&auxVarsRet->pagVarRet, ptrByte, sizeof(auxVarsRet->pagVarRet));
			ptrByte += sizeof(auxVarsRet->pagVarRet);

			memcpy(&auxVarsRet->offVarRet, ptrByte, sizeof(auxVarsRet->offVarRet));
			ptrByte += sizeof(auxVarsRet->offVarRet);

			memcpy(&auxVarsRet->sizeVarRet, ptrByte, sizeof(auxVarsRet->sizeVarRet));
			ptrByte += sizeof(auxVarsRet->sizeVarRet);

			list_add(aux->retVars, auxVarsRet);
		}



		list_add(pcb.indiceDelStack, aux);
	}



	memcpy(&pcb.estado, ptrByte, sizeof(pcb.estado));
	ptrByte += sizeof(pcb.estado);

	// termino de deserealizar el pcb

	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);

	memcpy(&logLen, ptrByte, sizeof(logLen));
	ptrByte += sizeof(logLen);
	log = malloc(logLen);
	memcpy(log, ptrByte, logLen);
	ptrByte += logLen;

	//log[logLen] = '\0';

	memcpy(&lenNomDispositivo, ptrByte, sizeof(lenNomDispositivo));
	ptrByte += sizeof(lenNomDispositivo);
	nombreDispositivo = malloc(lenNomDispositivo);
	memcpy(nombreDispositivo, ptrByte, lenNomDispositivo);
	ptrByte += lenNomDispositivo;

	//nombreDispositivo[lenNomDispositivo] = '\0';

	free(dataSerialized);
	return newStrCpuKer(id, action, pcb, pid, logLen, log, nombreDispositivo,
			lenNomDispositivo);
}

/***********************************************/
/* Unserialize CPU-UMC
 ***********************************************/
StrCpuUmc* unserializeCpuUmc(Stream dataSerialized) {
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

	// comienzo a unserializar espacioAsignado pageComienzo
	memcpy(&pageComienzo.IDPaginaInterno, ptrByte,
			sizeof(pageComienzo.IDPaginaInterno));
	ptrByte += sizeof(pageComienzo.IDPaginaInterno);

	memcpy(&pageComienzo.pid, ptrByte, sizeof(pageComienzo.pid));
	ptrByte += sizeof(pageComienzo.pid);

	memcpy(&pageComienzo.numDePag, ptrByte, sizeof(pageComienzo.numDePag));
	ptrByte += sizeof(pageComienzo.numDePag);

	memcpy(&pageComienzo.bitUso, ptrByte, sizeof(pageComienzo.bitUso));
	ptrByte += sizeof(pageComienzo.bitUso);

	memcpy(&pageComienzo.bitModificado, ptrByte,
			sizeof(pageComienzo.bitModificado));
	ptrByte += sizeof(pageComienzo.bitModificado);

	memcpy(&pageComienzo.punteroAPagina, ptrByte,
			sizeof(pageComienzo.punteroAPagina));
	ptrByte += sizeof(pageComienzo.punteroAPagina);

	memcpy(&pageComienzo.bitDePresencia, ptrByte,
			sizeof(pageComienzo.bitDePresencia));
	ptrByte += sizeof(pageComienzo.bitDePresencia);
	// termino de unserializar espacioAsignado pageComienzo

	memcpy(&offset, ptrByte, sizeof(offset));
	ptrByte += sizeof(offset);

	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);
	data = malloc(dataLen);
	memcpy(data, ptrByte, dataLen);
	ptrByte += dataLen;

	//data[dataLen] = '\0';

	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);

	free(dataSerialized);
	return newStrCpuUmc(id, action, pageComienzo, offset, dataLen, data, pid);
}

/***********************************************/
/* Unserialize UMC-Kernel
 ***********************************************/
StrUmcKer* unserializeUmcKer(Stream dataSerialized) {
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

	memcpy(&size, ptrByte, sizeof(size));
	ptrByte += sizeof(size);
	data = malloc(size);
	memcpy(data, ptrByte, size);
	ptrByte += size;

	//data[size] = '\0';

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
StrUmcCpu* unserializeUmcCpu(Stream dataSerialized) {
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

	// comienzo a unserializar espacioAsignado pageComienzo
	memcpy(&pageComienzo.IDPaginaInterno, ptrByte,
			sizeof(pageComienzo.IDPaginaInterno));
	ptrByte += sizeof(pageComienzo.IDPaginaInterno);

	memcpy(&pageComienzo.pid, ptrByte, sizeof(pageComienzo.pid));
	ptrByte += sizeof(pageComienzo.pid);

	memcpy(&pageComienzo.numDePag, ptrByte, sizeof(pageComienzo.numDePag));
	ptrByte += sizeof(pageComienzo.numDePag);

	memcpy(&pageComienzo.bitUso, ptrByte, sizeof(pageComienzo.bitUso));
	ptrByte += sizeof(pageComienzo.bitUso);

	memcpy(&pageComienzo.bitModificado, ptrByte,
			sizeof(pageComienzo.bitModificado));
	ptrByte += sizeof(pageComienzo.bitModificado);

	memcpy(&pageComienzo.punteroAPagina, ptrByte,
			sizeof(pageComienzo.punteroAPagina));
	ptrByte += sizeof(pageComienzo.punteroAPagina);

	memcpy(&pageComienzo.bitDePresencia, ptrByte,
			sizeof(pageComienzo.bitDePresencia));
	ptrByte += sizeof(pageComienzo.bitDePresencia);
	// termino de unserializar espacioAsignado pageComienzo

	memcpy(&offset, ptrByte, sizeof(offset));
	ptrByte += sizeof(offset);

	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);
	data = malloc(dataLen);
	memcpy(data, ptrByte, dataLen);
	ptrByte += dataLen;
	//data[dataLen] = '\0';

	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);

	free(dataSerialized);
	return newStrUmcCpu(id, action, pageComienzo, offset, dataLen, data, pid);
}

/***********************************************/
/* Unserialize UMC-Swap
 ***********************************************/
StrUmcSwa* unserializeUmcSwa(Stream dataSerialized) {
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

	// comienzo a unserializar espacioAsignado pageComienzo
	memcpy(&pageComienzo.IDPaginaInterno, ptrByte,
			sizeof(pageComienzo.IDPaginaInterno));
	ptrByte += sizeof(pageComienzo.IDPaginaInterno);

	memcpy(&pageComienzo.pid, ptrByte, sizeof(pageComienzo.pid));
	ptrByte += sizeof(pageComienzo.pid);

	memcpy(&pageComienzo.numDePag, ptrByte, sizeof(pageComienzo.numDePag));
	ptrByte += sizeof(pageComienzo.numDePag);

	memcpy(&pageComienzo.bitUso, ptrByte, sizeof(pageComienzo.bitUso));
	ptrByte += sizeof(pageComienzo.bitUso);

	memcpy(&pageComienzo.bitModificado, ptrByte,
			sizeof(pageComienzo.bitModificado));
	ptrByte += sizeof(pageComienzo.bitModificado);

	memcpy(&pageComienzo.punteroAPagina, ptrByte,
			sizeof(pageComienzo.punteroAPagina));
	ptrByte += sizeof(pageComienzo.punteroAPagina);

	memcpy(&pageComienzo.bitDePresencia, ptrByte,
			sizeof(pageComienzo.bitDePresencia));
	ptrByte += sizeof(pageComienzo.bitDePresencia);
	// termino de unserializar espacioAsignado pageComienzo

	memcpy(&cantPage, ptrByte, sizeof(cantPage));
	ptrByte += sizeof(cantPage);

	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);
	data = malloc(dataLen);
	memcpy(data, ptrByte, dataLen);
	ptrByte += dataLen;

	//data[dataLen] = '\0';

	memcpy(&pid, ptrByte, sizeof(pid));
	ptrByte += sizeof(pid);

	free(dataSerialized);
	return newStrUmcSwa(id, action, pageComienzo, cantPage, data, dataLen, pid);
}

/***********************************************/
/* Unserialize Swap-UMC
 ***********************************************/
StrSwaUmc* unserializeSwaUmc(Stream dataSerialized) {
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

	// comienzo de unserializar paginaAsignada pageComienzo
	memcpy(&pageComienzo.IDPaginaInterno, ptrByte,
			sizeof(pageComienzo.IDPaginaInterno));
	ptrByte += sizeof(pageComienzo.IDPaginaInterno);

	memcpy(&pageComienzo.pid, ptrByte, sizeof(pageComienzo.pid));
	ptrByte += sizeof(pageComienzo.pid);

	memcpy(&pageComienzo.bitLectura, ptrByte, sizeof(pageComienzo.bitLectura));
	ptrByte += sizeof(pageComienzo.bitLectura);

	memcpy(&pageComienzo.numDePag, ptrByte, sizeof(pageComienzo.numDePag));
	ptrByte += sizeof(pageComienzo.numDePag);
	// termino de unserializar paginaAsignada pageComienzo

	memcpy(&cantPage, ptrByte, sizeof(cantPage));
	ptrByte += sizeof(cantPage);

	memcpy(&dataLen, ptrByte, sizeof(dataLen));
	ptrByte += sizeof(dataLen);
	data = malloc(dataLen);
	memcpy(data, ptrByte, dataLen);
	ptrByte += dataLen;

	//data[dataLen] = '\0';

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
SocketBuffer* bitarrayToSocketBuffer(t_bitarray* barray) {
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
