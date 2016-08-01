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


/*
Este tp fue aprobado con huevos, con ganas, con esfuerzo y mil bardos.
Todos dimos mucho y perdimos mucho.
En particular fue un cuatrimestre muy jodido para mi.
Perdí demasiadas cosas... gente que amaba principalmente, a uno me lo llevo una enfermedad
y la otra persona simplemente decidio irse cuando mas la necesite.
Gracias a esa gente que se nos fue en el tp y nos ayudo desde donde estén ahora
Y a la gente que no nos banco, que nos abandono y nos dejo solos en sus caras forros/as.
Gracias por esta cursada chicos. Los quiero mucho.
Att: The Debugger.
*/


char * leerProgramaAnSISOP1(char * direccionArchivo);

// MAIN FUNCHION

int main(int argc, char** argv) {

//	pcb * pcbVacioLoco=newEmptyPcb();
//	StrKerCpu *hola = newStrKerCpu(KERNEL_ID,PRIMER_PCB,*pcbVacioLoco,8,"hola",0,"hola",0);
//	SocketBuffer *bufferLoco =serializeKerCpu(hola);
//	bufferLoco->data;
//	hola=unserializeKerCpu(bufferLoco->data);
//
//	paginaDeStack * paginita =list_get(hola->pcb.indiceDelStack,0);
//	paginaDeStack * paginita2 =list_get(hola->pcb.indiceDelStack,1);
//
//



	if(argc != 2){
		puts("No se ingreso la ruta del archivo de configuracion\n");
		return 0;
	}

	//memcpy(rutaArchivoConfig,&*argv[1],strlen(argv[1]));

	rutaArchivoConfig=argv[1];

	inicializarVariables(rutaArchivoConfig);
	printf("%d", tamanioPaginas);

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
