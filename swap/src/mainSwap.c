#include "swap.h"

int main(void) {
	swaplog = log_create("swap.log", "SWAP", TRUE, LOG_LEVEL_INFO);
	leerArchivoDeConfiguracion("swapconfig");
	tamPaginaLoca=atoi(tamPagina);
	iniciarEstructurasSwap();
	archivoMappeado = crearArchivo(tamArchivo, nombreSwap);

	manejoDeConexiones();

	return EXIT_SUCCESS;
}
