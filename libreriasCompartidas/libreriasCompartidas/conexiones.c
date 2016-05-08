/*
 * conexiones.c
 *
 *  Created on: 26/4/2016
 *      Author: utnso
 */
#include "conexiones.h"


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
	         //config_destroy(archivoDeConfiguracion); ESTA MIERDA ERA

}

int verificarExistenciaDeArchivo(char* rutaArchivoConfig) {
	FILE * archivoConfig = fopen(rutaArchivoConfig, "r");
	if (archivoConfig!=NULL){
		fclose(archivoConfig);
		return 1;
	}
	return FAIL;
}

