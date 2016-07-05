/*
 * cpu.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

/*******************************************************
 * DEPENDENCIAS
 *******************************************************/
#include "cpu.h"
#include <libreriasCompartidas/pointerStream.h>
#include <commons/collections/dictionary.h>
#define manejarError(msg) {perror(msg); abort();}
//======================================================

/*******************************************************
 * CONSTANTES
 *******************************************************/
#define NONE -1
#define T_NOMBRE_VARIABLE 0
#define T_PUNTERO 1
#define T_VALOR_VARIABLE 2
#define T_NOMBRE_COMPARTIDA 3
#define T_NOMBRE_ETIQUETA 4
#define T_NOMBRE_DISPOSITIVO 5
#define T_NOMBRE_SEMAFORO 6
#define CHAR_AST 7
#define INT 8

//======================================================

bool loadFunctionDictionary(t_dictionary** functionParameters) {

	*functionParameters = dictionary_create();

	//01
	FunctionParameters* fparamDefinirVariable = malloc(
			sizeof(FunctionParameters));
	fparamDefinirVariable->param[0] = T_NOMBRE_VARIABLE;
	fparamDefinirVariable->param[1] = NONE;
	fparamDefinirVariable->func = definirVariable;
	dictionary_put(*functionParameters, "DEFINIR_VARIABLE",
			fparamDefinirVariable);

	//02
	FunctionParameters* fparamObtenerPosicionVariable = malloc(
			sizeof(FunctionParameters));
	fparamObtenerPosicionVariable->param[0] = T_NOMBRE_VARIABLE;
	fparamObtenerPosicionVariable->param[1] = NONE;
	fparamObtenerPosicionVariable->func = obtenerPosicionVariable;
	dictionary_put(*functionParameters, "OBTENER_POSICION_VARIABLE",
			fparamObtenerPosicionVariable);

	//03
	FunctionParameters* fparamDereferenciar = malloc(
			sizeof(FunctionParameters));
	fparamDereferenciar->param[0] = T_PUNTERO;
	fparamDereferenciar->param[1] = NONE;
	fparamDereferenciar->func = dereferenciar;
	dictionary_put(*functionParameters, "DEREFERENCIAR", fparamDereferenciar);

	//04
	FunctionParameters* fparamAsignar = malloc(sizeof(FunctionParameters));
	fparamAsignar->param[0] = T_PUNTERO;
	fparamAsignar->param[1] = T_VALOR_VARIABLE;
	fparamAsignar->func = asignar;
	dictionary_put(*functionParameters, "ASIGNAR", fparamAsignar);

	//05
	FunctionParameters* fparamObtenerValorCompartida = malloc(
			sizeof(FunctionParameters));
	fparamObtenerValorCompartida->param[0] = T_NOMBRE_COMPARTIDA;
	fparamObtenerValorCompartida->param[1] = NONE;
	fparamObtenerValorCompartida->func = obtenerValorCompartida;
	dictionary_put(*functionParameters, "OBTENER_VALOR_COMPARTIDA",
			fparamObtenerValorCompartida);

	//06
	FunctionParameters* fparamAsignarValorCompartida = malloc(
			sizeof(FunctionParameters));
	fparamAsignarValorCompartida->param[0] = T_NOMBRE_COMPARTIDA;
	fparamAsignarValorCompartida->param[1] = T_VALOR_VARIABLE;
	fparamAsignarValorCompartida->func = asignarValorCompartida;
	dictionary_put(*functionParameters, "ASIGNAR_VALOR_COMPARTIDA",
			fparamAsignarValorCompartida);

	//07
	FunctionParameters* fparamIrAlLabel = malloc(sizeof(FunctionParameters));
	fparamIrAlLabel->param[0] = T_NOMBRE_ETIQUETA;
	fparamIrAlLabel->param[1] = NONE;
	fparamIrAlLabel->func = irAlLabel;
	dictionary_put(*functionParameters, "IR_A_LABEL", fparamIrAlLabel);

	//08
	FunctionParameters* fparamLlamarConRetorno = malloc(
			sizeof(FunctionParameters));
	fparamLlamarConRetorno->param[0] = T_NOMBRE_ETIQUETA;
	fparamLlamarConRetorno->param[1] = T_PUNTERO;
	fparamLlamarConRetorno->func = llamarConRetorno;
	dictionary_put(*functionParameters, "LLAMAR_CON_RETORNO",
			fparamLlamarConRetorno);

	//09
	FunctionParameters* fparamRetornar = malloc(sizeof(FunctionParameters));
	fparamRetornar->param[0] = T_VALOR_VARIABLE;
	fparamRetornar->param[1] = NONE;
	fparamRetornar->func = retornar;
	dictionary_put(*functionParameters, "RETORNAR", fparamRetornar);

	//10
	FunctionParameters* fparamImprimir = malloc(sizeof(FunctionParameters));
	fparamImprimir->param[0] = T_VALOR_VARIABLE;
	fparamImprimir->param[1] = NONE;
	fparamImprimir->func = imprimir;
	dictionary_put(*functionParameters, "IMPRIMIR", fparamImprimir);

	//11
	FunctionParameters* fparamImprimirTexto = malloc(
			sizeof(FunctionParameters));
	fparamImprimirTexto->param[0] = CHAR_AST;
	fparamImprimirTexto->param[1] = NONE;
	fparamImprimirTexto->func = imprimirTexto;
	dictionary_put(*functionParameters, "IMPRIMIR_TEXTO", fparamImprimirTexto);

	//12
	FunctionParameters* fparamEntradaSalida = malloc(
			sizeof(FunctionParameters));
	fparamEntradaSalida->param[0] = T_NOMBRE_DISPOSITIVO;
	fparamEntradaSalida->param[1] = INT;
	fparamEntradaSalida->func = entradaSalida;
	dictionary_put(*functionParameters, "ENTRADA_SALIDA", fparamEntradaSalida);

	//13
	FunctionParameters* fparamWait = malloc(sizeof(FunctionParameters));
	fparamWait->param[0] = T_NOMBRE_SEMAFORO;
	fparamWait->param[1] = NONE;
	fparamWait->func = wait;
	dictionary_put(*functionParameters, "WAIT", fparamWait);

	//14
	FunctionParameters* fparamSignal = malloc(sizeof(FunctionParameters));
	fparamSignal->param[0] = T_NOMBRE_SEMAFORO;
	fparamSignal->param[1] = NONE;
	fparamSignal->func = signal;
	dictionary_put(*functionParameters, "SIGNAL", fparamSignal);

	return true;
}

