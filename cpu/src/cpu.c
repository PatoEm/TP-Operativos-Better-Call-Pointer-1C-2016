/*
 * cpu.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */
#include "cpu.h"
#define manejarError(msg) {perror(msg); abort();}

//Creo structs necesarias para el parser BY DRMENGUECHE


void setearValores(t_config * archivoConfig) {
		ipUMC = config_get_string_value(archivoConfig, "IP_UMC");
		umcPort = config_get_string_value(archivoConfig, "PUERTO_UMC");
		ipNucleo = config_get_string_value(archivoConfig, "IP_NUCLEO");
		nucleoPort = config_get_string_value(archivoConfig, "PUERTO_NUCLEO");

}

t_puntero definirVariable(t_nombre_variable variable){

	printf("defino una variable");

}

t_puntero obtenerPosicionVariable(t_nombre_variable variable){

	printf("devuelvo la posición de una variable");

}

t_valor_variable dereferenciar(t_nombre_variable variable){

	printf("dereferencio una variable");

}

void asignar(t_puntero puntero, t_valor_variable variable){

	printf("asigno una variable");

}

t_valor_variable obtenerValorCompartida(t_nombre_compartida variable){

	printf("devuelvo el valor de una variable compartida");

}

t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor){

	printf("asigno una valorcito");
}

t_puntero_instruccion irAlLabel(t_nombre_etiqueta etiqueta){

	printf("Voy al label");

}

t_puntero_instruccion llamarFuncion(t_nombre_etiqueta etiqueta, t_puntero donde_retornar,
t_puntero_instruccion linea_en_ejecuccion){

	printf("llamaron a una instrucción");

}

t_puntero_instruccion retornar(t_valor_variable retorno){

	printf("retorno un estado");

}

// ya implementada ByDRMENGUECHE
int imprimir(t_valor_variable valor_mostrar){

	char* mensaje=string_new();
	char protocolo[3]="01";
	memcpy(mensaje,&protocolo,2);
	memcpy(mensaje[2],&valor_mostrar,4);
	enviarMensaje(clienteNucleo,mensaje,6);

}

// ya casi implementada ByDRMENGUECHE
int imprimirTexto(char* texto){

	printf("se imprime un texto");
	char* mensaje=string_new();
	char protocolo[3]="02";
	int tamanioDelTexto = (string_length(texto));
	memcpy(mensaje,&protocolo,2);
	memcpy(mensaje[2],&tamanioDelTexto,4);
	memcpy(mensaje[6],texto,string_length(texto));
	enviarMensaje(clienteNucleo,mensaje,6+string_length(texto));

}

int entradaSalida(t_nombre_dispositivo dispositivo, int tiempo){

	printf("operación de entrada y salida");

}

int wait(t_nombre_semaforo identificador_semaforo){

	printf("operación privilegiada wait");

}

int signal(t_nombre_semaforo identificador_semaforo){

	printf("operación privilegiada signal");
}

AnSISOP_funciones funciones = {
		.AnSISOP_definirVariable		= definirVariable,
		.AnSISOP_obtenerPosicionVariable= obtenerPosicionVariable,
		.AnSISOP_dereferenciar			= dereferenciar,
		.AnSISOP_asignar				= asignar,
		.AnSISOP_obtenerValorCompartida = obtenerValorCompartida,
		.AnSISOP_asignarValorCompartida = asignarValorCompartida,
		.AnSISOP_irAlLabel				= irAlLabel,
		.AnSISOP_llamarConRetorno		= llamarFuncion,
		.AnSISOP_retornar				= retornar,
		.AnSISOP_entradaSalida			= entradaSalida,
		.AnSISOP_imprimir				= imprimir,
		.AnSISOP_imprimirTexto			= imprimirTexto,

};
AnSISOP_kernel funcionesDeKernel= {
		.AnSISOP_wait			= wait,
		.AnSISOP_signal			= signal,
};

