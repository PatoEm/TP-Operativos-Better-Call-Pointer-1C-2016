/*
 * pcb.c
 *
 *  Created on: 9/6/2016
 *      Author: utnso
 */

#include "pcb.h"

void moverProgramCounterPcb(pcb * pcbLoca){
int i;
	for (i = 0; i < pcbLoca->instruccionesTotales; ++i) {
		if((pcbLoca->indiceDeCodigo[i].comienzo)==(pcbLoca->programCounter)){
			pcbLoca->programCounter=(pcbLoca->indiceDeCodigo[i].comienzo)+(pcbLoca->indiceDeCodigo[i].longitud)+1;
			pcbLoca->instruccionesRestantes-=1;
		}
	}
	pcbLoca->indiceDeCodigo[1].comienzo;
}



pcb* newPcb(Socket* consola, int id, int tamanioArchivoOriginal, t_puntero_instruccion programCounter,
		int paginasDeCodigo, arrayBidimensional* indiceDeCodigo, char* indiceDeEtiquetas,t_size indiceDeEtiquetasSize ,t_size sizeEti,t_size insTotales,t_size insRestantes,
		t_list* indiceDelStack, estadoPrograma estado) {

	pcb* pcb = malloc(sizeof(pcb));

	pcb->consola = consola;
	pcb->id = id;
	pcb->tamanioArchivoOriginal = tamanioArchivoOriginal;
	pcb->programCounter = programCounter;
	pcb->paginasDeCodigo = paginasDeCodigo;
	pcb->indiceDeCodigo = indiceDeCodigo;
	pcb->etiquetaSize= sizeEti;
	pcb->instruccionesTotales=insTotales;
	pcb->instruccionesRestantes=insRestantes;
	pcb->indiceDeEtiquetas = indiceDeEtiquetas;
	pcb->indiceDeEtiquetasSize=indiceDeEtiquetasSize;
	pcb->indiceDelStack = indiceDelStack;
	pcb->estado = estado;

	return pcb;
}

pcb* newEmptyPcb(){

	pcb* pcbLoco=malloc(sizeof(pcb));
	//Socket * auxSocket = socketCreate();
	arrayBidimensional * auxArray = malloc(sizeof(arrayBidimensional));
	auxArray[0].comienzo=0;
	auxArray[0].longitud=0;
	pcbLoco->consola=socketCreate();

	//memcpy(pcbLoco->consola,auxSocket,sizeof(Socket));
	pcbLoco->id=0;
	pcbLoco->tamanioArchivoOriginal=0;
	pcbLoco->programCounter=0;
	pcbLoco->paginasDeCodigo=0;
	pcbLoco->indiceDeCodigo=(arrayBidimensional*)malloc(sizeof(arrayBidimensional));
	memcpy(&(pcbLoco->indiceDeCodigo[0]),&auxArray[0],sizeof(arrayBidimensional));
	pcbLoco->indiceDeEtiquetas=malloc(sizeof(char)*17);
	//char * auxIndice = malloc(sizeof(char)*17);
	char * auxIndice;
	auxIndice="MUERTE A WINDOWS";
	memcpy(pcbLoco->indiceDeEtiquetas,auxIndice, 17);
	pcbLoco->indiceDeEtiquetasSize=17;
	pcbLoco->etiquetaSize=0;
	pcbLoco->instruccionesRestantes=0;
	pcbLoco->instruccionesTotales=1;

	pcbLoco->estado=NEW;
	free(auxArray);
	//free(auxSocket);
//	free(auxIndice);
	return pcbLoco;
}