//Creo structs necesarias para el parser BY DRMENGUECHE

void setearValores(t_config * archivoConfig) {
	ipUMC = config_get_string_value(archivoConfig, "IP_UMC");
	umcPort = config_get_string_value(archivoConfig, "PUERTO_UMC");
	ipNucleo = config_get_string_value(archivoConfig, "IP_NUCLEO");
	nucleoPort = config_get_string_value(archivoConfig, "PUERTO_NUCLEO");
	puts("hola");
}

void cuerpoDelCpu(){

}

/*******************************************************
 * FUNCIONES AnSISOP
 *******************************************************/

/*
 * definirVariable
 */
t_puntero definirVariable(t_nombre_variable identificador_variable) {
	int pagina;
	espacioAsignado paginaAMandar;
	paginaDeStack*nuevaVariable;
	StrCpuUmc*streamCpuUmc;
	SocketBuffer*buffer;
	StrUmcCpu*streamUmcCpu;
	if (0 == list_size(pcbProceso.indiceDelStack)) {
		pagina = ((pcbProceso.paginasDeCodigo) - 1); //todo ojo acá si explota algo
		nuevaVariable = malloc(sizeof(paginaDeStack));
		paginaAMandar.numDePag = ((pcbProceso.paginasDeCodigo) - 1);
		nuevaVariable->pos = 0;
		streamCpuUmc = newStrCpuUmc(CPU_ID, 25/*SOLICITAR_BYTES*/,
				paginaAMandar, (tamanioPaginaUmc - 5), 4, NULL, 0);
		buffer = serializeCpuUmc(streamCpuUmc);
		socketSend(socketUMC->ptrSocket, buffer);
		buffer = socketReceive(socketUMC->ptrSocket);
		streamUmcCpu = unserializeCpuUmc(buffer);
		if ((espacioMemoriaVacio(streamUmcCpu->dataLen, streamUmcCpu->data))) {
			nuevaVariable->vars.id = identificador_variable;
			nuevaVariable->vars.pag = paginaAMandar.numDePag;
			nuevaVariable->vars.off = (tamanioPaginaUmc - 5);
			nuevaVariable->vars.size = 4;
			t_list * lista = pcbProceso.indiceDelStack;
			list_add((pcbProceso.indiceDelStack), nuevaVariable);
		} else
			seguirEjecutando = FALSE;
	}
	if (list_size(pcbProceso.indiceDelStack) != 0) {

		paginaDeStack*ultimaPagina = list_get(pcbProceso.indiceDelStack,
				list_size(pcbProceso.indiceDelStack) - 1);
		if (ultimaPagina->vars.off - 4 < 0) {
			paginaAMandar.numDePag = ultimaPagina->vars.pag - 1;
			streamCpuUmc = newStrCpuUmc(CPU_ID, 25/*SOLICITAR_BYTES*/,
					paginaAMandar, tamanioPaginaUmc - 5, 4, 0, 0);
			buffer = serializeCpuUmc(streamCpuUmc);
			socketSend(socketUMC->ptrSocket, buffer);
			buffer = socketReceive(socketUMC->ptrSocket);
			streamUmcCpu = unserializeCpuUmc(buffer);
			if ((espacioMemoriaVacio(streamUmcCpu->dataLen, streamUmcCpu->data))) {
				nuevaVariable->pos = ultimaPagina->pos + 1;
				nuevaVariable->vars.id = identificador_variable;
				nuevaVariable->vars.pag = paginaAMandar.numDePag;
				nuevaVariable->vars.off = (tamanioPaginaUmc - 5);
				nuevaVariable->vars.size = 4;
				t_list * lista = pcbProceso.indiceDelStack;
				list_add((pcbProceso.indiceDelStack), nuevaVariable);
			} else
				seguirEjecutando = FALSE;
		} else {
			paginaAMandar.numDePag = ultimaPagina->vars.pag;
			streamCpuUmc = newStrCpuUmc(CPU_ID, 25/*SOLICITAR_BYTES*/,
					paginaAMandar, ultimaPagina->vars.off - 4, 4, 0, 0);
			buffer = serializeCpuUmc(streamCpuUmc);
			socketSend(socketUMC->ptrSocket, buffer);
			buffer = socketReceive(socketUMC->ptrSocket);
			streamUmcCpu = unserializeCpuUmc(buffer);
			if ((espacioMemoriaVacio(streamUmcCpu->dataLen, streamUmcCpu->data))) {
				nuevaVariable->pos = ultimaPagina->pos + 1;
				nuevaVariable->vars.id = identificador_variable;
				nuevaVariable->vars.pag = paginaAMandar.numDePag;
				nuevaVariable->vars.size = 4;
				nuevaVariable->vars.off = ultimaPagina->vars.off - 4;
				t_list * lista = pcbProceso.indiceDelStack;
				list_add((pcbProceso.indiceDelStack), nuevaVariable);
			} else {
				seguirEjecutando = FALSE;
			}

		}
	}

	return nuevaVariable->pos;
}

