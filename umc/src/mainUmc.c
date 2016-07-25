/*
 ============================================================================
 Name        : umc.c

BETTER CALL POINTER PERRO
 ============================================================================
 */

#include "umc.h"

int main(int argc, char** argv) {

	if(argc != 2){
		puts("No se ingreso la ruta del archivo de configuracion\n");
		return 0;
	}

	thread_socket= 3030;
	paginaEncontrada=TRUE;
	umclog=malloc(sizeof(t_log));
	memcpy(umclog,log_create("umc.log", "UMC", TRUE, LOG_LEVEL_TRACE), sizeof(t_log));

	leerArchivoDeConfiguracion(argv[1]);
	crearLogger(0);
	log_info(umclog, "Inicio UMC.");

	pthread_t hiloComandos;
	pthread_attr_t attrhiloComandos;

	memoriaReal = reservarMemoria(marcos, marco_Size); //Fabrico mi memoria real

	iniciarEstructurasUMC();

	socketSwap=socketCreateClient();

	socketConnect(socketSwap,ip_Swap,atoi(puerto_Swap));

	menuUMC(hiloComandos, attrhiloComandos);

	manageSocketConnections();


	liberarMemoria(memoriaReal); //Una vez terminado, libero toda mi memoria real
	log_destroy(logger);

	return EXIT_SUCCESS;

}

