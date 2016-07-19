#include "swap.h"

t_log* logger = NULL;

int main(void) {

	leerArchivoDeConfiguracion("/home/utnso/tp-2016-1c-Better-call-pointer/swap/swapconfig");
	tamPaginaLoca=atoi(tamPagina);
	iniciarEstructurasSwap();
	archivoMappeado = crearArchivo(tamArchivo, nombreSwap);
	
	manejoDeConexiones();
	
	log_destroy(logger);
	
	return EXIT_SUCCESS;
}
