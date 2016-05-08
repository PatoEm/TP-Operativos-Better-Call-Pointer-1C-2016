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




	//Reservo memoria y Pido  direccion de archivo ANSISOP

	char* paqueteAEnviar;
	char* direccionDeArchivo = (char *) malloc(150);
	verificarMemoria(direccionDeArchivo);

	printf("Ingrese archivo: ");
    scanf("%s",direccionDeArchivo);


    //Obtengo el tamaño
    tamArchivo(direccionDeArchivo);

    //Obtengo el contMienido del archivo
    leerProgramaAnSISOP(direccionDeArchivo);
    paqueteAEnviar=(char* )malloc(tamanio+sizeof(char)*6);
    verificarMemoria(paqueteAEnviar);
    string_append(&paqueteAEnviar,"00");
    string_append(&paqueteAEnviar,string_itoa(tamanio));
    string_append(&paqueteAEnviar,buffer);

	enviarMensaje(fdSocketConsola,paqueteAEnviar, tamanio+sizeof(char)*7);
	free(paqueteAEnviar);


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