// devuelve 1 si la página está vacía
bool espacioMemoriaVacio(int tamanio, char*bytes) {
	int counter = 0;
	while (counter < tamanio) {
		if (bytes[counter != '\0'])
			break;
		counter++;
	}
	if (counter == tamanio)
		return TRUE;
	else
		return FALSE;
}

/*
 * obtenerPosicionVariable
 */
t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable) {

	if (list_size(pcbProceso.indiceDelStack) != 0) {

		int i;
		paginaDeStack* aux;
		for (i = 0; i<list_size(pcbProceso.indiceDelStack); ++i) {

			aux =list_get(pcbProceso.indiceDelStack,i);
			if(aux->vars.id==identificador_variable){

				return aux->pos;
			}

		}



	} else {
		return -1;
	}
	return -1;
}

/*
 * dereferenciar
 */
t_valor_variable dereferenciar(t_puntero direccion_variable) {
	//todo Emi chequea esto
	espacioAsignado aux;
	aux.numDePag=direccion_variable/tamanioPaginaUmc;
	int offset=direccion_variable/tamanioPaginaUmc;
	//StrCpuUmc* newStrCpuUmc(Char id, Char action, espacioAsignado pageComienzo, Int32U offset, Int32U dataLen, Byte* data, Int32U pid){

	StrCpuUmc* streamCpuUMC;
	streamCpuUMC= newStrCpuUmc(CPU_ID, SOLICITAR_BYTES,aux ,offset, 4, NULL, 0);
	SocketBuffer* buffer= serializeCpuUmc(streamCpuUMC);
	socketSend(socketUMC->ptrSocket,buffer);
	buffer = socketReceive(socketUMC->ptrSocket);
	StrUmcCpu*streamUmcCpu;
	streamUmcCpu = unserializeCpuUmc(buffer);

	return atoi(streamCpuUMC->data);

}


