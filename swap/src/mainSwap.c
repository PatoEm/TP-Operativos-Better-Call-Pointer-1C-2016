#include "swap.h"

t_log* logger = NULL;

int main(int argc, char** argv) {

	if(argc != 2){
		puts("No se ingreso la ruta del archivo de configuracion\n");
		return 0;
	}

	leerArchivoDeConfiguracion(argv[1]);
	tamPaginaLoca=atoi(tamPagina);
	iniciarEstructurasSwap();
	archivoMappeado = crearArchivo(tamArchivo, nombreSwap);
	
	manejoDeConexiones();
	
	log_destroy(logger);
	
	return EXIT_SUCCESS;
}
