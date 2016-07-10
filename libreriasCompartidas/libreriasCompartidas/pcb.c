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
		int paginasDeCodigo, arrayBidimensional* indiceDeCodigo, char* indiceDeEtiquetas, t_size sizeEti,t_size insTotales,t_size insRestantes,
		t_list* indiceDelStack, t_metadata_program* metaProgram, estadoPrograma estado) {

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
	pcb->indiceDelStack = indiceDelStack;
	pcb->metaProgram = metaProgram;
	pcb->estado = estado;

	return pcb;
}

pcb* newEmptyPcb(){
	return newPcb(NULL, 0, 0, 0, 0, 0,0, 0,0,NULL, NULL, NULL, 0);
}

