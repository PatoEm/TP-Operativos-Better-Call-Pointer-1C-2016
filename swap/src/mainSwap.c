#include "swap.h"

int main(void) {

    leerArchivoDeConfiguracion("swapconfig");


    /*Se conecta a la UMC con el swap. LOCO COMENTEN!!!*/
    /*Funciona una vez. Falla el bind en la segunda. Testeado by Dr.Mengueche*/
    int codigoDeMensaje;
    int fdSocketUMC = crearSocketServidor(umcPort);
    escucharSocket(fdSocketUMC, 1);
    int fdSocketCliente= aceptarConexiones(fdSocketUMC);
    char* mensaje;
    mensaje=(char*)malloc(3*sizeof(char));
    verificarMemoria(mensaje);
    recibirMensaje(fdSocketCliente,mensaje,2*sizeof(char));
    mensaje[2]=20;//Centinela en c
    codigoDeMensaje=atoi(mensaje);
    free(mensaje);
    switch(codigoDeMensaje){
    	case RECIBIRTAMANIO;//recibo tamaño de mi memoria y la inicializo
    		char* tamanioSwap= (char*)malloc(8*sizeof(char));
    		recibirMensaje(fdSocketCliente, tamanioSwap, 7*sizeof(char));
    		verificarMemoria(tamanioSwap);
    		tamanioSwap[7]=20;//centinela de c


    }


    printf("%s",mensaje);

	printf("%d",fdSocketCliente);



	return EXIT_SUCCESS;
}
