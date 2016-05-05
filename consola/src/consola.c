/*
 * consola.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */
#include "consola.h"
//#define manejarError(msg) {perror(msg); abort();}



void setearValores(t_config * archivoConfig) {
		ipNucleo = config_get_string_value(archivoConfig, "IP_NUCLEO");
		nucleoPort= config_get_string_value(archivoConfig, "PUERTO_NUCLEO");
}
long tamanioArchivo(char * direccionArchivo) {

	FILE * fp;

     fp = fopen(direccionArchivo, "r");
	if (fp == NULL)
		puts("Error al abrir archivo");
	else {
		fseek(fp, 0, SEEK_END);
		tamanio = ftell(fp);
		fclose(fp);
		printf("El tamaño de %s es: %ld bytes.\n\n", direccionArchivo, tamanio);
	}

	return tamanio;
}
 char * leerProgramaAnsisop(char * direccionArchivo) {

	FILE * fp;

	fp = fopen(direccionArchivo, "r");

	if (fp == NULL)
		puts("Error al abrir archivo");
	else {
		 buffer = (char *) malloc(tamanio);
			if(buffer==NULL){
				printf("no se pudo reservar memoria para el archivo");
				return-1;
			}
		 fseek(fp, 0, 0);
		int n = 0;
		while (!feof(fp)) {
			buffer[n] = getc(fp);
			n++;
		}
		buffer[n - 1] = '\0';
		fclose(fp);
		printf("%s", buffer);
		return buffer;

	}
	return "";

}



