/*
 * umcTest.c
 *
 *  Created on: 2/7/2016
 *      Author: utnso
 */

/*
 * umcTest.c
 *
 *  Created on: 2/7/2016
 *      Author: utnso
 */

#include "umc.h"

int main() {
	leerArchivoDeConfiguracion("configumc");

	socketSwap = socketCreateClient();

	socketConnect(socketSwap, ip_Swap, atoi(puerto_Swap));

	iniciarEstructurasUMC();

	inicializarPrograma(1, 1, "holaF");

	char* cadena = solicitarBytes(1, 1, 0, 5);

	printf("en la página 1 del proceso 1 hay: %s",cadena);

	return 0;
}

