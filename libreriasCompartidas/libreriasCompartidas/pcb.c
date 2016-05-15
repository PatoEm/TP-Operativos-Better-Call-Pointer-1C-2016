/*
 * pcb.c
 *
 *  Created on: 15/5/2016
 *      Author: utnso
 */
#include "pcb.h"

pcb rearmarPcb(int id1, int tamanioArOr, t_puntero_instruccion pCounter, int pagCod,
		   arrayBidimensional * indiceCodigo, char * indiceEtiquetas, t_metadata_program* meta, estadoPrograma est){
	pcb pcbRearmada;

	pcbRearmada.id=id1;
	pcbRearmada.tamanioArchivoOriginal=tamanioArOr;
	pcbRearmada.programCounter=pCounter;
	pcbRearmada.paginasDeCodigo=pagCod;
	pcbRearmada.indiceDeCodigo=indiceCodigo;
	pcbRearmada.indiceDeEtiquetas=indiceEtiquetas;
	pcbRearmada.estado=est;

	return pcbRearmada;
}
