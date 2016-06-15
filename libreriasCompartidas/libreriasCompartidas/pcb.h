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

typedef struct {
	int comienzo;
	int longitud;
} arrayBidimensional;

typedef struct {
	int id;
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
	int id;
	int tamanioArchivoOriginal;
	t_puntero_instruccion programCounter;
	int paginasDeCodigo;
	arrayBidimensional * indiceDeCodigo;
	char * indiceDeEtiquetas;
	paginaDeStack * indiceDelStack;
	t_medatada_program* metaProgram;
	estadoPrograma estado;

} pcb;

pcb* newPcb(int id, int tamanioArchivoOriginal, t_puntero_instruccion programCounter,
		int paginasDeCodigo, arrayBidimensional* indiceDeCodigo, char* indiceDeEtiquetas,
		paginaDeStack* indiceDelStack, t_metadata_program* metaProgram, estadoPrograma estado);
pcb* newEmptyPcb();

char * pedirCodigoAUMC();
#endif /* LIBRERIASCOMPARTIDAS_PCB_H_ */
