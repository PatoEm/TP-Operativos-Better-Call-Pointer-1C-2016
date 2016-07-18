#include "swap.h"

int main(void) {
	swaplog = log_create("swap.log", "SWAP", TRUE, LOG_LEVEL_INFO);
	leerArchivoDeConfiguracion("/home/utnso/tp-2016-1c-Better-call-pointer/swap/swapconfig");
	tamPaginaLoca=atoi(tamPagina);
	iniciarEstructurasSwap();
	archivoMappeado = crearArchivo(tamArchivo, nombreSwap);

	manejoDeConexiones();

	return EXIT_SUCCESS;
}
