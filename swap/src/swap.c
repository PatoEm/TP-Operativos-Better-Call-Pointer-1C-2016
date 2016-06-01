/*
 * swap.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#include "swap.h"

//#define manejarError(msg) {perror(msg); abort();}

//Genero mi archivo de Swap y lo devuelvo mappeado en memoria DRMENGUECHE
char* crearArchivo(char* tamanio, char* nombre) {
	char* paraSistema = string_new();
	string_append(&paraSistema, "dd if=/dev/zero of=");
	string_append(&paraSistema, nombre);
	string_append(&paraSistema, " bs=");
	string_append(&paraSistema, tamanio);
	string_append(&paraSistema, " count=1 \n");
	system(paraSistema);
	return mappearArchivo(nombre);
	puts("hola");

}

// mappear el archivoo DRMENGUECHE

void* mappearArchivo(char* filename) {

	char *addr;
	int fd;
	struct stat sb;
	size_t length;

	fd = open(filename, O_RDWR);
	if (fd == -1)
		printf("error 3"); //acá van errores

	if (fstat(fd, &sb) == -1) /* To obtain file size */
		printf("error 3"); //acá van errores

	length = sb.st_size;
	addr = mmap(NULL, length, PROT_READ | PROT_WRITE,
	MAP_SHARED | MAP_NORESERVE, fd, 0);
	if (addr == MAP_FAILED)
		printf("error 3"); //acá van errores
	return addr;

}

void setearValores(t_config * archivoConfig) {
	umcPort = config_get_string_value(archivoConfig, "PUERTO_UMC");
	nombreSwap = config_get_string_value(archivoConfig, "NOMBRE_SWAP");
	paginas = config_get_string_value(archivoConfig, "CANTIDAD_PAGINAS");
	tamPagina = config_get_string_value(archivoConfig, "TAMANO_PAGINA");
	retCompactacion = config_get_string_value(archivoConfig,
			"RETARDO_COMPACTACION");
	tamArchivo = config_get_string_value(archivoConfig, "TAMANO_ARCHIVO");
}

bool escribirPagina(int pid, int numeroDePagina, char*pagina) {
	espacioAsignado* nodoALeer;
	int posicionActualDeNodo = 0;
	nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	while (((nodoALeer->pid) != pid) && (nodoALeer->numDePag != numeroDePagina)) {
		posicionActualDeNodo++;
		nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	}
	nodoALeer->bitMap = 0;
	list_replace(listaEspacioAsignado, posicionActualDeNodo, nodoALeer);
	int dondeEscribo = nodoALeer->posicionDePag;
	int enDondeEstoyDeLoQueMeMandaron = 0;
	while (enDondeEstoyDeLoQueMeMandaron < atoi(tamPagina)) {
		archivoMappeado[dondeEscribo] = pagina[enDondeEstoyDeLoQueMeMandaron];
		dondeEscribo++;
		enDondeEstoyDeLoQueMeMandaron++;
	}
	return TRUE;
}

char* leerUnaPagina(int pid, int numeroDePagina) {
	char paginaLeida[5] = "-2";
	char*punteroAPaginaLeida = (&paginaLeida[0]);
	espacioAsignado* nodoALeer;
	int posicionActualDeNodo = 0;
	nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	while (((nodoALeer->pid) != pid) && (nodoALeer->numDePag != numeroDePagina)) {
		posicionActualDeNodo++;
		nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	}
	if ((nodoALeer->bitMap) == 1) {
		return (punteroAPaginaLeida);
	} else {
		int lugarDeLaCadena = 0;
		nodoALeer->bitMap = 1;
		char paginaADevolver[atoi(tamPagina)];
		char*punteroADevolver = (&paginaADevolver[0]);
		int posicionDeChar = (nodoALeer->posicionDePag);
		while (posicionDeChar < (nodoALeer->posicionDePag) + atoi(tamPagina)) {
			paginaADevolver[lugarDeLaCadena] = archivoMappeado[posicionDeChar];
			posicionDeChar++;
			lugarDeLaCadena++;
		}
		return (punteroADevolver);
	}
}

int calcularIDPagina(int inicio) {

	if (inicio == 0)
		return 0;
	else {
		return (inicio / atoi(tamPagina));
	}

}

//después de crear archivo de configuración
void iniciarEstructurasSwap() {
	int counter = 0;
	int inicio = 0;
	crearListas();
	while (counter != atoi(paginas)) {
		agregarEspacioLibre(inicio);
		counter++;
		inicio = inicio + atoi(tamPagina);
	}

}

//Creo espacioLibre
espacioLibre * crearEspacioLibre(int inicio) {
	espacioLibre * nuevoEspacioLibre = malloc(sizeof(espacioLibre));
	nuevoEspacioLibre->inicio = inicio;
	nuevoEspacioLibre->tamanio = atoi(tamPagina);
	nuevoEspacioLibre->IDPaginaInterno = calcularIDPagina(inicio);
	return nuevoEspacioLibre;
}

