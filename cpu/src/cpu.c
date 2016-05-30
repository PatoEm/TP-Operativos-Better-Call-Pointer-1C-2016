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
	FunctionParameters* fparamDefinirVariable = malloc(sizeof(FunctionParameters));
	fparamDefinirVariable->param[0] = T_NOMBRE_VARIABLE;
	fparamDefinirVariable->param[1] = NONE;
	fparamDefinirVariable->func = definirVariable;
	dictionary_put(*functionParameters, "DEFINIR_VARIABLE", fparamDefinirVariable);

	//02
	FunctionParameters* fparamObtenerPosicionVariable = malloc(sizeof(FunctionParameters));
	fparamObtenerPosicionVariable->param[0] = T_NOMBRE_VARIABLE;
	fparamObtenerPosicionVariable->param[1] = NONE;
	fparamObtenerPosicionVariable->func = obtenerPosicionVariable;
	dictionary_put(*functionParameters, "OBTENER_POSICION_VARIABLE", fparamObtenerPosicionVariable);

	//03
	FunctionParameters* fparamDereferenciar = malloc(sizeof(FunctionParameters));
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
	FunctionParameters* fparamObtenerValorCompartida = malloc(sizeof(FunctionParameters));
	fparamObtenerValorCompartida->param[0] = T_NOMBRE_COMPARTIDA;
	fparamObtenerValorCompartida->param[1] = NONE;
	fparamObtenerValorCompartida->func = obtenerValorCompartida;
	dictionary_put(*functionParameters, "OBTENER_VALOR_COMPARTIDA", fparamObtenerValorCompartida);

	//06
	FunctionParameters* fparamAsignarValorCompartida = malloc(sizeof(FunctionParameters));
	fparamAsignarValorCompartida->param[0] = T_NOMBRE_COMPARTIDA;
	fparamAsignarValorCompartida->param[1] = T_VALOR_VARIABLE;
	fparamAsignarValorCompartida->func = asignarValorCompartida;
	dictionary_put(*functionParameters, "ASIGNAR_VALOR_COMPARTIDA", fparamAsignarValorCompartida);

	//07
	FunctionParameters* fparamIrAlLabel = malloc(sizeof(FunctionParameters));
	fparamIrAlLabel->param[0] = T_NOMBRE_ETIQUETA;
	fparamIrAlLabel->param[1] = NONE;
	fparamIrAlLabel->func = irAlLabel;
	dictionary_put(*functionParameters, "IR_A_LABEL", fparamIrAlLabel);

	//08
	FunctionParameters* fparamLlamarConRetorno = malloc(sizeof(FunctionParameters));
	fparamLlamarConRetorno->param[0] = T_NOMBRE_ETIQUETA;
	fparamLlamarConRetorno->param[1] = T_PUNTERO;
	fparamLlamarConRetorno->func = llamarConRetorno;
	dictionary_put(*functionParameters, "LLAMAR_CON_RETORNO", fparamLlamarConRetorno);

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
	FunctionParameters* fparamImprimirTexto = malloc(sizeof(FunctionParameters));
	fparamImprimirTexto->param[0] = CHAR_AST;
	fparamImprimirTexto->param[1] = NONE;
	fparamImprimirTexto->func = imprimirTexto;
	dictionary_put(*functionParameters, "IMPRIMIR_TEXTO", fparamImprimirTexto);

	//12
	FunctionParameters* fparamEntradaSalida = malloc(sizeof(FunctionParameters));
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

/*******************************************************
 * FUNCIONES AnSISOP
 *******************************************************/

/*
 * definirVariable
 */
t_puntero definirVariable(t_nombre_variable identificador_variable){
	printf("Operacion definir variable");
	return 0;
}

/*
 * obtenerPosicionVariable
 */
t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable){
	printf("Operacion obtener posicion variable");
	return 0;
}

/*
 * dereferenciar
 */
t_valor_variable dereferenciar(t_puntero direccion_variable){
	printf("Operacion dereferenciar");
	return 0;
}

/*
 * asignar
 */
void asignar(t_puntero direccion_variable, t_valor_variable valor){
	printf("Operacion asignar");
}

/*
 * obtenerValorCompartida
 */
t_valor_variable obtenerValorCompartida(t_nombre_compartida variable){
	printf("Operacion obtener valor variable compartida");
	return 0;
}

/*
 * 	asignarValorCompartida
 */
t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor){
	printf("Operacion asignar valor variable compartida");
	return 0;
}

/*
 * 	irAlLabel
 */
t_puntero_instruccion irAlLabel(t_nombre_etiqueta etiqueta){
	printf("Operacion ir al label");
	return 0;
}

/*
 * llamarSinRetorno
 */
void llamarSinRetorno(t_nombre_etiqueta etiqueta){
	printf("Operacion llamar sin retorno");
}

/*
 * llamarConRetorno
 */
void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar){
	printf("Operacion llamar con retorno");
}

/*
 * finalizar
 */
void finalizar(void){

}

/*
 * retornar
 *
 */
t_puntero_instruccion retornar(t_valor_variable retorno){
	printf("Operacion de retorno");
	return 0;
}

/*
 * imprimir
 */
int imprimir(t_valor_variable valor_mostrar){
	char* mensaje=string_new();
	char protocolo[3]="01";
	memcpy(mensaje,&protocolo,2);
	memcpy(mensaje[2],&valor_mostrar,4);
	enviarMensaje(clienteNucleo,mensaje,6);
}

/*
 * imprimirTexto
 */
int imprimirTexto(char* texto){
	printf("se imprime un texto");
	char* mensaje=string_new();
	char protocolo[3]="02";
	int tamanioDelTexto = (string_length(texto));
	memcpy(mensaje,&protocolo,2);
	memcpy(mensaje[2],&tamanioDelTexto,4);
	memcpy(mensaje[6],texto,string_length(texto));
	enviarMensaje(clienteNucleo,mensaje,6+string_length(texto));
	return 0;
}

/*
 * entradaSalida
 */
int entradaSalida(t_nombre_dispositivo dispositivo, int tiempo){
	printf("Operacion de entrada y salida");
	return 0;
}

/*
 * wait
 */
int wait(t_nombre_semaforo identificador_semaforo){
	printf("Operacion privilegiada wait");
	return 0;
}

/*
 * signal
 */
int signal(t_nombre_semaforo identificador_semaforo){
	printf("Operacion privilegiada signal");
	return 0;
}


AnSISOP_funciones funciones = {
		.AnSISOP_definirVariable			= definirVariable,
		.AnSISOP_obtenerPosicionVariable	= obtenerPosicionVariable,
		.AnSISOP_dereferenciar				= dereferenciar,
		.AnSISOP_asignar					= asignar,
		.AnSISOP_obtenerValorCompartida 	= obtenerValorCompartida,
		.AnSISOP_asignarValorCompartida 	= asignarValorCompartida,
		.AnSISOP_irAlLabel					= irAlLabel,
		.AnSISOP_llamarConRetorno			= llamarConRetorno,
		.AnSISOP_retornar					= retornar,
		.AnSISOP_entradaSalida				= entradaSalida,
		.AnSISOP_imprimir					= imprimir,
		.AnSISOP_imprimirTexto				= imprimirTexto,
};

AnSISOP_kernel funcionesDeKernel= {
		.AnSISOP_wait			= wait,
		.AnSISOP_signal			= signal,
};

