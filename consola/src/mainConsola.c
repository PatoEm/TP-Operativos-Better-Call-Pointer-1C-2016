/*
 ============================================================================
 Name        : consola.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "consola.h"


int main(void) {

	//lee archivo de configuración y se conecta al nucleo. COMENTEMOS QUE HACEMOS
	 leerArchivoDeConfiguracion("configconsola");
	 

	int fdSocketConsola = crearCliente(ipNucleo,nucleoPort );
	//int fdSocketConsola = crearCliente("127.0.0.1","6002" );
	enviarMensaje(fdSocketConsola,"Consola Ok",11*sizeof(char));


	//Pido  direccion de archivo ANSISOP

	char* direccionDeArchivo;
	printf("Ingrese archivo: ");
    	scanf("%s",direccionDeArchivo);

    	//Obtengo el tamaño y contenido del archivo
    	leerProgramaAnsisop(direccionDeArchivo);
		//puts("hola");


	//Envio la longitud del archivo
	enviarMensaje(fdSocketConsola, tamanio, sizeof(long), 0);


	//Envio el contenido del archivo
    	enviarMensaje(fdSocketConsola, buffer, tamanio+1);


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

		/*case IMPRIMIR: ;

			char* variable;
			int valorDeVariable;

			estadoOpe= recibirMensaje(fdSocketConsola, &variable, sizeof(char*));
			estadoOpe = recibirMensaje(fdSocketConsola, valorDeVariable, sizeof(int));

			for(int i =0; i< ; i++){
				if(estadoOpe) printf("VARIABLE: %d \n%d \n"", variable,valorDeLaVariable);
			}
		break; */ //ESTO NOSE MUY BIEN COMO SE HACE
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



