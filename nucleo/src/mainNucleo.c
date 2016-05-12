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
int main(void) {

	idProgramas=0;
	 leerArchivoDeConfiguracion("confignucleo");


	//lee archivo de configuración y se conecta a la consola. COMENTEMOS QUE HACEMOS



	puts("Inicio Nucleo");



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

	t_metadata_program programa1 = metadata_desde_literal(mensaje);






	// creo el servidor para el cpu y lo acepto. Author: Dr.Mengueche

	int fdSocketCpu= crearSocketServidor(cpuPort);
	escucharSocket(fdSocketCpu,5);
	int fdSocketClienteCpu1=aceptarConexiones(fdSocketCpu);





	enviarMensaje(fdSocketClienteCpu1,mensaje,11*sizeof(char));





	//Se conecta con la memoria. Author: Dr.Mengueche
	//Socket UMC testeado by Dr.Mengueche
	int fdSocketUMC=crearCliente(ipUMC,UMCPort);




	return EXIT_SUCCESS;
}