void agregarEspacioLibre(int inicio) {
	espacioLibre * nuevoEspacioLibre = crearEspacioLibre(inicio);
	list_add(listaEspacioLibre, nuevoEspacioLibre);
}

//Creo espacioAsignado
espacioAsignado * crearEspacioAsignado(bool bitMap, int numDePag,
		int posicionDePag, int pid) { //teoricamente tiene que estar el pid, lo que no creo es que este bien utilizado acá

	espacioAsignado * espacio = malloc(sizeof(espacioAsignado));

	espacio->bitMap = bitMap; //1 sucio 0 limpia
	espacio->tamanio = atoi(tamPagina);
	espacio->IDPaginaInterno = calcularIDPagina(posicionDePag); // nro de página en el swap
	espacio->pid = pid;
	espacio->numDePag = numDePag; //número de página en el programa
	espacio->posicionDePag = posicionDePag; // donde comienza mi página en el swap

	return espacio;
}

void agregarEspacioAsigando(bool bitMap, int numDePag, int posicionDePag,
		int tamanio, int pid) {
	espacioAsignado * nuevoEspacioAsignado = crearEspacioAsignado(bitMap,
			numDePag, posicionDePag, pid);
	list_add(listaEspacioLibre, nuevoEspacioAsignado);
}

bool verificarSiHayEspacio(int cantidadDePaginas) {
	if (cantidadDePaginas <= list_size(listaEspacioLibre))
		return TRUE;
	else
		return FALSE;
}

//me dice si tengo que compactar. -1 hay que compactar, siino devuelve a partir de que pagina puedo usar
int paginasContiguasDeSwap(int cantidadDePaginas) {
	int contadorDePaginasSeguidas = 1; //tiene que ser igual a cantidadDePaginas para devolver una página determinada
	int miPaginaLibre = 0; //me da el ID de la página libre actual
	int paginaActual = 0; //me dice en que página estoy actualmente
	espacioLibre* nodoActual; //el nodo que voy a ir iterando
	nodoActual = list_get(listaEspacioLibre, paginaActual);
	miPaginaLibre=nodoActual->IDPaginaInterno;
	while ((paginaActual <= list_size(listaEspacioLibre))) { // si es menor a la lista o conseguí la cantidad de páginas que buscaba hago esto
		if (cantidadDePaginas == contadorDePaginasSeguidas)
			return (miPaginaLibre);
		paginaActual++;
		nodoActual = list_get(listaEspacioLibre, paginaActual);
		if (miPaginaLibre + 1 == nodoActual->IDPaginaInterno) {
			contadorDePaginasSeguidas++; //si consigo paginas seguidas sumo una
		} else {
			contadorDePaginasSeguidas = 1; //sino vuelvo a 0
			miPaginaLibre = nodoActual->IDPaginaInterno; //reasigno la página para que sea la correcta
		}
	}
	return -1;
}

void eliminarProceso(int pid) {
	espacioAsignado*nodoAReventar;
	espacioLibre nodoAAgregar;
	espacioLibre*nodoLibre;
	int enDondeAgregarEspacio = 0;
	int nodoActualAReventar = 0;
	nodoAReventar = list_get(listaEspacioAsignado, nodoActualAReventar);
	while ((nodoAReventar->pid) != pid) {
		nodoActualAReventar++;
		nodoAReventar = list_get(listaEspacioAsignado, nodoActualAReventar);
	}
	nodoAReventar = list_remove(listaEspacioAsignado, nodoActualAReventar);
	nodoLibre = list_get(listaEspacioLibre, enDondeAgregarEspacio);
	while (nodoLibre->IDPaginaInterno < nodoAReventar->IDPaginaInterno) {
		enDondeAgregarEspacio++;
		nodoLibre = list_get(listaEspacioLibre, enDondeAgregarEspacio);
	}
	while ((nodoAReventar->pid) == pid) {
		nodoAAgregar.IDPaginaInterno = nodoAReventar->IDPaginaInterno;
		nodoAAgregar.inicio = nodoAReventar->posicionDePag;
		nodoAAgregar.tamanio = nodoAReventar->tamanio;
		list_add_in_index(listaEspacioLibre, enDondeAgregarEspacio,
				(&nodoAAgregar));
		enDondeAgregarEspacio++;
		nodoAReventar = list_remove(listaEspacioAsignado, nodoActualAReventar);
	}
}

