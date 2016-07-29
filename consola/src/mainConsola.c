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
StrConKer* sckthread = NULL;
StrKerCon* skc = NULL;
String direccionDeArchivo;
int mandoPrograma;
Boolean lastInstruction;

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
void crearHiloConsola();


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

		mandoPrograma=0;
		//Creo hilo de manejo de mensajes por Consola

		crearHiloConsola();

		log_info(getLogger(), "Obteniendo el archivo .AnSISOP y enviandolo al NUCLEO");
		if (!callAndSendAnSISOP(direccionDeArchivo)) {
			log_error(getLogger(), "No se pudo obtener el archivo .AnSISOP y enviarlo al NUCLEO");
			return FALSE;
		}
		mandoPrograma = 1;
		lastInstruction = FALSE;
		while (TRUE) {
			if(!instructionsFromKernel()) {
				if (lastInstruction) {
					log_info(getLogger(), "===============================================");
					log_info(getLogger(), "===== CONSOLA FINALIZADA PARA EL PROGRAMA =====");
					log_info(getLogger(), "===============================================");
					return FALSE;
				}
				log_error(getLogger(), "No se recibio instrucciones del NUCLEO");
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
    	log_error(getLogger(), "No se encuentra o falta el archivo de configuracion en la ruta '/%s'.", ARCHIVO_CONF);
    	return FALSE;
    }

    if (config_keys_amount(tConfig) == PARAM_LENGTH) {
    	if (config_has_property(tConfig, PUERTO_NUCLEO)) {
    		log_info(getLogger(), "El parametro PUERTO_NUCLEO se encontro en el archivo de configuracion");
    		puertoKernel = config_get_int_value(tConfig, PUERTO_NUCLEO);
    	} else {
    		log_error(getLogger(), "Falta el parametro: PUERTO_NUCLEO.");
    		return FALSE;
    	}

    	if (config_has_property(tConfig, IP_NUCLEO)) {
    		log_info(getLogger(), "El parametro IP_NUCLEO se encontro en el archivo de configuracion");
    		ipKernel = config_get_string_value(tConfig, IP_NUCLEO);
    	} else {
    		log_error(getLogger(), "Falta el parametro: IP_NUCLEO.");
    		return FALSE;
    	}

    	log_info(getLogger(), "Archivo de configuracion CONSOLA leido exitosamente\n=============");
    	printf("PUERTO_NUCLEO: %d\nIP_NUCLEO: %s\n", puertoKernel, ipKernel);
    	return TRUE;
    } else {
    	log_error(getLogger(), "El archivo de configuracion no tiene todos los campos.");
    	return FALSE;
    }
}

Boolean socketConnection() {
	kernelClient = socketCreateClient();

	do {
		log_info(getLogger(), "**********************************");
		log_info(getLogger(), "Intentando conectar con el NUCLEO.");
		log_info(getLogger(),"IP: %s, Puerto: %d\n", ipKernel, (int)puertoKernel);
		sleep(3);
	} while(!socketConnect(kernelClient, ipKernel, puertoKernel));

	log_info(getLogger(), "La CONSOLA se conecto al NUCLEO exitosamente\n=============");

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
		log_error(getLogger(), "La direccion del archivo es vacio. ------- Terminando.");
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
		log_error(getLogger(), "Se trato de enviar el stream al NUCLEO sin inicializar. ----- Terminando.");
		return FALSE;
	}

	SocketBuffer* sb = serializeConKer(sck);

	if(!socketSend(kernelClient->ptrSocket, sb)) {
		log_error(getLogger(), "No se pudo enviar el stream al NUCLEO. ----- Terminando.");
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
		log_error(getLogger(),"No se pudo recibir el stream del NUCLEO. ----- Terminando.");
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
//			StrConKer* out_ker_msg = newStrConKer(CONSOLA_ID, TODO_PIOLA, NULL, 0);
//			sb = serializeConKer(out_ker_msg);
//
//			if(!socketSend(kernelClient->ptrSocket, sb)){
//				puts("NO SE PUDO ENVIAR EL MENSAJE!!");
//			}

			return realizarCierreConsola();
		default:
			log_error(getLogger(), "Action: %d", skc->action);
			return FALSE;
	}
	return TRUE;
}

Boolean realizarImprimir() {
	t_valor_variable valor_mostrar;
	valor_mostrar = skc->valorImp;
	printf("%d\n", valor_mostrar);
	return TRUE;
}

Boolean realizarImprimirTexto() {
	printf("%s\n", stringFromByteArray(skc->log, skc->logLen));
	return TRUE;
}

Boolean realizarCierreConsola() {
	printf("%s\n", stringFromByteArray(skc->log, skc->logLen));
	lastInstruction = TRUE;
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

void funcionHiloConsola(pthread_t hiloConsola){
	while(mandoPrograma == 0);
		while(TRUE){
			printf("Si desea finalizar el programa ingrese 1: \n");
			int finalizar_Programa;
			scanf("%d",&finalizar_Programa);
			if(finalizar_Programa == 1){
				sckthread = newStrConKer((char) CONSOLA_ID, (char) ABORTAR_PROGRAMA, NULL, 0);

				if (sckthread == NULL) {
					log_error(getLogger(), "Se trato de enviar el stream al NUCLEO sin inicializar. ----- Terminando.");
				}

				SocketBuffer* sbthread = serializeConKer(sckthread);

				if(!socketSend(kernelClient->ptrSocket, sbthread)) {
					log_error(getLogger(), "No se pudo enviar el stream al NUCLEO. ----- Terminando.");
				} else {
					log_info(getLogger(), "Se aborto el programa por CONSOLA");
				}
				free(sbthread);

				int pidconsola = getpid();
				kill(pidconsola, SIGKILL);

				//fclose(fp); esto no va giles
			} else {
				printf("Numero no válido\n");
			}
		}
}

void crearHiloConsola() {
	pthread_t hiloConsola;
	pthread_attr_t attrHiloConsola;
	pthread_attr_init(&attrHiloConsola);
	pthread_attr_setdetachstate(&attrHiloConsola, PTHREAD_CREATE_DETACHED);
	pthread_create(&hiloConsola, &attrHiloConsola, &funcionHiloConsola, NULL);
	pthread_attr_destroy(&attrHiloConsola);
}
