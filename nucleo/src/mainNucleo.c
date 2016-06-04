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
int main(void) {

	inicializarVariables();
	puts("Ya inicialice");

//Creacion de Hilos

	 pthread_t hiloQuantum;
	 pthread_attr_t attrHiloQuantum;
	 pthread_attr_init(&attrHiloQuantum);
	 pthread_attr_setdetachstate(&attrHiloQuantum, PTHREAD_CREATE_DETACHED);
	 pthread_create(&hiloQuantum, &attrHiloQuantum, &funcionHiloQuantum, NULL);
	 pthread_attr_destroy(&attrHiloQuantum);


	//Se conecta a la consola. COMENTEMOS QUE HACEMOS

/*

 	int fdSocketConsola = crearSocketServidor(puertoPropio);
	//int fdSocketConsola = crearSocketServidor("6002");
	escucharSocket(fdSocketConsola,5);
	int fdSocketCliente1= aceptarConexiones(fdSocketConsola);

	char* mensaje;
	mensaje=(char*)malloc(11*sizeof(char));
	if(mensaje==NULL){
		puts("falló al reservar la memoria");
		return -1;
	}
	recibirMensaje(fdSocketCliente1,mensaje,11*sizeof(char));
	puts(mensaje);

	// creo el servidor para el cpu y lo acepto. Author: Dr.Mengueche

	int fdSocketCpu= crearSocketServidor(cpuPort);
	escucharSocket(fdSocketCpu,5);
	int fdSocketClienteCpu1=aceptarConexiones(fdSocketCpu);
	enviarMensaje(fdSocketClienteCpu1,mensaje,11*sizeof(char));

	//Se conecta con la memoria. Author: Dr.Mengueche
	//Socket UMC testeado by Dr.Mengueche
	int fdSocketUMC=crearCliente(ipUMC,UMCPort);

*/

	/*
	puts("hola");
	char * mensajito = leerProgramaAnSISOP1("/home/utnso/tp-2016-1c-Better-call-pointer/consola/programasEjemplo/stackoverflow.ansisop");
	//t_metadata_program * programa1=metadata_desde_literal(mensajito);
	puts("hola1");
	int tamanio= tamArchivo("/home/utnso/tp-2016-1c-Better-call-pointer/consola/programasEjemplo/stackoverflow.ansisop");
	puts("hola2");
	pcb hola = crearNuevoPcb(mensajito,tamanio);
	puts("hola3");
	*/

	//Destruyo semaforos sincro
	//pthread_mutex_destroy(&mutexQuantum);
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

	int tamanio=tamArchivo(direccionArchivo);

	FILE * fp;

	fp = fopen(direccionArchivo, "r");

	if (fp == NULL)
		puts("Error al abrir archivo");
	else {
		char* buffer = (char *) malloc(tamanio);
			if(buffer==NULL){
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




