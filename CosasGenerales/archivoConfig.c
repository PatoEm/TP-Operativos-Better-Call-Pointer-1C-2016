#include "archivoConfig.h"
#define manejarError(msg) {perror(msg); abort();}

void leerArchivoDeConfiguracion(char * configPath) {
	t_config * archivoConfig;

	if (verificarExistenciaDeArchivo(configPath) == FAIL)
		manejarError("[ERROR] Archivo de configuracion no encontrado");

	archivoConfig = config_create(configPath);
	setearValores(archivoConfig);
	config_destroy(archivoConfig);
}

int verificarExistenciaDeArchivo(char* rutaArchivoConfig) {
	FILE * archivoConfig = fopen(rutaArchivoConfig, "r");
	if (archivoConfig!=NULL){
		fclose(archivoConfig);
		return 1;
	}
	return FAIL;
}

char * ingresarRuta(char * deQue){
	char * path = malloc(sizeof(char) * 100);
	printf("Ingrese la ruta del archivo de %s: ",deQue);
	scanf("%s",path);
	free(deQue);
	return path;
}
