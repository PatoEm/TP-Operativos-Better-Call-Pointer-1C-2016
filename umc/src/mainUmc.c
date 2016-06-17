/*
 ============================================================================
 Name        : umc.c

BETTER CALL POINTER PERRO
 ============================================================================
 */
#include "umc.h"

int main(void) {

	leerArchivoDeConfiguracion("configumc");
	crearLogger(0);

	puts("Inicio UMC");
	log_info(logger, "Inicio Log UMC", NULL);

	t_list * TLB;
	int aciertosTLB;
	int accesosTLB;
	pthread_t hiloComandos;
	pthread_attr_t attrhiloComandos;

	memoriaReal = reservarMemoria(marcos, marco_Size); //Fabrico mi memoria real

	menuUMC(hiloComandos, attrhiloComandos);

	inicioTLB(TLB, aciertosTLB, accesosTLB);

	escuchoMuchasConexiones();

/*
	int fdSocketNucleo= crearSocketServidor(puertoEscucha);
	escucharSocket(fdSocketNucleo,1);
	int aceptarNucleo=aceptarConexiones(fdSocketNucleo);
*/
	//no tengo idea para que sirve esto, lo puedo romper? COMENTEMOS QUE HACEMOS

/*
	int fdSocketCPU= crearSocketServidor(puertoEscucha);
	escucharSocket(fdSocketCPU,5);
	int conexionCPU = aceptarConexiones(fdSocketCPU);
	char* mensaje;
	mensaje=(char*)malloc(11*sizeof(char));
	if(mensaje==NULL){
		puts("falló al reservar la memoria");
		return -1;
	}
	recibirMensaje(conexionCPU,mensaje,11*sizeof(char));
	puts(mensaje);
*/

	int fdSocketUMC = crearCliente(ip_Swap,puerto_Swap);
	//enviarMensaje(fdSocketUMC,mensaje,11*sizeof(char));




	//printf("el socket de la umc es:%d y el del nucleo es: %d",fdSocketUMC, aceptarNucleo);

	liberarMemoria(memoriaReal); //Una vez terminado, libero toda mi memoria real
	log_destroy(logger);

	return EXIT_SUCCESS;

}
