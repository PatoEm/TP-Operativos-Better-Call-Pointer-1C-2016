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

//Genero mi archivo de Swap y lo devuelvo mappeado en memoria DRMENGUECHE
char* crearArchivo(char* tamanio,char* nombre){

	char* paraSistema=string_new();
	string_append(&paraSistema,"dd if=/dev/zero of=");
	string_append(&paraSistema,nombre);
	string_append(&paraSistema," bs=");
	string_append(&paraSistema,tamanio);
	string_append(&paraSistema," count=1");
	system(paraSistema);
	return mappearArchivo(nombre);

}

// mappear el archivoo DRMENGUECHE

void* mappearArchivo(char* filename) {

	char *addr;
	int fd;
	struct stat sb;
	size_t length;

	fd = open(filename, O_RDWR);
	if (fd == -1)
		handle_error("open");

	if (fstat(fd, &sb) == -1)
		handle_error("fstat");

	length = sb.st_size;
	addr = mmap(NULL, length, PROT_READ | PROT_WRITE,MAP_SHARED | MAP_NORESERVE, fd, 0);
	if (addr == MAP_FAILED)
		handle_error("mmap");
	return addr;

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
