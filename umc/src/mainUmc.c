/*
 ============================================================================
 Name        : umc.c

BETTER CALL POINTER PERRO
 ============================================================================
 */

#include "umc.h"

int main(void) {

	leerArchivoDeConfiguracion("configumc");
	crearLogger(0);

	puts("Inicio UMC");
	log_info(logger, "Inicio Log UMC", NULL);

	t_list * TLB;
	int accesosTLB;
	int aciertosTLB;
	pthread_t hiloComandos;
	pthread_attr_t attrhiloComandos;

	memoriaReal = reservarMemoria(marcos, marco_Size); //Fabrico mi memoria real

	inicioTLB(TLB, aciertosTLB, accesosTLB);

	socketSwap=socketCreateClient();

	socketConnect(socketSwap,ip_Swap,atoi(puerto_Swap));

	menuUMC(hiloComandos, attrhiloComandos);

	manageSocketConnections();


	liberarMemoria(memoriaReal); //Una vez terminado, libero toda mi memoria real
	log_destroy(logger);

	return EXIT_SUCCESS;

}
