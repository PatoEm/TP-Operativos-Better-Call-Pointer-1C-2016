/*
 * pcb.h
 *
 *  Created on: 15/5/2016
 *      Author: utnso
 */

#ifndef LIBRERIASCOMPARTIDAS_PCB_H_
#define LIBRERIASCOMPARTIDAS_PCB_H_
#include <stdio.h>
#include <stdlib.h>
#include <parser/metadata_program.h>
#include <parser/parser.h>

//Estructuras PCB
   typedef struct{
   	int comienzo;
   	int longitud;
   }arrayBidimensional;

   typedef struct {
 	   int id;
 	   int pag;
 	   int off;
 	   int size;
    }variables;
    typedef struct{
 	   int pag;
 	   int off;
 	   int size;
    }argumentos;

    typedef struct {
 	   int pos;
 	   argumentos args;
 	   variables vars;
 	   int retPos;
 	   variables retVars;

    }indiceDeStack;

   typedef enum {
        NEW=0,
        READY=1,
        EXEC=2,
		BLOCK=3,
        EXIT=4
    }estadoPrograma;


   typedef struct{
   	int id;
   	int tamanioArchivoOriginal;
   	t_puntero_instruccion programCounter;
   	int paginasDeCodigo;
   	arrayBidimensional * indiceDeCodigo;
   	char * indiceDeEtiquetas;
   	indiceDeStack * indiceDelStack;
   	t_medatada_program* metaProgram;
   	estadoPrograma estado;

   }pcb ;

   pcb rearmarPcb(int id1, int tamanioArOr, t_puntero_instruccion pCounter, int pagCod,
		   arrayBidimensional * indiceCodigo, char * indiceEtiquetas, t_metadata_program* meta, estadoPrograma est);


  // typedef pcb pcb;
   //typedef arrayBidimensional arrayBidimensional;

#endif /* LIBRERIASCOMPARTIDAS_PCB_H_ */
