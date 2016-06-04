#include "swap.h"

int main(void) {
	char*paginaLeida;

	leerArchivoDeConfiguracion("swapconfig");

	archivoMappeado = crearArchivo(tamArchivo, nombreSwap);

	iniciarEstructurasSwap();

	recibirNuevoPrograma(1, 1, 0);

	recibirNuevoPrograma(2, 1, 0);

	paginaLeida = leerUnaPagina(1, 0);

	strcpy(paginaLeida, "hola");

	escribirPagina(1, 0, paginaLeida);

	eliminarProceso(1);

	recibirNuevoPrograma(3, 2, 0);

}
