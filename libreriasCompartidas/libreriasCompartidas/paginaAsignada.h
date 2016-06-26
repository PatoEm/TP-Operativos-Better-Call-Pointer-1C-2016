/*
 * paginaAsignada.h
 *
 *  Created on: 26/6/2016
 *      Author: utnso
 */

#ifndef LIBRERIASCOMPARTIDAS_PAGINAASIGNADA_H_
#define LIBRERIASCOMPARTIDAS_PAGINAASIGNADA_H_

#include <stdbool.h>

typedef struct {
	int IDPaginaInterno;
	int pid;
	bool bitLectura;
	int numDePag;

} paginaAsignada;

#endif /* LIBRERIASCOMPARTIDAS_PAGINAASIGNADA_H_ */
