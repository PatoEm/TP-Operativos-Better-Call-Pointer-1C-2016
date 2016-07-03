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


void enviarArchivoANucleo(char*direccion){

	char* paqueteAEnviar;
	puts("hola");
	//Obtengo el tamaño
	int elTamanio=tamArchivo(direccion);

	//Obtengo el contMienido del archivo
	char* buffer=leerProgramaAnSISOP(direccion);
	char protocolo[3]="00";
	paqueteAEnviar=(char* )malloc(elTamanio+sizeof(char)*2+sizeof(int));
	memcpy(paqueteAEnviar,&protocolo,sizeof(char)*2);
	memcpy(paqueteAEnviar[2],&elTamanio,4);
	memcpy(paqueteAEnviar[5],buffer,elTamanio);
	enviarMensaje(fdSocketConsola,paqueteAEnviar,elTamanio+sizeof(char)*2+sizeof(int));
	free(paqueteAEnviar);
	free(protocolo);
	free(buffer);

}

void recibirArchivo(){

	char* direccionDeArchivo = (char *) malloc(150);
	verificarMemoria(direccionDeArchivo);

	printf("Ingrese archivo: ");
    scanf("%s",direccionDeArchivo);
    enviarArchivoANucleo(direccionDeArchivo);

}

void setearValores(t_config * archivoConfig) {
		//ipNucleo = config_get_string_value(archivoConfig, "IP_NUCLEO");
		//nucleoPort= config_get_string_value(archivoConfig, "PUERTO_NUCLEO");
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

	int tamanio=tamArchivo(direccionArchivo);

	FILE * fp;

	fp = fopen(direccionArchivo, "r");

	if (fp == NULL)
		puts("Error al abrir archivo");
	else {
		char* buffer = (char *) malloc(tamanio);
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



