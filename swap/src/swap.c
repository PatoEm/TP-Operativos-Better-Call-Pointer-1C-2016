/*
* swap.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#include "swap.h"

//#define manejarError(msg) {perror(msg); abort();}


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

	length = sb.st_size;
	addr = mmap(NULL, length, PROT_READ | PROT_WRITE,MAP_SHARED | MAP_NORESERVE, fd, 0);

	return addr;

}



void setearValores(t_config * archivoConfig) {
		umcPort = config_get_string_value(archivoConfig, "PUERTO_UMC");
		nombreSwap = config_get_string_value(archivoConfig, "NOMBRE_SWAP");
		paginas = config_get_string_value(archivoConfig, "CANTIDAD_PAGINAS");
		tamPagina = config_get_string_value(archivoConfig, "TAMANO_PAGINA");
		retCompactacion = config_get_string_value(archivoConfig, "RETARDO_COMPACTACION");
		tamArchivo=config_get_string_value(archivoConfig, "TAMANO_ARCHIVO");
}


int calcularIDPagina(inicio){

	if(inicio==0)return 0;
	else{
		return (inicio/(atoi(tamPagina)));
	}

}
//Creo espacioLibre
espacioLibre * crearEspacioLibre (int inicio) {
	espacioLibre * nuevoEspacioLibre = malloc(sizeof(espacioLibre));
	nuevoEspacioLibre->inicio = inicio;
	nuevoEspacioLibre->tamanio= atoi(tamPagina);
	nuevoEspacioLibre->IDPaginaInterno=calcularIDPagina(inicio);
	return nuevoEspacioLibre;
}

void agregarEspacioLibre(int inicio) {
	espacioLibre * nuevoEspacioLibre = crearEspacioLibre (inicio);
	list_add(listaEspacioLibre , nuevoEspacioLibre);
}

//Creo espacioAsignado
espacioAsignado * crearEspacioAsignado (bool bitMap,int numDePag,int posicionDePag, int pid) { //teoricamente tiene que estar el pid, lo que no creo es que este bien utilizado acá

	espacioAsignado  * espacio = malloc(sizeof(espacioAsignado ));


	espacio->bitMap= bitMap;
	espacio->tamanio= atoi(tamPagina);
	espacio->IDPaginaInterno=calcularIDPagina(posicionDePag);// nro de página en el swap
	espacio->pid = pid;
	espacio->numDePag= numDePag;//número de página en el programa
	espacio->posicionDePag= posicionDePag;// donde comienza mi página en el swap

	return espacio;
}

void agregarEspacioAsigando(bool bitMap,int numDePag,int posicionDePag, int tamanio, int pid) {
       	espacioAsignado * nuevoEspacioAsignado = crearEspacioAsignado(bitMap,numDePag,posicionDePag,pid);
       	list_add(listaEspacioLibre , nuevoEspacioAsignado);
       }


bool  verificarSiHayEspacio(int cantidadDePaginas){
   if(cantidadDePaginas <= list_size(listaEspacioLibre))return TRUE;
   else return FALSE;
}


//me dice si tengo que compactar. -1 hay que compactar, siino devuelve a partir de que pagina puedo usar
int paginasContiguasDeSwap(int cantidadDePaginas){
		int contadorDePaginasSeguidas=1;//tiene que ser igual a cantidadDePaginas para devolver una página determinada
		int miPaginaLibre=0;//me da el ID de la página libre actual
		int paginaActual=0;//me dice en que página estoy actualmente
		espacioLibre* nodoActual;//el nodo que voy a ir iterando
		while((paginaActual<=list_size(listaEspacioLibre))||(cantidadDePaginas==contadorDePaginasSeguidas)){
			nodoActual=list_get(listaEspacioLibre,paginaActual);// si es menor a la lista o conseguí la cantidad de páginas que buscaba hago esto
			if(miPaginaLibre+1==nodoActual->IDPaginaInterno){
				contadorDePaginasSeguidas++;//si consigo paginas seguidas sumo una
			}
			else{
				contadorDePaginasSeguidas=1;//sino vuelvo a 0
				miPaginaLibre=nodoActual->IDPaginaInterno;//reasigno la página para que sea la correcta
			}
		paginaActual++;
		}
		if(cantidadDePaginas==contadorDePaginasSeguidas)return (miPaginaLibre);
		else return -1;
}

void reservarPaginas(paginaDeComienzo,pid,cantidadDePaginas){
	 int paginaActual=paginaDeComienzo;// en donde empieza todo
	 int contadorDePaginas=0;//cuento para el while
	 espacioAsignado* paginasAReservar;//nodo para agarrar cosas
	 espacioAsignado unNodoPiola;
	 paginasAReservar=(&unNodoPiola);
	 (paginasAReservar->pid)=pid;
	 (paginasAReservar->bitMap)=0;
	 while(contadorDePaginas<cantidadDePaginas){
		 //ver este while con ideas más claras mañana

	 }

   }

//Recibe un nuevo programa. 1 si lo puede recibir, 0 si no
bool recibirNuevoPrograma(int pid,int cantidadDePaginasAGuardar){
	if(verificarSiHayEspacio(cantidadDePaginasAGuardar)){
		if(paginasContiguasDeSwap(cantidadDePaginasAGuardar)==-1){
			compactarSwap();//Tengo que seguir desde acá DR Mengueche

			//reservarPaginas();
			return TRUE;
		}
		else{
			reservarPaginas(paginasContiguasDeSwap(cantidadDePaginasAGuardar),pid,cantidadDePaginasAGuardar);
			return TRUE;
		}
	}
	else return FALSE;
}


//Creo listas de espacio libre y espacio asignado

  void crearListas () {
  		listaEspacioLibre = list_create ();
  		listaEspacioAsignado = list_create ();
 }

  void compactarSwap(){

	  int paginasContiguas=0;
	  espacioAsignado* nodoActual;
	  int contadorParaCadenaActual;
	  int contadorParaCadenaVieja;
	  espacioLibre* nodoLibre;
	  nodoActual=list_get(listaEspacioAsignado,paginasContiguas);
	  do{
		  if((nodoActual ->IDPaginaInterno)!= paginasContiguas){
			  contadorParaCadenaActual=paginasContiguas*atoi(tamPagina);
			  contadorParaCadenaVieja= paginasContiguas*(atoi(tamPagina)+1);
			  while(contadorParaCadenaActual<paginasContiguas*(atoi(tamPagina)+1)){
				  archivoMappeado[contadorParaCadenaActual]=archivoMappeado[contadorParaCadenaVieja];
				  archivoMappeado[contadorParaCadenaVieja]='\0';
				  contadorParaCadenaActual++;
				  contadorParaCadenaVieja++;
			  }
			 (nodoActual->IDPaginaInterno)= paginasContiguas;
			 (nodoActual->posicionDePag )= paginasContiguas*atoi(tamPagina);
		  }
		  paginasContiguas++;
		  nodoActual=list_get(listaEspacioAsignado,paginasContiguas);

	  }while(paginasContiguas<=list_size(listaEspacioAsignado));
	  list_clean(listaEspacioLibre);
	  paginasContiguas--;
	  nodoActual=list_get(listaEspacioAsignado,paginasContiguas);
	  (nodoLibre->inicio)=(nodoActual->posicionDePag)+atoi(tamPagina);
	  (nodoLibre->tamanio)=atoi(tamPagina);
	  (nodoLibre->IDPaginaInterno) = calcularIDPagina(nodoLibre->inicio);
	  list_add(listaEspacioLibre,nodoLibre);
	  while((nodoLibre->IDPaginaInterno )!= atoi(paginas)){

		  (nodoLibre->IDPaginaInterno)=((nodoLibre->IDPaginaInterno)+1);
		  (nodoLibre->inicio)=atoi(tamPagina)*(nodoLibre->IDPaginaInterno);
		  list_add(listaEspacioLibre,nodoLibre);

	  }
usleep(1000*atoi(retCompactacion));
  }
