/*
 * swap.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#include "swap.h"

#define manejarError(msg) {perror(msg); abort();}


//Genero mi archivo de Swap y lo devuelvo mappeado en memoria DRMENGUECHE
char* crearArchivo(char* tamanio,char* nombre){
	char* paraSistema=string_new();
	string_append(&paraSistema,"dd if=/dev/zero of=");
	string_append(&paraSistema,nombre);
	string_append(&paraSistema," bs=");
	string_append(&paraSistema,tamanio);
	string_append(&paraSistema," count=1");
	system(paraSistema);
	return mappearArchivo(nombre);

}

// mappear el archivoo DRMENGUECHE

void* mappearArchivo(char* filename) {

	char *addr;
	int fd;
	struct stat sb;
	size_t length;

	fd = open(filename, O_RDWR);

	length = sb.st_size;
	addr = mmap(NULL, length, PROT_READ | PROT_WRITE,MAP_SHARED | MAP_NORESERVE, fd, 0);

	return addr;

}



void setearValores(t_config * archivoConfig) {
		umcPort = config_get_string_value(archivoConfig, "PUERTO_UMC");
		nombreSwap = config_get_string_value(archivoConfig, "NOMBRE_SWAP");
		paginas = config_get_string_value(archivoConfig, "CANTIDAD_PAGINAS");
		tamPagina = config_get_string_value(archivoConfig, "TAMANO_PAGINA");
		retCompactacion = config_get_string_value(archivoConfig, "RETARDO_COMPACTACION");
		tamArchivo=config_get_string_value(archivoConfig, "TAMANO_ARCHIVO");
}

