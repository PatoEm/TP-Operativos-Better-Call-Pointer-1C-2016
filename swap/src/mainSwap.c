#include "swap.h"

int main(void) {

    void leerArchivoDeConfiguracion();


    /*Se conecta a la UMC con el swap. LOCO COMENTEN!!!*/
    int fdSocketUMC = crearSocketServidor(umcPort);
    escucharSocket(fdSocketUMC, 1);
    int fdSocketCliente= aceptarConexiones(fdSocketUMC);


	printf("%d",fdSocketUMC);



	return EXIT_SUCCESS;
}
