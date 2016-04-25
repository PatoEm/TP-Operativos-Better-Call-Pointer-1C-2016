#include "swap.h"

int main(void) {

    void leerArchivoDeConfiguracion();


	int fdSocketSwap = crearCliente(ipUMC,umcPort );
	printf("%d",fdSocketSwap);



	return EXIT_SUCCESS;
}
