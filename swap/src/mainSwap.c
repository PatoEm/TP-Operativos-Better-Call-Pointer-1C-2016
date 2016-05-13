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
    	case RECIBIRTAMANIO :{//recibo tamaño de mi memoria y la inicializo
    		char* tamanioSwap= (char*)malloc(8*sizeof(char));
    		recibirMensaje(fdSocketCliente, tamanioSwap, 7*sizeof(char));
    		verificarMemoria(tamanioSwap);
    		tamanioSwap[7]=20;//centinela de c
    		int swap_tamanio;
    		swap_tamanio=atoi (tamanioSwap);
    		free(tamanioSwap);
    		system("dd if=/dev/zero of=miSwap bs= count=1");
    		break;
    	}

    		
    }

    /*

    //Creo listas de espacio libre y espacio asignado
     *
     * void crearListas () {
     * 		listaEspacioLibre = list_create ();
     * 		listaEspacioAsignado = list_create ();
     * 	};
     //Libero las listas
     * int espacio; //DUDO Y MUCHO QUE ESTE BIEN DEFINIDA
     * void borrarListaEspacioLibre (){
     * 		void liberarEspacioLibre (espacioLibre * espacio){
     * 				free(espacio);
     * 		};
     * 		list_destroy_and_destroy_elements(listaEspacioLibre*, void(*borrarListaEspacioLibre)(void*)); //es funcion de las commons de sisop
     * 		listaEspacioLibre = NULL;
     * 	}
     *
     * 	void borrarListaEspacioAsignado (){
     * 		void liberarEspacioAsignado (espacioAsignado * espacio){
     * 				free(espacio);
     * 		};
     * 		list_destroy_and_destroy_elements(listaEspacioAsignado*, void(*borrarListaEspacioAsignado)(void*));
     * 		listaEspacioAsignado = NULL;
     * 	}
     *
     //Creo espacioLibre
      * espacioLibre * crearEspacioLibre (int inicio, int swap_tamanio) {
      * 	espacioLibre * nuevoEspacioLibre = malloc(sizeof(espacioLibre));
      * 	nuevoEspacioLibre->inicio = inicio;
      * 	nuevoEspacioLibre->swap_tamanio = swap_tamanio;
      * 	return nuevoEspacioLibre;
      * }
      *
      * void agregarEspacioLibre(int inicio, int swap_tamanio) {
      * 	espacioLibre * nuevoEspacioLibre = crearEspacioLibre (inicio, swap_tamanio);
      * 	list_add(listaEspacioLibre *, void *nuevoEspacioLibre); //Tambien está en las commons
      * }
      *
      //Lo mismo con espacioAsignado
      * espacioAsignado * crearEspacioAsignado (int pid, int inicio, int swap_tamanio) { //teoricamente tiene que estar el pid, lo que no creo es que este bien utilizado acá
      * 	espacioAsignado * espacio = malloc(sizeof(espacioAsignado));
      * 	espacio->pid = pid;
      * 	espacio->inicio = inicio;
      * 	espacio->swap_tamanio = swap_tamanio;
      * 	return espacio;
      * }
      *
      * void agregarEspacioAsignado(int pid, int inicio, int swap_tamanio) {
      * 	espacioAsignado * espacio = crearEspacioAsignado (pid, inicio, swap_tamanio);
      * 	list_add(listaEspacioAsignado *, void *espacio);
      * }
      *
      //Verifico si voy a tener espacio
       * int hayEspacio (int swap_tamanio){
       * 	int i, tamanioLibre = 0;
       * 	for (i = 0; list_size(listaEspacioLibre *); i++) { //list_size retorna el tam de una lista
       * 		espacioLibre * espacio = (espacioLibre *);
       * 		list_get (listaEspacioLibre *, i); //Retorna el contenido de la posicion i
       * 		if (espacio->swap_tamanio){
       * 			return 1;
       * 		}
       * 		tamanioLibre += espacio->swap_tamanio;
       * 	}
       * 	if (tamanioLibre >= swap_tamanio) {
       * 		return FRAGMENTACION_EXTERNA; //esta definido en el .h como -2
       * 	}
       * 	return FAIL;
       * }
     */




    printf("%s",mensaje);

	printf("%d",fdSocketCliente);



	return EXIT_SUCCESS;
}