void reservarPaginas(int paginaDeComienzo, int pid, int cantidadDePaginas) {
	int paginaActual = paginaDeComienzo; // en donde empieza todo.
	int lugarEnDondeDeboColocarMiNodo = 0; // aca se en donde tengo que meter esto
	int nodosQueDeboReventar = 0; // los nodos que quiero fusilar en donde empiezan
	int contadorDePaginas = 0; //cuento para el while
	espacioAsignado* paginasAReservar = malloc(sizeof(espacioAsignado)); //nodo para agarrar cosas
	espacioLibre* paginaAMatar;
	if (list_size(listaEspacioAsignado) != 0) {
		paginasAReservar = list_get(listaEspacioAsignado,
				lugarEnDondeDeboColocarMiNodo);
		while ((paginasAReservar->IDPaginaInterno) < paginaDeComienzo) {
			lugarEnDondeDeboColocarMiNodo++;
			paginasAReservar = list_get(listaEspacioAsignado,
					lugarEnDondeDeboColocarMiNodo);
		}
		paginaAMatar = list_get(listaEspacioLibre, nodosQueDeboReventar);
		while ((paginaAMatar->IDPaginaInterno) != paginaDeComienzo) {

			nodosQueDeboReventar++;
			paginaAMatar = list_get(listaEspacioLibre, nodosQueDeboReventar);

		}
	}
	(paginasAReservar->pid) = pid;
	(paginasAReservar->bitMap) = 0;
	(paginasAReservar->tamanio) = atoi(tamPagina);

	while (contadorDePaginas < cantidadDePaginas) {

		(paginasAReservar->IDPaginaInterno) = paginaActual;
		(paginasAReservar->numDePag) = contadorDePaginas;
		(paginasAReservar->posicionDePag) = paginaActual * atoi(tamPagina);
		list_add_in_index(listaEspacioAsignado, lugarEnDondeDeboColocarMiNodo,
				paginasAReservar);
		free(list_remove(listaEspacioLibre, nodosQueDeboReventar));
		nodosQueDeboReventar++;
		contadorDePaginas++;
		paginaActual++;
		lugarEnDondeDeboColocarMiNodo++;
	}

}

//Recibe un nuevo programa. 1 si lo puede recibir, 0 si no
bool recibirNuevoPrograma(int pid, int cantidadDePaginasAGuardar) {
	if (verificarSiHayEspacio(cantidadDePaginasAGuardar)) {
		if (paginasContiguasDeSwap(cantidadDePaginasAGuardar) == -1) {
			compactarSwap(); //Tengo que seguir desde acá DR Mengueche
			reservarPaginas(paginasContiguasDeSwap(cantidadDePaginasAGuardar),
					pid, cantidadDePaginasAGuardar);
			return TRUE;
		} else {
			reservarPaginas(paginasContiguasDeSwap(cantidadDePaginasAGuardar),
					pid, cantidadDePaginasAGuardar);
			return TRUE;
		}
	} else
		return FALSE;
}

//Creo listas de espacio libre y espacio asignado

void crearListas() {
	listaEspacioLibre = list_create();
	listaEspacioAsignado = list_create();
}

void compactarSwap() {

	int paginasContiguas = 0;
	espacioAsignado* nodoActual;
	int contadorParaCadenaActual;
	int contadorParaCadenaVieja;
	espacioLibre unNodoPiola;
	espacioLibre* nodoLibre = (&unNodoPiola);
	nodoActual = list_get(listaEspacioAsignado, paginasContiguas);
	do {
		if ((nodoActual->IDPaginaInterno) != paginasContiguas) {
			contadorParaCadenaActual = paginasContiguas * atoi(tamPagina);
			contadorParaCadenaVieja = (nodoActual->posicionDePag);
			while (contadorParaCadenaActual
					< (paginasContiguas + 1) * (atoi(tamPagina))) {
				archivoMappeado[contadorParaCadenaActual] =
						archivoMappeado[contadorParaCadenaVieja];
				archivoMappeado[contadorParaCadenaVieja] = '\0';
				contadorParaCadenaActual++;
				contadorParaCadenaVieja++;
			}
			(nodoActual->IDPaginaInterno) = paginasContiguas;
			(nodoActual->posicionDePag) = paginasContiguas * atoi(tamPagina);
		}
		paginasContiguas++;
		nodoActual = list_get(listaEspacioAsignado, paginasContiguas);

	} while (paginasContiguas <= list_size(listaEspacioAsignado));
	list_clean(listaEspacioLibre);
	paginasContiguas--;
	nodoActual = list_get(listaEspacioAsignado, paginasContiguas);
	(nodoLibre->inicio) = (nodoActual->posicionDePag) + atoi(tamPagina);
	(nodoLibre->tamanio) = atoi(tamPagina);
	(nodoLibre->IDPaginaInterno) = calcularIDPagina(nodoLibre->inicio);
	list_add(listaEspacioLibre, nodoLibre);
	int iDActual = ((nodoLibre->IDPaginaInterno) + 1);
	while ((nodoLibre->IDPaginaInterno) != atoi(paginas)) {

		nodoLibre++;
		(nodoLibre->IDPaginaInterno) = iDActual;
		(nodoLibre->inicio) = atoi(tamPagina) * (nodoLibre->IDPaginaInterno);
		list_add(listaEspacioLibre, nodoLibre);
		iDActual++;

	}
	usleep(1000 * atoi(retCompactacion));
}
