
#ifndef LIBRERIASCOMPARTIDAS_ARCHIVOSYLOGSYMAS_H_
#define LIBRERIASCOMPARTIDAS_ARCHIVOSYLOGSYMAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/temporal.h>
#include <commons/config.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/config.h>
#define FAIL -1

t_log * logger;

int verificarMemoria(void*algo);

void leerArchivoDeConfiguracion(const char * direccionArchivo);

int verificarExistenciaDeArchivo(char* rutaArchivoConfig);

void setearValores(t_config * archivoConfig);

void crearLogger(int );

int cantidadPaginas (int tamanioArchivo, int tamanioPagina);

#endif /* LIBRERIASCOMPARTIDAS_ARCHIVOSYLOGSYMAS_H_ */
