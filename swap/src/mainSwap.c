#include "swap.h"

int main(void) {

    leerArchivoDeConfiguracion("swapconfig");


    /*Se conecta a la UMC con el swap. LOCO COMENTEN!!!*/
    /*Funciona una vez. Falla el bind en la segunda. Testeado by Dr.Mengueche*/
    int fdSocketUMC = crearSocketServidor(umcPort);
    escucharSocket(fdSocketUMC, 1);
    int fdSocketCliente= aceptarConexiones(fdSocketUMC);
    char* mensaje;
    mensaje=(char*)malloc(12*sizeof(char));
    if(mensaje==NULL){
    	puts("falló al reservar la memoria");
    	return -1;
    }
    recibirMensaje(fdSocketCliente,mensaje,12*sizeof(char));
    printf("%s",mensaje);

	printf("%d",fdSocketCliente);



	return EXIT_SUCCESS;
}