/*
 * asignar
 */
void asignar(t_puntero direccion_variable, t_valor_variable valor) {

	espacioAsignado aux;
	aux.numDePag=direccion_variable/tamanioPaginaUmc;
	int offset=direccion_variable%tamanioPaginaUmc;


	Byte* dataAMandar;
	sprintf(dataAMandar,"%d",valor);


	StrCpuUmc* streamCpuUMC;

		streamCpuUMC= newStrCpuUmc(CPU_ID, ALMACENAR_BYTES,aux ,offset, 4, valor, 0);
		SocketBuffer* buffer= serializeCpuUmc(streamCpuUMC);
		socketSend(socketUMC->ptrSocket,buffer);
		buffer = socketReceive(socketUMC->ptrSocket);

}

//void asignar(t_puntero direccion_variable, t_valor_variable valor){
//	uint32_t pagina=direccion_variable/tamanioPagina;
//	uint32_t offset=direccion_variable%tamanioPagina;
//	almacenarBytesUMC(umc,pagina,offset,4,valor);
//}



/*
 * obtenerValorCompartida
 */
t_valor_variable obtenerValorCompartida(t_nombre_compartida variable) {
	StrCpuKer*streamCpuKer;
	streamCpuKer = newStrCpuKer(CPU_ID, 30 /*OBTENER_VALOR_COMPARTIDA*/,
			pcbProceso, pcbProceso.id, sizeof(variable), variable);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);
	buffer = socketReceive(socketNucleo->ptrSocket);
	StrKerCpu*streamKerCpu = unserializeKerCpu(buffer);
	if (streamKerCpu->action == 30/*OBTENER_VALOR_COMPARTIDA*/) {

		return (atoi(streamKerCpu->data));

	}
}

/*
 * 	asignarValorCompartida
 */
t_valor_variable asignarValorCompartida(t_nombre_compartida variable,
		t_valor_variable valor) {
	char* buffer;
	StrCpuKer*streamCpuKer;
	char* datosAEnviar = (sizeof(variable) + sizeof(valor) + 1);
	datosAEnviar = strcat(variable, atoi(valor));
	streamCpuKer = newStrCpuKer(CPU_ID, 33/*ASIGNAR_VALOR_COMPARTIDA*/,
			pcbProceso, pcbProceso.id, sizeof(variable), datosAEnviar);
	buffer = socketReceive(socketNucleo->ptrSocket);
	StrKerCpu*streamKerCpu = unserializeKerCpu(buffer);
	if (streamKerCpu->action == 33/*ASIGNAR_VALOR_COMPARTIDA*/) {
		return atoi(streamKerCpu->data);
	}
}

/*
 * 	irAlLabel
 */
void irAlLabel(t_nombre_etiqueta etiqueta) {
	pcbProceso.programCounter = metadata_buscar_etiqueta(etiqueta,pcbProceso.indiceDeEtiquetas,pcbProceso.etiquetaSize);

}

/*
 * llamarSinRetorno
 */
void llamarSinRetorno(t_nombre_etiqueta etiqueta) {
	printf("Operacion llamar sin retorno");
	//todo Que carajo es esto que ni en el tp existe?
}

/*
 * llamarConRetorno
 */
