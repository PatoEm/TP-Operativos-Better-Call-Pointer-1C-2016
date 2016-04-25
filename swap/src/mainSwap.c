#include "swap.h"

int main(void) {

    void leerArchivoDeConfiguracion();


	int fdSocketSwap = crearCliente(ipUMC,umcPort );/*va UMC o van variables de SWAP?*/
	printf("%d",fdSocketSwap);



	return EXIT_SUCCESS;
}
