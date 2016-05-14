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
