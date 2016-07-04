/*
 * pcb.c
 *
 *  Created on: 9/6/2016
 *      Author: utnso
 */

#include "pcb.h"


pcb* newPcb(int id, int tamanioArchivoOriginal, t_puntero_instruccion programCounter,
		int paginasDeCodigo, arrayBidimensional* indiceDeCodigo, char* indiceDeEtiquetas, t_size sizeEti,
		t_list* indiceDelStack, t_metadata_program* metaProgram, estadoPrograma estado) {

	pcb* pcb = malloc(sizeof(pcb));

	pcb->id = id;
	pcb->tamanioArchivoOriginal = tamanioArchivoOriginal;
	pcb->programCounter = programCounter;
	pcb->paginasDeCodigo = paginasDeCodigo;
	pcb->indiceDeCodigo = indiceDeCodigo;
	pcb->etiquetaSize= sizeEti;
	pcb->indiceDeEtiquetas = indiceDeEtiquetas;
	pcb->indiceDelStack = indiceDelStack;
	pcb->metaProgram = metaProgram;
	pcb->estado = estado;

	return pcb;
}

pcb* newEmptyPcb(){
	return newPcb(0, 0, 0, 0, 0, 0,NULL, NULL, NULL, 0);
}

