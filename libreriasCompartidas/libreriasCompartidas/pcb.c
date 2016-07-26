/*
 * pcb.c
 *
 *  Created on: 9/6/2016
 *      Author: utnso
 */

#include "pcb.h"

void moverProgramCounterPcb(pcb * pcbLoca) {
	pcbLoca->programCounter++;
	pcbLoca->instruccionesRestantes--;

}

pcb* newPcb(Socket* consola,Socket* cpu, int id, int tamanioArchivoOriginal, t_puntero_instruccion programCounter, int paginasDeCodigo,
	arrayBidimensional* indiceDeCodigo, char* indiceDeEtiquetas, t_size indiceDeEtiquetasSize, t_size sizeEti, t_size insTotales,
	t_size insRestantes, t_list* indiceDelStack, estadoPrograma estado, t_size indiceDeCodigoSize, t_size cantElementsStack) {

		pcb* pcb = malloc(sizeof(pcb));

		pcb->consola = consola;
		pcb->cpu = cpu;
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
		pcb->cantElementsStack = cantElementsStack;
		return pcb;
}

paginaDeStack*crearPaginaDeStackVaciaPiola(){
	paginaDeStack* soyUnaPaginaDeStackVacia = malloc(sizeof (paginaDeStack));
	soyUnaPaginaDeStackVacia->pos = 0;
	soyUnaPaginaDeStackVacia->retPos = 0;
	soyUnaPaginaDeStackVacia->cantArgs = 0;
	soyUnaPaginaDeStackVacia->cantVars = 0;
	soyUnaPaginaDeStackVacia->cantRetvars = 0;
	soyUnaPaginaDeStackVacia->vars = list_create();
	soyUnaPaginaDeStackVacia->retVars = list_create();
	soyUnaPaginaDeStackVacia->args = list_create();
	return soyUnaPaginaDeStackVacia;
}


argumentos*crearArgumentoVacioPiola(){
	argumentos* soyUnArgumentoVacio = malloc(sizeof(argumentos));
	soyUnArgumentoVacio->pagArg = 0;
	soyUnArgumentoVacio->offArgs = 0;
	soyUnArgumentoVacio->sizeArgs = 0;
	return soyUnArgumentoVacio;
}

variables*crearVariableVacioPiola(){
	variables* soyUnaVariableVacia = malloc(sizeof(variables));
	soyUnaVariableVacia->idVar = 0;
	soyUnaVariableVacia->pagVar = 0;
	soyUnaVariableVacia->offVar = 0;
	soyUnaVariableVacia->sizeVar = 0;
	return soyUnaVariableVacia;
}

variablesRetorno*crearVariableRetornoVacioPiola(){
	variablesRetorno* soyUnaVariableRetornoVacia = malloc(sizeof(variablesRetorno));
	soyUnaVariableRetornoVacia->idVarRet = 0;
	soyUnaVariableRetornoVacia->pagVarRet = 0;
	soyUnaVariableRetornoVacia->offVarRet = 0;
	soyUnaVariableRetornoVacia->sizeVarRet = 0;
	return soyUnaVariableRetornoVacia;
}

pcb* newEmptyPcb() {

	pcb* pcbLoco = malloc(sizeof(pcb));
	//Socket * auxSocket = socketCreate();
	pcbLoco->consola = socketCreate();
	pcbLoco->cpu = socketCreate();

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

	paginaDeStack * aux = crearPaginaDeStackVaciaPiola();


	list_add(pcbLoco->indiceDelStack, aux);

	paginaDeStack * aux2 = crearPaginaDeStackVaciaPiola();


	list_add(pcbLoco->indiceDelStack, aux2);

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
	//free(auxIndice);
	puts("Cree un pcb Vacio");

	return pcbLoco;
}

