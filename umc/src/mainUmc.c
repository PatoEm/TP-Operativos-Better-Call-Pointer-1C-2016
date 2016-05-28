/*
 ============================================================================
 Name        : umc.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "umc.h"

int main(void) {

	leerArchivoDeConfiguracion("configumc");
	crearLogger(1);

	puts("Inicio UMC");

	log_info(logger, "Inicio Log UMC", NULL);

	memoriaReal = reservarMemoria(marcos, marco_Size); //Fabrico mi memoria real
	pthread_t hiloComandos;

	pthread_attr_t attrhiloComandos;

	pthread_attr_init(&attrhiloComandos);

	pthread_attr_setdetachstate(&attrhiloComandos, PTHREAD_CREATE_DETACHED);
	int hiloParaComandos = pthread_create (&hiloComandos, &attrhiloComandos, (void *)comandosUMC, NULL);


	pthread_attr_destroy(&attrhiloComandos);
	escuchoMuchasConexiones();

/*
	int fdSocketNucleo= crearSocketServidor(puertoEscucha);
	escucharSocket(fdSocketNucleo,1);
	int aceptarNucleo=aceptarConexiones(fdSocketNucleo);
*/
	//no tengo idea para que sirve esto, lo puedo romper? COMENTEMOS QUE HACEMOS

/*
	int fdSocketCPU= crearSocketServidor(puertoEscucha);
	escucharSocket(fdSocketCPU,5);
	int conexionCPU = aceptarConexiones(fdSocketCPU);
	char* mensaje;
	mensaje=(char*)malloc(11*sizeof(char));
	if(mensaje==NULL){
		puts("falló al reservar la memoria");
		return -1;
	}
	recibirMensaje(conexionCPU,mensaje,11*sizeof(char));
	puts(mensaje);
*/

	int fdSocketUMC = crearCliente(ip_Swap,puerto_Swap);
	//enviarMensaje(fdSocketUMC,mensaje,11*sizeof(char));




	//printf("el socket de la umc es:%d y el del nucleo es: %d",fdSocketUMC, aceptarNucleo);

	liberarMemoria(memoriaReal); //Una vez terminado, libero toda mi memoria real
	log_destroy(logger);

	return EXIT_SUCCESS;

}