void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {

	paginaDeStack* aux=malloc(sizeof(paginaDeStack));
	aux->retVars.pag=donde_retornar/tamanioPaginaUmc;
	aux->retVars.off=donde_retornar%tamanioPaginaUmc;
	aux->retPos=pcbProceso.programCounter;
	list_add(pcbProceso.indiceDelStack, aux);

	pcbProceso.programCounter=metadata_buscar_etiqueta(etiqueta,pcbProceso.indiceDeEtiquetas,pcbProceso.etiquetaSize);

	printf("Operacion llamar con retorno");



}


/*
 * finalizar
 */
void finalizar(void) {
	StrCpuKer*streamCpuKer;
	streamCpuKer = newStrCpuKer(CPU_ID, 22 /*FINALIZAR_PROGRAMA*/, pcbProceso,
			pcbProceso.id, 0, NULL);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);
}

/*
 * retornar
 *
 */
void retornar(t_valor_variable retorno) {
	printf("Operacion de retorno");
	//todo EMI REVISA ESTO PORFA
	paginaDeStack *aux = list_remove(pcbProceso.indiceDelStack,(pcbProceso.indiceDelStack->elements_count)-1);
	pcbProceso.programCounter=aux->retPos;
	StrCpuUmc*streamCpuUmc;
	espacioAsignado paginaAMandar;
	paginaAMandar.numDePag=aux->retVars.pag;
	streamCpuUmc = newStrCpuUmc(CPU_ID, 34/*ALMACENAR_BYTES*/,paginaAMandar, aux->retVars.off, 4, NULL, 0);
		SocketBuffer*buffer = serializeCpuUmc(streamCpuUmc);
		socketSend(socketUMC->ptrSocket, buffer);

}


/*
 * imprimir
 */
int imprimir(t_valor_variable valor_mostrar) {

	StrCpuKer*streamCpuKer;
	Byte* aux;
		sprintf(aux,"%d",valor_mostrar);

	//streamCpuKer = newStrCpuKer(CPU_ID, IMPRIMIR, pcbProceso, pcbProceso.id,
			//strlen(itoa(valor_mostrar)), itoa(valor_mostrar));
		streamCpuKer = newStrCpuKer(CPU_ID, IMPRIMIR, pcbProceso, pcbProceso.id,
					strlen(aux), aux);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);

	Byte* auxLoco;
		sprintf(auxLoco,"%d",valor_mostrar);

	//return strlen(itoa(valor_mostrar));
		return auxLoco;
}

/*
 * imprimirTexto
 */

int imprimirTexto(char* texto) {
	StrCpuKer*streamCpuKer;
	streamCpuKer = newStrCpuKer(CPU_ID, IMPRIMIRTEXTO, pcbProceso,
			pcbProceso.id, strlen(texto), texto);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);
	return strlen(texto);
}

/*
 * entradaSalida
 */
void entradaSalida(t_nombre_dispositivo dispositivo, int tiempo) {
	Byte * auxLoco;
	sprintf(auxLoco,"%d",tiempo);
	char*inOut = malloc(
			//sizeof(dispositivo) + sizeof(itoa(tiempo)) + sizeof(char));
			sizeof(dispositivo) + sizeof(auxLoco) + sizeof(char));
	//inOut = strcat(dispositivo, itoa(tiempo));
	inOut = strcat(dispositivo, auxLoco);
	StrCpuKer*streamCpuKer;
	streamCpuKer = newStrCpuKer(CPU_ID, 29 /*ENTRADA_SALIDA*/, pcbProceso,
			pcbProceso.id, strlen(dispositivo), inOut);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);
	buffer = socketReceive(socketNucleo->ptrSocket);
	StrKerCpu*StreamKerCpu = unserializeKerCpu(buffer);
	if (StreamKerCpu->action == 31/*PROGRAMA_bLOQUEADO*/) {
		streamCpuKer = newStrCpuKer(CPU_ID, 32 /*ENVIO_PCB*/, pcbProceso,
				pcbProceso.id, 0, NULL);
		buffer = serializeCpuKer(streamCpuKer);
		socketSend(socketNucleo->ptrSocket, buffer);
		seguirEjecutando = FALSE;
	}
}

