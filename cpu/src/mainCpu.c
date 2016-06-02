/*
 ============================================================================
 Name        : cpu.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "cpu.h"

void* nuevoCore(void* argumento);

int main() {


	leerArchivoDeConfiguracion("configcpu");


//	IMPLEMENTACION DE PRUEBA

	pthread_t nuevoHilo;
	t_list* coreList = list_create();
	char opcion='n';

	while(opcion != 'e'){

		puts("h: Crear nuevo hilo");
		puts("l: Listar hilos");
		puts("e: Salir");

		scanf("%s",&opcion);

		switch(opcion){

		case 'h':
			pthread_create(&nuevoHilo, NULL, nuevoCore, NULL);
			list_add(coreList, &nuevoHilo);
			break;
		case 'l':

			break;

		}
	}

//	FIN IMPLEMENTACION DE PRUEBA


//	Conecto con el núcleo primero que nada Author: Dr.Mengueche

//	clienteNucleo=crearCliente(ipNucleo,nucleoPort);

//	Conecto con el cpu. Author: Dr.Mengueche
//	puts("hola");
//	int clienteUMC=crearCliente(ipUMC,umcPort);

//	Imprimo al cliente. Author: Dr.Mengueche






	return EXIT_SUCCESS;

}

void* nuevoCore(void* argumento){

	printf("Se ha creado un CPU.\n");
	pthread_exit(NULL);
}
