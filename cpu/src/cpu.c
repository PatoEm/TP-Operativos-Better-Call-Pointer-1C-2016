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

//======================================================
//Creo structs necesarias para el parser BY DRMENGUECHE
void setearValores(t_config * archivoConfig) {
	ipUMC = config_get_string_value(archivoConfig, "IP_UMC");
	umcPort = config_get_string_value(archivoConfig, "PUERTO_UMC");
	ipNucleo = config_get_string_value(archivoConfig, "IP_NUCLEO");
	nucleoPort = config_get_string_value(archivoConfig, "PUERTO_NUCLEO");
	puts("hola");
}

void cuerpoDelCpu() {

}

/*******************************************************
 * FUNCIONES AnSISOP
 *******************************************************/

/*
 * definirVariable
 */
t_puntero definirVariable(t_nombre_variable identificador_variable) { //NO TOCAR, YA ESTA TERMINADA
	int pagina;
	paginaDeStack*nuevaVariable;
	StrCpuUmc*streamCpuUmc;
	SocketBuffer*buffer;
	StrUmcCpu*streamUmcCpu;
	if (0 == list_size(pcbProceso.indiceDelStack)) {
		pagina = ((pcbProceso.paginasDeCodigo) - 1); //todo ojo acá si explota algo
		nuevaVariable = malloc(sizeof(paginaDeStack));
		asignadoVacio->numDePag = ((pcbProceso.paginasDeCodigo) - 1);
		nuevaVariable->pos = 0;
		streamCpuUmc = newStrCpuUmc(CPU_ID, SOLICITAR_BYTES, *asignadoVacio,
				(tamanioPaginaUmc - 5), 4, NULL, 0);
		buffer = serializeCpuUmc(streamCpuUmc);
		socketSend(socketUMC->ptrSocket, buffer);
		buffer = socketReceive(socketUMC->ptrSocket);
		streamUmcCpu = unserializeCpuUmc(buffer);
		if (streamUmcCpu->action == ABORTAR_PROGRAMA) {
			seguirEjecutando = FALSE;
		} else {
			if ((espacioMemoriaVacio(streamUmcCpu->dataLen, streamUmcCpu->data))) {
				nuevaVariable->vars.id = identificador_variable;
				nuevaVariable->vars.pag = asignadoVacio->numDePag;
				nuevaVariable->vars.off = (tamanioPaginaUmc - 5);
				nuevaVariable->vars.size = 4;
				t_list * lista = pcbProceso.indiceDelStack;
				list_add((pcbProceso.indiceDelStack), nuevaVariable);
			} else
				seguirEjecutando = FALSE;
		}
	}
	if (list_size(pcbProceso.indiceDelStack) != 0) {

		paginaDeStack*ultimaPagina = list_get(pcbProceso.indiceDelStack,
				list_size(pcbProceso.indiceDelStack) - 1);
		if (ultimaPagina->vars.off - 4 < 0) {
			asignadoVacio->numDePag = ultimaPagina->vars.pag - 1;
			streamCpuUmc = newStrCpuUmc(CPU_ID, SOLICITAR_BYTES, *asignadoVacio,
					tamanioPaginaUmc - 5, 4, 0, 0);
			buffer = serializeCpuUmc(streamCpuUmc);
			socketSend(socketUMC->ptrSocket, buffer);
			buffer = socketReceive(socketUMC->ptrSocket);
			streamUmcCpu = unserializeCpuUmc(buffer);
			if (streamUmcCpu->action == ABORTAR_PROGRAMA) {
				seguirEjecutando = FALSE;
			} else {
				if ((espacioMemoriaVacio(streamUmcCpu->dataLen,
						streamUmcCpu->data))) {
					nuevaVariable->pos = ultimaPagina->pos + 1;
					nuevaVariable->vars.id = identificador_variable;
					nuevaVariable->vars.pag = asignadoVacio->numDePag;
					nuevaVariable->vars.off = (tamanioPaginaUmc - 5);
					nuevaVariable->vars.size = 4;
					t_list * lista = pcbProceso.indiceDelStack;
					list_add((pcbProceso.indiceDelStack), nuevaVariable);
				} else
					seguirEjecutando = FALSE;
			}
		} else {
			asignadoVacio->numDePag = ultimaPagina->vars.pag;
			streamCpuUmc = newStrCpuUmc(CPU_ID, SOLICITAR_BYTES, *asignadoVacio,
					ultimaPagina->vars.off - 4, 4, 0, 0);
			buffer = serializeCpuUmc(streamCpuUmc);
			socketSend(socketUMC->ptrSocket, buffer);
			buffer = socketReceive(socketUMC->ptrSocket);
			streamUmcCpu = unserializeCpuUmc(buffer);
			if (streamUmcCpu->action == ABORTAR_PROGRAMA) {
				seguirEjecutando = FALSE;
			} else {
				if ((espacioMemoriaVacio(streamUmcCpu->dataLen,
						streamUmcCpu->data))) {
					nuevaVariable->pos = ultimaPagina->pos + 1;
					nuevaVariable->vars.id = identificador_variable;
					nuevaVariable->vars.pag = asignadoVacio->numDePag;
					nuevaVariable->vars.size = 4;
					nuevaVariable->vars.off = ultimaPagina->vars.off - 4;
					t_list * lista = pcbProceso.indiceDelStack;
					list_add((pcbProceso.indiceDelStack), nuevaVariable);
				} else {
					seguirEjecutando = FALSE;
				}
			}
		}
	}

	return nuevaVariable->pos;
}

