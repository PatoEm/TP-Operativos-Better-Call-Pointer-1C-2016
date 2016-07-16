#include "espacioAsignado.h"

espacioAsignado*newEspacioAsignado(){
	espacioAsignado*unEspacioVacio = malloc(sizeof(espacioAsignado));
	unEspacioVacio->IDPaginaInterno = 0;
	unEspacioVacio->bitDePresencia = 0;
	unEspacioVacio->bitModificado = 0;
	unEspacioVacio->bitUso = 0;
	unEspacioVacio->numDePag = 0;
	unEspacioVacio->pid = 0;
	unEspacioVacio->punteroAPagina = 0;
	return unEspacioVacio;
}
