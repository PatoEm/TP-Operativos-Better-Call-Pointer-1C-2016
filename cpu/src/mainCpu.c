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

void* tareaCore(void* argumento);
void imprimirElementosLista (t_list *a);
int nuevoCore(t_list *lista);

int main() {


	leerArchivoDeConfiguracion("configcpu");


//	IMPLEMENTACION DE PRUEBA

	t_list* coreList = list_create();
	char opcion='n';

	while(opcion != 'e'){

		puts("h: Crear nuevo hilo");
		puts("l: Listar hilos");
		puts("s: Cantidad Cpus");
		puts("e: Salir");

		scanf("%s",&opcion);

		switch(opcion){

		case 'h':
			nuevoCore(coreList);
			break;
		case 'l':
			imprimirElementosLista(coreList);
			break;
		case 's':
			printf("Hay %d CPUs.\n", list_size(coreList));
			break;

		}

	}
	list_clean(coreList);

//	FIN IMPLEMENTACION DE PRUEBA


//	Conecto con el núcleo primero que nada Author: Dr.Mengueche

//	clienteNucleo=crearCliente(ipNucleo,nucleoPort);

//	Conecto con el cpu. Author: Dr.Mengueche
//	puts("hola");
//	int clienteUMC=crearCliente(ipUMC,umcPort);

//	Imprimo al cliente. Author: Dr.Mengueche






	return EXIT_SUCCESS;

}

void* tareaCore(void* argumento){

	printf("Se ha creado un CPU.\n");
	pthread_exit(NULL);
}

int nuevoCore(t_list *lista){
	pthread_t nuevoHilo;
	pthread_create(&nuevoHilo, NULL, tareaCore, NULL);
	list_add(lista, 5);
	printf("ID: %d\n", (int)&nuevoHilo);
	return 0;
}


void imprimirElementosLista (t_list *a){
	t_link_element *aux = a->head;
	int i = 0;
	while (aux != NULL)
	{
		printf("\nelemento de la lista %d: %d\n", i++,aux->data);
		aux = aux->next;
	}
	if (aux == NULL)
		printf("\n\nSe ha llegado al final de la lista\n\n");
}