/*
 * wait
 */
void wait(t_nombre_semaforo identificador_semaforo) {
	StrCpuKer*streamCpuKer;
	streamCpuKer = newStrCpuKer(CPU_ID, 27 /*WAIT*/, pcbProceso, pcbProceso.id,
			strlen(identificador_semaforo), identificador_semaforo);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);
	buffer = socketReceive(socketNucleo->ptrSocket);
	StrKerCpu*StreamKerCpu = unserializeKerCpu(buffer);
	if (StreamKerCpu->action == 31/*PROGRAMA_bLOQUEADO*/) {
		streamCpuKer = newStrCpuKer(CPU_ID, 32 /*ENVIO_PCB*/, pcbProceso,
				pcbProceso.id, 0, NULL);
		buffer = serializeCpuKer(streamCpuKer);
		socketSend(socketNucleo->ptrSocket, buffer);
		seguirEjecutando = FALSE;
	}
}

/*
 * signal
 */
void signal(t_nombre_semaforo identificador_semaforo) {
	StrCpuKer*streamCpuKer;
	streamCpuKer = newStrCpuKer(CPU_ID, 28 /*SIGNAL*/, pcbProceso,
			pcbProceso.id, strlen(identificador_semaforo),
			identificador_semaforo);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);
}

char * pedirCodigoAUMC() {

//pcbEnEjecucion.programCounter;
	char*lineaDeCodigoADevolver =
			malloc(
					sizeof(char)
							* pcbProceso.indiceDeCodigo[pcbProceso.programCounter].longitud);
	int comienzo = pcbProceso.indiceDeCodigo[pcbProceso.programCounter].comienzo;

	int paginaDeComienzo = comienzo / tamanioPaginaUmc;

	int desplazamiento = comienzo
			+ pcbProceso.indiceDeCodigo[pcbProceso.programCounter].longitud;

	int paginaDeFin = desplazamiento / tamanioPaginaUmc;

	int dondeEmpiezo = comienzo - tamanioPaginaUmc * paginaDeComienzo;

	int longitudTotalAPedir =
			pcbProceso.indiceDeCodigo[pcbProceso.programCounter].longitud;

	if (paginaDeComienzo == paginaDeFin) {
		//todo pedir pagina con el tamaño que ya tengo de antemano
		//pcbEnEjecucion.indiceDeCodigo[pcbEnEjecucion.programCounter].longitud
		//usarPaginaDeComienzo
	} else {
		int cantPaginas = paginaDeFin - paginaDeComienzo;
		int i;
		int tamanioAPedir;
		int paginaActual = paginaDeComienzo;
		for (i = 0; i <= cantPaginas; i++) {

			tamanioAPedir = tamanioPaginaUmc - dondeEmpiezo;
			if (tamanioAPedir > longitudTotalAPedir) {
				//todo pedir longitudTotalAPedir de paginaActual
				break;
			} else {
				//todo pedir tamanioAPedir de paginaActual
				longitudTotalAPedir = longitudTotalAPedir - tamanioAPedir;
				dondeEmpiezo = 0;
			}
			//hay que ver como voy agrupando todo lo que pido en una sola cadena char*
			paginaActual++;

		}
	}
	return lineaDeCodigoADevolver;
}

AnSISOP_funciones funciones = { .AnSISOP_definirVariable = definirVariable,
		.AnSISOP_obtenerPosicionVariable = obtenerPosicionVariable,
		.AnSISOP_dereferenciar = dereferenciar, .AnSISOP_asignar = asignar,
		.AnSISOP_obtenerValorCompartida = obtenerValorCompartida,
		.AnSISOP_asignarValorCompartida = asignarValorCompartida,
		.AnSISOP_irAlLabel = irAlLabel, .AnSISOP_llamarConRetorno =
				llamarConRetorno, .AnSISOP_retornar = retornar,
		.AnSISOP_entradaSalida = entradaSalida, .AnSISOP_imprimir = imprimir,
		.AnSISOP_imprimirTexto = imprimirTexto, };

AnSISOP_kernel funcionesDeKernel = { .AnSISOP_wait = wait, .AnSISOP_signal =
		signal, };

