/*
 * conexiones.c
 *
 *  Created on: 26/4/2016
 *      Author: utnso
 */
#include "archivosYLogsYMas.h"


int verificarMemoria(void*algo){
	if(algo==NULL){
							printf("no se pudo reservar memoria para la ruta del Archivo");
							return-1;
			              }
	return 0;
}

void leerArchivoDeConfiguracion(const char * direccionArchivo) {
             t_config* archivoDeConfiguracion;
             char*configPath = direccionArchivo;
            if (verificarExistenciaDeArchivo(configPath) == FAIL)
            puts("[ERROR] Archivo de configuracion no encontrado");
             archivoDeConfiguracion = config_create(configPath);
             setearValores(archivoDeConfiguracion);
	        // config_destroy(archivoDeConfiguracion); //ESTA MIERDA ERA

}

int verificarExistenciaDeArchivo(char* rutaArchivoConfig) {
	FILE * archivoConfig = fopen(rutaArchivoConfig, "r");
	if (archivoConfig!=NULL){
		fclose(archivoConfig);
		return 1;
	}
	return FAIL;
}

void crearLogger(int pantalla) //recibe 0 para log solo por archivo y recibe 1 para log por archivo y por pantalla
{
	if (pantalla)
		logger = log_create("log.txt", "Elestac",true, LOG_LEVEL_INFO);
	else
		logger = log_create("log.txt", "Elestac",false, LOG_LEVEL_INFO);

    /*
     * FUNCIONES UTILIZABLES PARA EL LOGGEO, EJEMPLOS:
    log_trace(logger, "LOG A NIVEL %s", "TRACE");
    log_debug(logger, "LOG A NIVEL %s", "DEBUG");
    log_info(logger, "LOG A NIVEL %s", "INFO");
    log_warning(logger, "LOG A NIVEL %s", "WARNING");
    log_error(logger, "LOG A NIVEL %s", "ERROR");
	*/

} //Probado, anda piola

int cantidadPaginas (int tamanioArchivo, int tamanioPagina){

	if ((tamanioArchivo%tamanioPagina)==0){
		return (tamanioArchivo/tamanioPagina);
	}
	else{
		return ((tamanioArchivo/tamanioPagina)+1);
	}
}

