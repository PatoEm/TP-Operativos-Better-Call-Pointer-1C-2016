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

	int tamanio=tamArchivo(direccionArchivo);

	FILE * file;

	file = fopen(direccionArchivo, "r");
	puts("Leyendo el archivo");
	if (!file) {
		puts("[ERROR]: No se pudo abrir el archivo.");
		return FALSE;
	} else {
		char* buffer = (char *) malloc(tamanio);
		if(buffer == NULL){
			printf("no se pudo reservar memoria para el archivo");
			return FALSE;
			}
		fseek(file, 0, 0);
		int n = 0;
		while (!feof(file)) {
			buffer[n] = getc(file);
			n++;
		}
		buffer[n-1] = '\0';
		fclose(file);
		printf("%s", buffer);
		return buffer;
	}
	return "";

}
