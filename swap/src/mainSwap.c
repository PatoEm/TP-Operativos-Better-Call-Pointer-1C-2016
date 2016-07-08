#include "swap.h"

int main(void) {

	leerArchivoDeConfiguracion("swapconfig");
	iniciarEstructurasSwap();
	archivoMappeado = crearArchivo(tamArchivo, nombreSwap);

	manejoDeConexiones();

	return EXIT_SUCCESS;
}
