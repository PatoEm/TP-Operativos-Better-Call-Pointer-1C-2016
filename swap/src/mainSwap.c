#include "swap.h"

int main(void) {

    leerArchivoDeConfiguracion("swapconfig");


    /*Se conecta a la UMC con el swap. LOCO COMENTEN!!!*/
    /*Funciona una vez. Falla el bind en la segunda. Testeado by Dr.Mengueche*/
    int fdSocketUMC = crearSocketServidor(umcPort);
    escucharSocket(fdSocketUMC, 1);
    int fdSocketCliente= aceptarConexiones(fdSocketUMC);


	printf("%d",fdSocketCliente);



	return EXIT_SUCCESS;
}