// devuelve 1 si la página está vacía
bool espacioMemoriaVacio(int tamanio, char*bytes) {
	int counter = 0;
	while (counter < tamanio) {
		if (bytes[counter] != '\0')
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
 */ //NO TOCAR YA ESTÁ TERMINADA
t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable) {
	if (list_size(pcbProceso.indiceDelStack) != 0) {
		int i;
		paginaDeStack* aux;
		for (i = 0; i < list_size(pcbProceso.indiceDelStack); ++i) {
			aux = list_get(pcbProceso.indiceDelStack, i);
			if (aux->vars.id == identificador_variable) {
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
	//espacioAsignado espacio;
	asignadoVacio->numDePag = direccion_variable / tamanioPaginaUmc;
	int offset = direccion_variable / tamanioPaginaUmc;
	//StrCpuUmc* newStrCpuUmc(Char id, Char action, espacioAsignado pageComienzo, Int32U offset, Int32U dataLen, Byte* data, Int32U pid){

	StrCpuUmc* streamCpuUMC;
	streamCpuUMC = newStrCpuUmc(CPU_ID, SOLICITAR_BYTES, *asignadoVacio, offset,
			4, NULL, 0);
	SocketBuffer* buffer = serializeCpuUmc(streamCpuUMC);
	socketSend(socketUMC->ptrSocket, buffer);
	buffer = socketReceive(socketUMC->ptrSocket);
	StrUmcCpu*streamUmcCpu;
	streamUmcCpu = unserializeCpuUmc(buffer);
	if (streamUmcCpu->action == ABORTAR_PROGRAMA) {
		seguirEjecutando = FALSE;
		return 0;
	} else
		return atoi(streamCpuUMC->data);

}

/*
 * asignar
 */
void asignar(t_puntero direccion_variable, t_valor_variable valor) {

	asignadoVacio->numDePag = direccion_variable / tamanioPaginaUmc;
	int offset = direccion_variable % tamanioPaginaUmc;

	Byte* dataAMandar;
	sprintf(dataAMandar, "%d", valor);

	StrCpuUmc* streamCpuUMC;

	streamCpuUMC = newStrCpuUmc(CPU_ID, ALMACENAR_BYTES, *asignadoVacio, offset,
			4, valor, 0);	// todo chequear si llego bien o aborta
	SocketBuffer* buffer = serializeCpuUmc(streamCpuUMC);
	socketSend(socketUMC->ptrSocket, buffer);
	buffer = socketReceive(socketUMC->ptrSocket);
	//////////////////////////////////////////////////////////////////
	StrUmcCpu*streamUmcCpu;
	streamUmcCpu = unserializeCpuUmc(buffer);
	if (streamUmcCpu->action == ABORTAR_PROGRAMA)
		seguirEjecutando = FALSE;
}

//void asignar(t_puntero direccion_variable, t_valor_variable valor){
//	uint32_t pagina = direccion_variable / tamanioPagina;
//	uint32_t offset = direccion_variable % tamanioPagina;
//	almacenarBytesUMC(umc, pagina, offset, 4, valor);
//}

/*
 * obtenerValorCompartida
 *///YA ESTA TERMINADO
t_valor_variable obtenerValorCompartida(t_nombre_compartida variable) {
	StrCpuKer*streamCpuKer;
	streamCpuKer = newStrCpuKer(CPU_ID, OBTENER_VALOR_COMPARTIDA, pcbProceso,
			pcbProceso.id, sizeof(variable), variable,
			NULL /*NOMBRE DISPOSITIVO*/, 0 /*LEN NOMBRE DISPOSITIVO*/);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);
	buffer = socketReceive(socketNucleo->ptrSocket);
	StrKerCpu*streamKerCpu = unserializeKerCpu(buffer);
	if (streamKerCpu->action == OBTENER_VALOR_COMPARTIDA)
		return (atoi(streamKerCpu->data));
	else
		seguirEjecutando = FALSE;
	return -1;
}

/*
 * 	asignarValorCompartida
 */	// YA ESTÁ TERMINADA
t_valor_variable asignarValorCompartida(t_nombre_compartida variable,
		t_valor_variable valor) {
	char* buffer;
	StrCpuKer*streamCpuKer;
	streamCpuKer = newStrCpuKer(CPU_ID, ASIGNAR_VALOR_COMPARTIDA, pcbProceso,
			pcbProceso.id, valor /*LOGLEN*/,
			NULL /*LOG*/, variable /*NOMBRE DISPOSITIVO*/,
			strlen(variable) /*LEN NOMBRE DISPOSITIVO*/);
	buffer = socketReceive(socketNucleo->ptrSocket);
	StrKerCpu*streamKerCpu = unserializeKerCpu(buffer);
	if (streamKerCpu->action == ASIGNAR_VALOR_COMPARTIDA)
		return atoi(streamKerCpu->lenNomDispositivo);
	else {
		seguirEjecutando = FALSE;
		return -1;
	}
}

/*
 * 	irAlLabel
 */	// YA ESTA TERMINADA
void irAlLabel(t_nombre_etiqueta etiqueta) {
	pcbProceso.programCounter = metadata_buscar_etiqueta(etiqueta,
			pcbProceso.indiceDeEtiquetas, pcbProceso.etiquetaSize);
}

/*
 * llamarConRetorno
 */	// ESTÁ TERMINADA
void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {

	paginaDeStack* aux = malloc(sizeof(paginaDeStack));
	aux->retVars.pag = donde_retornar / tamanioPaginaUmc;
	aux->retVars.off = donde_retornar % tamanioPaginaUmc;
	aux->retPos = pcbProceso.programCounter;
	list_add(pcbProceso.indiceDelStack, aux);

	pcbProceso.programCounter = metadata_buscar_etiqueta(etiqueta,
			pcbProceso.indiceDeEtiquetas, pcbProceso.etiquetaSize);
}

/*
 * finalizar
 */	// FINALIZAR SOLO SI EL PROGRAMA TERMINA BIEN
void finalizar(void) {
	StrCpuKer*streamCpuKer;
	streamCpuKer = newStrCpuKer(CPU_ID, FINALIZAR_PROGRAMA, pcbProceso,
			pcbProceso.id, 0, NULL, NULL /*NOMBRE DISPOSITIVO*/,
			0 /*LEN NOMBRE DISPOSITIVO*/);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);
	seguirEjecutando = FALSE;
}

/*
 * retornar
 *
 */	// FALTA MANEJO DE ABORTO
void retornar(t_valor_variable retorno) {
	printf("Operacion de retorno");
	//todo EMI REVISA ESTO PORFA
	paginaDeStack *aux = list_remove(pcbProceso.indiceDelStack,
			(pcbProceso.indiceDelStack->elements_count) - 1);
	pcbProceso.programCounter = aux->retPos;
	StrCpuUmc*streamCpuUmc;
	asignadoVacio->numDePag = aux->retVars.pag;
	streamCpuUmc = newStrCpuUmc(CPU_ID, ALMACENAR_BYTES, *asignadoVacio,
			aux->retVars.off, 4, NULL, 0);
	SocketBuffer*buffer = serializeCpuUmc(streamCpuUmc);
	socketSend(socketUMC->ptrSocket, buffer);
	//////////////////////////////////////////////////////////////////
	buffer = socketReceive(socketUMC->ptrSocket);

	StrUmcCpu*streamUmcCpu;
	streamUmcCpu = unserializeCpuUmc(buffer);
	if (streamUmcCpu->action == ABORTAR_PROGRAMA)
		seguirEjecutando = FALSE;
}

/*
 * imprimir
 */	// YA ESTA
int imprimir(t_valor_variable valor_mostrar) {

	StrCpuKer*streamCpuKer;
	Byte* aux;
	sprintf(aux, "%d", valor_mostrar);

	//streamCpuKer = newStrCpuKer(CPU_ID, IMPRIMIR, pcbProceso, pcbProceso.id,
	//strlen(itoa(valor_mostrar)), itoa(valor_mostrar));
	streamCpuKer = newStrCpuKer(CPU_ID, IMPRIMIR, pcbProceso, pcbProceso.id,
			strlen(aux), aux, NULL /*NOMBRE DISPOSITIVO*/,
			0 /*LEN NOMBRE DISPOSITIVO*/);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);

	Byte* auxLoco;
	sprintf(auxLoco, "%d", valor_mostrar);

	//return strlen(itoa(valor_mostrar));
	return auxLoco;
}

/*
 * imprimirTexto
 */	// ya esta
int imprimirTexto(char* texto) {
	StrCpuKer*streamCpuKer;
	streamCpuKer = newStrCpuKer(CPU_ID, IMPRIMIRTEXTO, pcbProceso,
			pcbProceso.id, strlen(texto), texto, NULL /*NOMBRE DISPOSITIVO*/,
			0 /*LEN NOMBRE DISPOSITIVO*/);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);
	return strlen(texto);
}

