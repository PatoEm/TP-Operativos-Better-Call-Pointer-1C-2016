/*
 ============================================================================
 Name        : consola.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
/*
 * Dependencias
 */
#include "consola.h"

/*
 * Variables Globales
 */
Int32U puertoKernel = 0;
String ipKernel;
t_config* tConfig;
SocketClient* kernelClient = NULL;

StrConKer* sck = NULL;
StrKerCon* skc = NULL;
String direccionDeArchivo;

/*
 * Prototipos
 */
Boolean loadConfig();
Boolean socketConnection();
Boolean callAndSendAnSISOP(String);
Boolean sendStream();
Boolean instructionsFromKernel();
Boolean realizarImprimir();
Boolean realizarImprimirTexto();
Boolean realizarCierreConsola();
String stringFromByteArray(Byte*, Int32U);

int main(void) {

	if (loadConfig() && socketConnection()) {

		direccionDeArchivo = (char *) malloc(150);
		verificarMemoria(direccionDeArchivo);

		do {
		printf("Ingrese direccion del archivo: ");
	    scanf("%s", direccionDeArchivo);
		} while (access(direccionDeArchivo, R_OK));


		if (!callAndSendAnSISOP(direccionDeArchivo)) {
			puts("No se pudo obtener el archivo y enviarlo al Kernel");
			return FALSE;
		}

		while (TRUE) {
			if(!instructionsFromKernel()) {
				puts("No se recibio instrucciones del Kernel");
				return FALSE;
			}
		}
	}
	config_destroy(tConfig);
	return TRUE;
}

/*
 * Implementacion de Funciones
 */
Boolean loadConfig() {
    tConfig = config_create(ARCHIVO_CONF);
    if (tConfig == NULL) {
    	printf("No se encuentra o falta el archivo de configuracion en la direccion '/%s'.\n", ARCHIVO_CONF);
    	return FALSE;
    }

    if (config_keys_amount(tConfig) == PARAM_LENGTH) {
    	if (config_has_property(tConfig, PUERTO_NUCLEO)) {
    		puertoKernel = config_get_int_value(tConfig, PUERTO_NUCLEO);
    	} else {
    		printf("[ERROR]: Falta un parametro. \n");
    		return FALSE;
    	}

    	if (config_has_property(tConfig, IP_NUCLEO)) {
    		ipKernel = config_get_string_value(tConfig, IP_NUCLEO);
    	} else {
    		printf("[ERROR]: Falta un parametro. \n");
    		return FALSE;
    	}

    	printf("Archivo de configuracion CONSOLA leido exitosamente\n=============\n");
    	printf("PUERTO_NUCLEO: %d\nIP_NUCLEO: %s\n", puertoKernel, ipKernel);
    	return TRUE;
    } else {
    	printf("[ERROR]: El archivo de configuracion no tiene todos los campos. \n");
    	return FALSE;
    }
}

Boolean socketConnection() {
	kernelClient = socketCreateClient();

	do {
		puts("**********************************");
		puts("Intentando conectar con el Nucleo.");
		printf("IP: %s, Puerto: %d\n", ipKernel, (int)puertoKernel);
		sleep(3);
	} while(!socketConnect(kernelClient, ipKernel, puertoKernel));

	if(handshake(kernelClient, CONSOLA_ID)){
		puts("Handshake realizado con exito.");
	} else {
		puts("No se pudo realizar el handshake!!!.");
		return FALSE;
	}

	return TRUE;
}



Boolean callAndSendAnSISOP(String path) {

	if ( path == NULL || string_is_empty(path)) {
		printf("[ERROR]: La direccion del archivo es vacio. ------- Terminando \n");
		return FALSE;
	}

	char* buffer = NULL;
	char cpBuffer[18] = "";

	long fileLen;
	fileLen = tamArchivo(path);
	buffer = leerProgramaAnSISOP(path);

	strncpy(cpBuffer, buffer, 18);

	// Confirmo si el archivo tiene el SHEBANG.
	char rutaCodigoAnsisop[18] = "#!/usr/bin/ansisop";
	int j;
	for (j = 0; j < 18; ++j) {
		if (rutaCodigoAnsisop[j] != cpBuffer[j]){
			puts("El archivo obtenido no es un .AnSISOP");
			return FALSE;
		}
	}

	puts("Se reconocio un archivo.AnSISOP");
	//printf("%s\n", buffer);

	if (sck != NULL) {
		free(sck);
	}

	sck = newStrConKer((char) CONSOLA_ID, (char) ARCHIVO_ANSISOP, (Byte*) buffer, fileLen);
	puts("Enviando al Nucleo el archivo");
	return sendStream();
}

Boolean sendStream() {
	if (sck == NULL) {
		printf("[ERROR] Al tratar de enviar el stream al Kernel sin inicializar. ----- Terminando \n");
		return FALSE;
	}

	SocketBuffer* sb = serializeConKer(sck);

	if(!socketSend(kernelClient->ptrSocket, sb)) {
		printf("[ERROR]: No se pudo enviar el stream al Kernel. ----- Ternimando \n");
		return FALSE;
	}
	free(sb);
	return TRUE;
}

Boolean instructionsFromKernel() {
	SocketBuffer* sb;

	if (skc != NULL) {
		free(skc);
	}
	puts("Esperando al Kernel");

	if((sb = socketReceive(kernelClient->ptrSocket)) == NULL) {
		printf("[ERROR]: No se pudo recibir el stream del Kernel. ----- Terminando \n");
		return FALSE;
	}
	puts("Instruccion del Kernel recibida");
	skc = unserializeKerCon((Stream) sb->data);

	switch(skc->action) {
		case IMPRIMIR:
			puts("Funcion [IMPRIMIR]");
			return realizarImprimir();
		case IMPRIMIRTEXTO:
			puts("Funcion [IMPRIMIRTEXTO]");
			return realizarImprimirTexto();
		case CERRARCONSOLA:
			puts("Cerrando Consola");
			return realizarCierreConsola();
		default:
			printf("Action: %d", skc->action);
			return FALSE;
	}
	return TRUE;
}

Boolean realizarImprimir() {
	t_valor_variable valor_mostrar;
	valor_mostrar = skc->log;
	printf("%d\n", valor_mostrar);
	return TRUE;
}

Boolean realizarImprimirTexto() {
	printf("%s\n", stringFromByteArray(skc->log, skc->logLen));
	return TRUE;
}

Boolean realizarCierreConsola() {
	printf("%s\n", stringFromByteArray(skc->log, skc->logLen));
	return FALSE;
}

String stringFromByteArray(Byte* data, Int32U size) {
	int i;
	String result = malloc(size);
	char* c = (char*) data;
	for (i=0; i < size; i++) {
		result[i] = *c;
		c++;
	}
	result[size] = '\0';
	return result;
}
