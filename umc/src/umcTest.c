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
	memoriaReal = reservarMemoria(marcos, marco_Size);
	iniciarEstructurasUMC();

	inicializarPrograma(1, 1, "holaF");

	inicializarPrograma(2, 2, "Me duermo");

	char* cadena = solicitarBytes(1, 0, 0, 5);

	char* cadena2= solicitarBytes(2, 0, 0, 3);

	almacenarBytes(2, 0,0,3,"no ");

	cadena2= solicitarBytes(2, 0, 0, 9);

	almacenarBytes(2, 1, 0, 3, "gol");

	cadena2= solicitarBytes(2,1,0,3);

	printf("en la página 1 del proceso 1 hay: %s",cadena);

	finalizarPrograma(2);

	finalizarPrograma(1);

	return 0;
}