/*
 * entradaSalida
 */
void entradaSalida(t_nombre_dispositivo dispositivo, int tiempo) {
	Byte * auxTiempo;
	sprintf(auxTiempo, "%d", tiempo);
	StrCpuKer*streamCpuKer;
//(Char id, Char action, pcb pcb, Int32U pid, Int32U logLen, Byte* log, Byte* nombreDispositivo, Int32U lenNomDispositivo)
	streamCpuKer = newStrCpuKer(CPU_ID, ENTRADA_SALIDA, pcbProceso,
			pcbProceso.id, strlen(auxTiempo), auxTiempo,
			dispositivo /*NOMBRE DISPOSITIVO*/,
			strlen(dispositivo) /*LEN NOMBRE DISPOSITIVO*/);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);
	seguirEjecutando = FALSE;
}

/*
 * wait
 */	//MODIFICAR PROGRAMA DESBLOQUEADO y envio
void wait(t_nombre_semaforo identificador_semaforo) {
	StrCpuKer*streamCpuKer;
	streamCpuKer = newStrCpuKer(CPU_ID, WAIT_SEM_ANSISOP, pcbProceso,
			pcbProceso.id, strlen(identificador_semaforo),
			identificador_semaforo, NULL /*NOMBRE DISPOSITIVO*/,
			0 /*LEN NOMBRE DISPOSITIVO*/);
	SocketBuffer*buffer = serializeCpuKer(streamCpuKer);
	socketSend(socketNucleo->ptrSocket, buffer);
	buffer = socketReceive(socketNucleo->ptrSocket);
	StrKerCpu*StreamKerCpu = unserializeKerCpu(buffer);
	if (StreamKerCpu->action == WAIT_REALIZADO) {
		seguirEjecutando = TRUE;
	} else
		seguirEjecutando = FALSE;

}

