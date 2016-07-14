/*
 * pcb.h
 *
 *  Created on: 6/6/2016
 *      Author: utnso
 */

#ifndef LIBRERIASCOMPARTIDAS_PCB_H_
#define LIBRERIASCOMPARTIDAS_PCB_H_
#include <parser/metadata_program.h>
#include <parser/parser.h>
#include <commons/collections/list.h>
#include "pointerSocketes.h"

typedef struct {
	int comienzo;
	int longitud;
} arrayBidimensional;

typedef struct {
	char id;
	int pag;
	int off;
	int size;
} variables;
typedef struct {
	int pag;
	int off;
	int size;
} argumentos;

typedef struct {
	int pos;
	argumentos args;
	variables vars;
	int retPos;
	variables retVars;

} paginaDeStack;

typedef enum {
	NEW = 0, READY = 1, EXEC = 2, BLOCK = 3, EXIT = 4
} estadoPrograma;

typedef struct {
	Socket* consola;
	int id;
	int tamanioArchivoOriginal;
	t_puntero_instruccion programCounter;
	int paginasDeCodigo;
	arrayBidimensional * indiceDeCodigo;
	char * indiceDeEtiquetas;
	t_size etiquetaSize;
	t_size instruccionesTotales;
	t_size instruccionesRestantes;
	t_list* indiceDelStack;
	estadoPrograma estado;


} pcb;

pcb* newPcb(Socket* consola, int id, int tamanioArchivoOriginal, t_puntero_instruccion programCounter,
		int paginasDeCodigo, arrayBidimensional* indiceDeCodigo, char* indiceDeEtiquetas,t_size etiquetas,t_size instruccionesTotales,t_size instruccionesRestantes,
		t_list* indiceDelStack, estadoPrograma estado);
pcb* newEmptyPcb();

char * pedirCodigoAUMC();

void moverProgramCounterPcb(pcb * pcbLoca);

#endif /* LIBRERIASCOMPARTIDAS_PCB_H_ */
