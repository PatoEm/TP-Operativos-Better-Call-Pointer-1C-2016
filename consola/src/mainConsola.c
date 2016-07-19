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

/*
 * Estructura para Logger
 */
t_log* logger = NULL;

int main(void) {
	log_info(getLogger(), "===============================================");
	log_info(getLogger(), "===== BIENVENIDO A LA CONSOLA DEL SISTEMA =====");
	log_info(getLogger(), "===============================================");
	log_info(getLogger(), "                                               ");
	log_info(getLogger(), "                                               ");
	log_info(getLogger(), "Leyendo archivo de configuracion y conectandose al NUCLEO");
	if (loadConfig() && socketConnection()) {

		direccionDeArchivo = (char *) malloc(150);
		verificarMemoria(direccionDeArchivo);

		do {
		printf("Ingrese la ruta del archivo: ");
	    scanf("%s", direccionDeArchivo);
		} while (access(direccionDeArchivo, R_OK));

		log_info(getLogger(), "Obteniendo el archivo .AnSISOP y enviandolo al NUCLEO");
		if (!callAndSendAnSISOP(direccionDeArchivo)) {
			log_error(getLogger(), "No se pudo obtener el archivo .AnSISOP y enviarlo al NUCLEO");
			return FALSE;
		}

		while (TRUE) {
			if(!instructionsFromKernel()) {
				log_error(getLogger(), "No se recibio instrucciones del NUCLEO");
				return FALSE;
			}
		}
	}
	config_destroy(tConfig);
	log_destroy(logger);
	return TRUE;
}


/*
 * Implementacion de Funciones
 */
Boolean loadConfig() {
    tConfig = config_create(ARCHIVO_CONF);
    if (tConfig == NULL) {
    	log_error(getLogger(), "No se encuentra o falta el archivo de configuracion en la ruta '/%s'.\n", ARCHIVO_CONF);
    	return FALSE;
    }

    if (config_keys_amount(tConfig) == PARAM_LENGTH) {
    	if (config_has_property(tConfig, PUERTO_NUCLEO)) {
    		log_info(getLogger(), "El parametro PUERTO_NUCLEO se encontro en el archivo de configuracion");
    		puertoKernel = config_get_int_value(tConfig, PUERTO_NUCLEO);
    	} else {
    		log_error(getLogger(), "Falta el parametro: PUERTO_NUCLEO. \n");
    		return FALSE;
    	}

    	if (config_has_property(tConfig, IP_NUCLEO)) {
    		log_info(getLogger(), "El parametro IP_NUCLEO se encontro en el archivo de configuracion");
    		ipKernel = config_get_string_value(tConfig, IP_NUCLEO);
    	} else {
    		log_error(getLogger(), "Falta el parametro: IP_NUCLEO. \n");
    		return FALSE;
    	}

    	log_info(getLogger(), "Archivo de configuracion CONSOLA leido exitosamente\n=============\n");
    	printf("PUERTO_NUCLEO: %d\nIP_NUCLEO: %s\n", puertoKernel, ipKernel);
    	return TRUE;
    } else {
    	log_error(getLogger(), "El archivo de configuracion no tiene todos los campos. \n");
    	return FALSE;
    }
}

Boolean socketConnection() {
	kernelClient = socketCreateClient();

	do {
		puts("**********************************");
		puts("Intentando conectar con el NUCLEO.");
		printf("IP: %s, Puerto: %d\n", ipKernel, (int)puertoKernel);
		sleep(3);
	} while(!socketConnect(kernelClient, ipKernel, puertoKernel));

	log_info(getLogger(), "La CONSOLA se conecto al NUCLEO exitosamente\n=============\n");

	if(handshake(kernelClient, CONSOLA_ID)){
		log_info(getLogger(), "Handshake realizado con exito.");
	} else {
		log_error(getLogger(), "No se pudo realizar el handshake!!!.");
		return FALSE;
	}

	return TRUE;
}

Boolean callAndSendAnSISOP(String path) {

	if ( path == NULL || string_is_empty(path)) {
		log_error(getLogger(), "La direccion del archivo es vacio. ------- Terminando \n");
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
			log_error(getLogger(), "El archivo obtenido no es un .AnSISOP");
			return FALSE;
		}
	}

	log_info(getLogger(), "Se reconocio el archivo como un .AnSISOP");
	//printf("%s\n", buffer);

	if (sck != NULL) {
		free(sck);
	}

	sck = newStrConKer((char) CONSOLA_ID, (char) ARCHIVO_ANSISOP, (Byte*) buffer, fileLen);
	log_info(getLogger(), "Enviando el archivo al NUCLEO");
	return sendStream();
}

Boolean sendStream() {
	if (sck == NULL) {
		log_error(getLogger(), "Se trato de enviar el stream al NUCLEO sin inicializar. ----- Terminando \n");
		return FALSE;
	}

	SocketBuffer* sb = serializeConKer(sck);

	if(!socketSend(kernelClient->ptrSocket, sb)) {
		log_error(getLogger(), "No se pudo enviar el stream al NUCLEO. ----- Terminando \n");
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
	log_info(getLogger(), "Esperando instrucciones del NUCLEO");

	if((sb = socketReceive(kernelClient->ptrSocket)) == NULL) {
		log_error(getLogger(),"No se pudo recibir el stream del NUCLEO. ----- Terminando \n");
		return FALSE;
	}
	log_info(getLogger(), "Instruccion del NUCLEO recibida");
	skc = unserializeKerCon((Stream) sb->data);

	switch(skc->action) {
		case IMPRIMIR:
			log_info(getLogger(), "Funcion [IMPRIMIR]");
			return realizarImprimir();
		case IMPRIMIRTEXTO:
			log_info(getLogger(), "Funcion [IMPRIMIRTEXTO]");
			return realizarImprimirTexto();
		case CERRARCONSOLA:
			log_info(getLogger(), "Cerrando CONSOLA");
			return realizarCierreConsola();
		default:
			log_error(getLogger(), "Action: %d", skc->action);
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