/*
 * signal
 */ // YA ESTA
void signale(t_nombre_semaforo identificador_semaforo) {
	StrCpuKer*streamCpuKer;
	streamCpuKer = newStrCpuKer(CPU_ID, 28 /*SIGNAL*/, pcbProceso,
			pcbProceso.id, strlen(identificador_semaforo),
			identificador_semaforo, NULL /*NOMBRE DISPOSITIVO*/,
			0 /*LEN NOMBRE DISPOSITIVO*/);
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
		signale, };

void senialParaAbortar() {
	signal(SIGUSR1, abortarCPU);
}

void mandarElAbortoAKernel(pcb * pcbActual) {
	SocketBuffer * buffer;
	StrCpuKer* sck;

	sck = newStrCpuKer(CPU_ID, ABORTAR_PROGRAMA, *pcbActual, 0, 0, NULL,
	NULL, 0);
	buffer = serializeCpuKer(sck);
	if (!socketSend(socketNucleo->ptrSocket, buffer)) {
		puts("No se pudo enviar el buffer al nucleo.");
	}
}

void abortarCPU() {
	abortoCPU = 1;
}

void mirarSenial(pcb * pcbActual) {
	if (abortoCPU == 1) {
		puts("Se recibio senial SIGUSR1\n");
		mandarElAbortoAKernel(pcbActual);
		puts("SIGUSR1 se trato correctamente\n");
		abortoCPU = 0;
	}
}

// Devuelve el logger para loggear los logs para poder loggear
t_log* getLogger() {
	if (logger == NULL) {
		logger = log_create(LOGGER_FILE, "cpu", FALSE, LOG_LEVEL_INFO);
	}
	return logger;
}
