/*
 * espacioAsignado.h
 *
 *  Created on: 23/6/2016
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	int IDPaginaInterno;
	int pid;
	int numDePag;
	int bitUso;
	int bitModificado;
	bool punteroAPagina;
}espacioAsignado;
