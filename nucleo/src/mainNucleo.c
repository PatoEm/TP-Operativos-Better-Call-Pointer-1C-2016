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

	//Inicio el Contador de programas en 0 y creo las Colas
	idProgramas=0;
	colaNew = queue_create();
	colaReady= queue_create();
	colaExec= queue_create();
	colaBlock= queue_create();
	colaExit= queue_create();
	leerArchivoDeConfiguracion("confignucleo");


	//lee archivo de configuración y se conecta a la consola. COMENTEMOS QUE HACEMOS



	puts("Inicio Nucleo");

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

	//t_metadata_program programa1 = metadata_desde_literal(mensaje);






	// creo el servidor para el cpu y lo acepto. Author: Dr.Mengueche

	int fdSocketCpu= crearSocketServidor(cpuPort);
	escucharSocket(fdSocketCpu,5);
	int fdSocketClienteCpu1=aceptarConexiones(fdSocketCpu);





	enviarMensaje(fdSocketClienteCpu1,mensaje,11*sizeof(char));





	//Se conecta con la memoria. Author: Dr.Mengueche
	//Socket UMC testeado by Dr.Mengueche
	int fdSocketUMC=crearCliente(ipUMC,UMCPort);

*/
	char * mensajito = leerProgramaAnSISOP1("/home/utnso/tp-2016-1c-Better-call-pointer/consola/programasEjemplo/stackoverflow.ansisop");
	t_metadata_program * programa1=metadata_desde_literal(mensajito);



	puts("hola");
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




