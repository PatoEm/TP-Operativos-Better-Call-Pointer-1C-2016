#include "swap.h"

int main(void) {

	leerArchivoDeConfiguracion("swapconfig");

	archivoMappeado = crearArchivo(tamArchivo, nombreSwap);

	manejoDeConexiones();

}
