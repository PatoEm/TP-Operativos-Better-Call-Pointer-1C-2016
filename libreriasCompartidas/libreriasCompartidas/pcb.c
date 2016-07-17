/*
 * pcb.c
 *
 *  Created on: 9/6/2016
 *      Author: utnso
 */

#include "pcb.h"

void moverProgramCounterPcb(pcb * pcbLoca) {
//int i;
//	for (i = 0; i < pcbLoca->instruccionesTotales; ++i) {
//		if((pcbLoca->indiceDeCodigo[i].comienzo)==(pcbLoca->programCounter)){
//			pcbLoca->programCounter=(pcbLoca->indiceDeCodigo[i].comienzo)+(pcbLoca->indiceDeCodigo[i].longitud)+1;
//			pcbLoca->instruccionesRestantes-=1;
//		}
//	}

	pcbLoca->programCounter++;

}

pcb* newPcb(Socket* consola, int id, int tamanioArchivoOriginal,
		t_puntero_instruccion programCounter, int paginasDeCodigo,
		arrayBidimensional* indiceDeCodigo, char* indiceDeEtiquetas,
		t_size indiceDeEtiquetasSize, t_size sizeEti, t_size insTotales,
		t_size insRestantes, t_list* indiceDelStack, estadoPrograma estado,
		t_size indiceDeCodigoSize, Byte* buffer, t_size cantElementsStack) {

	pcb* pcb = malloc(sizeof(pcb));

	pcb->consola = consola;
	pcb->id = id;
	pcb->tamanioArchivoOriginal = tamanioArchivoOriginal;
	pcb->programCounter = programCounter;
	pcb->paginasDeCodigo = paginasDeCodigo;
	pcb->indiceDeCodigo = indiceDeCodigo;
	pcb->etiquetaSize = sizeEti;
	pcb->instruccionesTotales = insTotales;
	pcb->instruccionesRestantes = insRestantes;
	pcb->indiceDeEtiquetas = indiceDeEtiquetas;
	pcb->indiceDeEtiquetasSize = indiceDeEtiquetasSize;
	pcb->indiceDelStack = indiceDelStack;
	pcb->estado = estado;
	pcb->indiceDeCodigoSize = indiceDeCodigoSize;
	pcb->buffer = buffer;
	pcb->cantElementsStack = cantElementsStack;

	return pcb;
}

paginaDeStack*crearPaginaDeStackVaciaPiola(){
	paginaDeStack* soyUnaPaginaDeStackVacia= malloc(sizeof (paginaDeStack));
	soyUnaPaginaDeStackVacia->pos=0;
	soyUnaPaginaDeStackVacia->args.pag=0;
	soyUnaPaginaDeStackVacia->args.off=0;
	soyUnaPaginaDeStackVacia->args.size=0;
	soyUnaPaginaDeStackVacia->retPos=0;
	soyUnaPaginaDeStackVacia->vars.id='a';
	soyUnaPaginaDeStackVacia->vars.off=0;
	soyUnaPaginaDeStackVacia->vars.pag=0;
	soyUnaPaginaDeStackVacia->vars.size=0;
	soyUnaPaginaDeStackVacia->retVars.id='2';
	soyUnaPaginaDeStackVacia->retVars.off=0;
	soyUnaPaginaDeStackVacia->retVars.pag=0;
	soyUnaPaginaDeStackVacia->retVars.size=0;
	return soyUnaPaginaDeStackVacia;
}

pcb* newEmptyPcb() {

	pcb* pcbLoco = malloc(sizeof(pcb));
	//Socket * auxSocket = socketCreate();
	pcbLoco->consola = socketCreate();

	pcbLoco->id = 0;
	pcbLoco->tamanioArchivoOriginal = 0;
	pcbLoco->programCounter = 0;
	pcbLoco->paginasDeCodigo = 0;

	pcbLoco->indiceDeCodigoSize = (sizeof(arrayBidimensional));
	pcbLoco->indiceDeEtiquetas = malloc(sizeof(char) * 17);
	//char * auxIndice = malloc(sizeof(char)*17);
	char * auxIndice;
	auxIndice = "MUERTE A WINDOWS";
	memcpy(pcbLoco->indiceDeEtiquetas, auxIndice, 17);
	pcbLoco->indiceDeEtiquetasSize = 17;
	pcbLoco->etiquetaSize = 0;
	pcbLoco->instruccionesRestantes = 0;
	pcbLoco->instruccionesTotales = 1;
	pcbLoco->indiceDeCodigo = sizeof(arrayBidimensional)
			* pcbLoco->instruccionesTotales;
	pcbLoco->indiceDelStack = list_create();

	paginaDeStack * aux = malloc(sizeof(paginaDeStack));
	aux->pos = 1;

	aux->args.pag = 2;
	aux->args.off = 3;
	aux->args.size = 4;

	aux->vars.id = 'a';
	aux->vars.pag = 5;
	aux->vars.off = 6;
	aux->vars.size = 7;

	aux->retPos = 8;
	aux->retVars.id = 'b';
	aux->retVars.pag = 9;
	aux->retVars.off = 10;
	aux->retVars.size = 11;

	list_add(pcbLoco->indiceDelStack, aux);

	paginaDeStack * aux2 = malloc(sizeof(paginaDeStack));
	aux2->pos = 12;

	aux2->args.pag = 13;
	aux2->args.off = 14;
	aux2->args.size = 14;

	aux2->vars.id = 'c';
	aux2->vars.pag = 15;
	aux2->vars.off = 16;
	aux2->vars.size = 17;

	aux2->retPos = 18;
	aux2->retVars.id = 'd';
	aux2->retVars.pag = 19;
	aux2->retVars.off = 20;
	aux2->retVars.size = 21;
	list_add(pcbLoco->indiceDelStack, aux2);

	pcbLoco->buffer = "Empty";
	pcbLoco->cantElementsStack = 2;

	pcbLoco->estado = NEW;

	arrayBidimensional * auxArray = (arrayBidimensional*) malloc(
			sizeof(arrayBidimensional));
	//arrayBidimensional auxArray;
	auxArray[0].comienzo = 0;
	auxArray[0].longitud = 0;
	pcbLoco->indiceDeCodigo = (arrayBidimensional*) malloc(
			sizeof(arrayBidimensional));
	memcpy(&(pcbLoco->indiceDeCodigo[0]), &auxArray[0],
			sizeof(arrayBidimensional));

	//free(auxArray);
	//free(auxSocket);
//	free(auxIndice);
	puts("Cree un pcb Vacio");

	return pcbLoco;
}

