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

	iniciarEstructurasUMC();

	inicializarPrograma(1,1,"holaF");

	return 0;
}

