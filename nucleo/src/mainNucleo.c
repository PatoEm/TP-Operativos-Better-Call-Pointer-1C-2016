/*
 ============================================================================
 Name        : nucleo.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "nucleo.h"

char * leerProgramaAnSISOP1(char * direccionArchivo);

// MAIN FUNCHION

int main(void) {

//	pcb * pcbVacioLoco=newEmptyPcb();
//	StrKerCpu *hola = newStrKerCpu(KERNEL_ID,PRIMER_PCB,*pcbVacioLoco,8,"hola",0,"hola",0);
//	SocketBuffer *bufferLoco =serializeKerCpu(hola);
//	bufferLoco->data;
//	hola=unserializeKerCpu(bufferLoco->data);
//
//	paginaDeStack * paginita =list_get(hola->pcb.indiceDelStack,0);
//	paginaDeStack * paginita2 =list_get(hola->pcb.indiceDelStack,1);

<<<<<<< HEAD
	inicializarVariables();
	printf("%d", tamanioPaginas);
=======
	paginaDeStack paginita;
	paginita.pos=0;
	//paginita.

>>>>>>> branch 'master' of https://github.com/sisoputnfrba/tp-2016-1c-Better-call-pointer.git

	//int tamanioLoco = tamArchivo("/home/utnso/tp-2016-1c-Better-call-pointer/consola/programasEjemplo/facil.ansisop");
	//printf("%d",tamanioLoco);
	log_info(nucleolog, "Variables inicializadas.");

	/**************************************************/
	//Creacion de Hilos
	crearHilos();
	log_info(nucleolog, "Se han creado los threads.");

	/**************************************************/

	/* ESTE WAIT ES PARA QUE SIGAN CORRIENDO LOS HILOS */
	while (1) {
		sleep(5);
	}
	/***************************************************/

	log_info(nucleolog, "Termine re piola.");
	return EXIT_SUCCESS;
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

char * leerProgramaAnSISOP1(char * direccionArchivo) {

	int tamanio = tamArchivo(direccionArchivo);

	FILE * fp;

	fp = fopen(direccionArchivo, "r");

	if (fp == NULL)
		puts("Error al abrir archivo");
	else {
		char* buffer = (char *) malloc(tamanio);
		if (buffer == NULL) {
			printf("no se pudo reservar memoria para el archivo");
			//return -1;
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
