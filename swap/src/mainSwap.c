#include "swap.h"

int main(void) {

	leerArchivoDeConfiguracion("swapconfig");
	tamPaginaLoca=atoi(tamPagina);
	iniciarEstructurasSwap();
	archivoMappeado = crearArchivo(tamArchivo, nombreSwap);

	manejoDeConexiones();

	return EXIT_SUCCESS;
}
