/*
 * espacioAsignado.h
 *
 *  Created on: 23/6/2016
 *      Author: utnso
 */

#ifndef LIBRERIASCOMPARTIDAS_ESPACIOASIGNADO_H_
#define LIBRERIASCOMPARTIDAS_ESPACIOASIGNADO_H_

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
} espacioAsignado;

#endif /* LIBRERIASCOMPARTIDAS_ESPACIOASIGNADO_H_ */
