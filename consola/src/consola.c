/*
 * consola.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

/*
 * Dependencias
 */
#include "consola.h"

void setearValores(t_config * archivoConfig) {
}

int tamArchivo(char * direccionArchivo) {

	FILE * fp;
	int tamanio;
     fp = fopen(direccionArchivo, "r");
	if (fp == NULL)
		puts("Error al abrir archivo");
	else {
		fseek(fp, 0, SEEK_END);
		tamanio = ftell(fp);
		fclose(fp);
		printf("El tamaño de %s es: %d bytes.\n\n", direccionArchivo, tamanio);
	}

	return tamanio;
}

char * leerProgramaAnSISOP(char * direccionArchivo) {

	int tamanio = tamArchivo(direccionArchivo);

	FILE * fp;

	fp = fopen(direccionArchivo, "r");

	if (fp == NULL)
		puts("Error al abrir archivo");
	else {
		char* buffer = malloc(tamanio+1);
		if(buffer == NULL){
			printf("No se pudo reservar memoria para el archivo");
			return FALSE;
		}
		fseek(fp, 0, 0);
		int n = 0;
		while (!feof(fp)) {
			buffer[n] = getc(fp);
			n++;
		}
		buffer[n-1] = '\0';

		fclose(fp);
		return buffer;

	}
	return FALSE;
}
