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
#include "cpuHandler.h"


int main() {
	leerArchivoDeConfiguracion("configcpu");
	puts("Hello world!!!");

	cpuHandlerThread();

	return EXIT_SUCCESS;

}
