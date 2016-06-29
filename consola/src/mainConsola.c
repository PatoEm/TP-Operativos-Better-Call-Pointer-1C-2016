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
		printf("Ingrese direccion del archivo: ");
	    scanf("%s", direccionDeArchivo);


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
		puts("No se pudo realizar el handshake.");
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
	long fileLen=tamArchivo(path);
	buffer=leerProgramaAnSISOP(path);
//	FILE* file = fopen(path, "rb");
//	puts("Leyendo el archivo");
//	if (!file) {
//		puts("[ERROR]: No se pudo abrir el archivo.");
//		return FALSE;
//	} else {
//		puts("Se encontro el archivo.");
//		fseek(file, 0, SEEK_END);
//		fileLen = ftell(file);
//		rewind(file);
//		buffer = (char*) malloc((fileLen + 1) * sizeof(char));
//		fread(buffer, fileLen, 1, file);
//		fclose(file);
//	}



	if (sck != NULL) {
		free(sck);
	}

	sck = newStrConKer((char) CONSOLA_ID, (char) ARCHIVO_ANSISOP, buffer, fileLen);
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

/*
	//lee archivo de configuración y se conecta al nucleo. COMENTEMOS QUE HACEMOS
	leerArchivoDeConfiguracion("configconsola");
	 
	puts("hola");
	fdSocketConsola = crearCliente(ipNucleo,nucleoPort );
	recibirArchivo();//Se encarga de recibir el archivo y mandarselo al nucleo
	int estado;
	do{
		char* protocolo=string_new();
		recibirMensaje(fdSocketConsola,protocolo,2);
		estado=atoi(protocolo);
		switch(estado){
			case IMPRIMIR:{
				char* nombre=string_new();
				int variable;
				recibirMensaje(fdSocketConsola,&variable,4);
				recibirMensaje(fdSocketConsola,nombre,1);
				printf("la variable %s vale: %d \n",nombre,variable);
				break;
			}
			case IMPRIMIRTEXTO:{
				int tamanioTexto;
				char* textoAImprimir=string_new();
				recibirMensaje(fdSocketConsola,&tamanioTexto,4);
				recibirMensaje(fdSocketConsola,textoAImprimir,tamanioTexto);
				printf("%s \n",textoAImprimir);
				free(textoAImprimir);
				break;

			}
			case CERRARCONSOLA:{
				printf("el procesamiento de su archivo ha finalizado.Buen día");
				cerrarSocket(fdSocketConsola);

			}


		}

	}while(estado!=5);

    return 0;
 }
*/

// <<<<<< COMENTADO VIEJO >>>>>
    	/*
}
	int operacionId;
	int exit = false;
	while(!exit){
	//Esperando operacion a realizar
	int estado = recibirMensaje(fdSocketConsola, operacionId, sizeof(int));


	if(!estado){
		puts("Nucleo desconectado");
		exit = true;
	}else{
		switch(operacionId){
		int estadoOpe;

		case IMPRIMIR: ;

			char* variable;
			int valorDeVariable;

			estadoOpe= recibirMensaje(fdSocketConsola, &variable, sizeof(char*));
			estadoOpe = recibirMensaje(fdSocketConsola, valorDeVariable, sizeof(int));

			for(int i =0; i< ; i++){
				if(estadoOpe) printf("VARIABLE: %d \n%d \n"", variable,valorDeLaVariable);
			}
		break;
		case IMPRIMIRTEXTO: ;

			long longitudDelTexto;
			estadoOpe = recibirMensaje(fdSocketConsola, longitudDelTexto, sizeof(long)); //recivo la longitud

			if(estadoOpe){
				char* texto = malloc(sizeof(char)*(longitudDelTexto));
				estadoOpe= recv(fdSocketConsola, &texto, sizeof(char)*(longitudDelTexto), 0);//recivo el texto

				if(estadoOpe) printf("IMPRESION DE TEXTO: %s \n",texto);

				free(texto);
					}
		break;
		}
	}
	}
	close(fdSocketConsola);	// Cierro el socket

return EXIT_SUCCESS;
}
*/ //Hasta que no sepa el protolo no puedo hacerlo
