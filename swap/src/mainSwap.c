#include "swap.h"

int main(void) {

    leerArchivoDeConfiguracion("swapconfig");

    /*Se conecta a la UMC con el swap. LOCO COMENTEN!!!*/
    /*Funciona una vez. Falla el bind en la segunda. Testeado by Dr.Mengueche*/
    int fdSocketUMC = crearSocketServidor(umcPort);
    escucharSocket(fdSocketUMC, 1);
    int fdSocketCliente= aceptarConexiones(fdSocketUMC);
    archivoMappeado=crearArchivo(tamArchivo,nombreSwap);



	return EXIT_SUCCESS;
}


//Creo espacioLibre
    espacioLibre * crearEspacioLibre (int inicio, int tamanio) {
    	espacioLibre * nuevoEspacioLibre = malloc(sizeof(espacioLibre));
    	nuevoEspacioLibre->inicio = inicio;
    	nuevoEspacioLibre->tamanio= tamanio;
    	return nuevoEspacioLibre;
    }

    void agregarEspacioLibre(int inicio, int swap_tamanio) {
    	espacioLibre * nuevoEspacioLibre = crearEspacioLibre (inicio, swap_tamanio);
    	list_add(listaEspacioLibre , nuevoEspacioLibre);
    }

  //Creo espacioAsignado
  espacioAsignado * crearEspacioAsignado (bool bitMap,int numDePag,int posicionDePag, int inicio, int tamanio, int pid) { //teoricamente tiene que estar el pid, lo que no creo es que este bien utilizado acá

  	espacioAsignado  * espacio = malloc(sizeof(espacioAsignado ));

  	espacio->bitMap= bitMap;
  	espacio->inicio = inicio;
  	espacio->tamanio= tamanio;
  	espacio->pid = pid;
    	espacio->numDePag= numDePag;
     	espacio->posicionDePag= posicionDePag;

     	return espacio;
    }

  void agregarEspacioAsigando(bool bitMap,int numDePag,int posicionDePag, int inicio, int tamanio, int pid) {
           	espacioAsignado * nuevoEspacioAsignado = crearEspacioLibre(bitMap,numDePag,posicionDePag,inicio,tamanio,pid);
           	list_add(listaEspacioLibre , nuevoEspacioAsignado);
           }


    int  verificarSiHayEspacio(int tamanio){
         int posicion;
         int totalDeEspacioLibre;


         for(posicion=0;posicion<list_size(listaEspacioLibre);posicion++){

        	 espacioLibre* espacio=(espacioLibre*)list_get(listaEspacioLibre,posicion);


		    if(espacio->tamanio >= tamanio) return posicion;

           totalDeEspacioLibre=+ espacio->tamanio;
         }
         if (totalDeEspacioLibre>= tamanio)return FRAGMENTACION_EXTERNA;

    return FAIL;
    }

    int recibirNuevoPrograma(int pid,int tamanio){
    	int espacioLibre=verificarSiHayEspacio(tamanio);
    	if(espacioLibre>=0){
    		return guardarPrograma(espacioLibre,pid, tamanio);
    	}
    	if (espacioLibre == FRAGMENTACION_EXTERNA ){
    		compactarEspacioLibre();
    		return 0;
    	}
    	if(espacioLibre == FAIL) return FAIL;

    	return 0;
    }

  int  guardarPrograma(posicion,pid,tamanio){
  espacioLibre* espacio = (espacioLibre)*list_get(listaEspacioLibre,posicion);

  return 0;
    }

/*

    //Creo listas de espacio libre y espacio asignado
     *
     * void crearListas () {
     * 		listaEspacioLibre = list_create ();
     * 		listaEspacioAsignado = list_create ();
     * 	}
     //Libero las listas
     * int espacio; //DUDO Y MUCHO QUE ESTE BIEN DEFINIDA
     * void borrarListaEspacioLibre (){
     * 		void liberarEspacioLibre (espacioLibre * espacio){
     * 				free(espacio);
     * 		}
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
