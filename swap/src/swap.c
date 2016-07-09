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
		printf("error 3"); //acá van errores todo

	if (fstat(fd, &sb) == -1) /* To obtain file size */
		printf("error 3"); //acá van errores todo

	length = sb.st_size;
	addr = mmap(NULL, length, PROT_READ | PROT_WRITE,
	MAP_SHARED | MAP_NORESERVE, fd, 0);
	if (addr == MAP_FAILED)
		printf("error 3"); //acá van errores todo
	return addr;

}

void setearValores(t_config * archivoConfig) {
	swapPort = config_get_int_value(archivoConfig, "PUERTO_ESCUCHA");
	nombreSwap = config_get_string_value(archivoConfig, "NOMBRE_SWAP");
	paginas = config_get_string_value(archivoConfig, "CANTIDAD_PAGINAS");
	tamPagina = config_get_string_value(archivoConfig, "TAMANIO_PAGINA");
	retCompactacion = config_get_string_value(archivoConfig,
			"RETARDO_COMPACTACION");
	tamArchivo = config_get_string_value(archivoConfig, "TAMANIO_ARCHIVO");
}

bool escribirPagina(int pid, int numeroDePagina, char*pagina) {
	paginaAsignada* nodoALeer;
	int posicionActualDeNodo = 0;
	nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	while (((nodoALeer->pid) != pid) && (nodoALeer->bitLectura != 1)) {
		posicionActualDeNodo++;
		nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	}
	nodoALeer->numDePag = numeroDePagina;
	nodoALeer->bitLectura = 0;
	list_replace(listaEspacioAsignado, posicionActualDeNodo, nodoALeer);
	int dondeEscribo = (nodoALeer->IDPaginaInterno) * atoi(tamPagina);
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
	paginaAsignada* nodoALeer;
	int posicionActualDeNodo = 0;
	nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	while (((nodoALeer->pid) != pid) && (nodoALeer->numDePag != numeroDePagina)) {
		posicionActualDeNodo++;
		nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	}
	if ((nodoALeer->bitLectura) == 1) {
		return (punteroAPaginaLeida);
	} else {
		int lugarDeLaCadena = 0;
		nodoALeer->bitLectura = 1;
		char*punteroADevolver = malloc(sizeof(char)*atoi(tamPagina));
		int posicionDeChar = (nodoALeer->IDPaginaInterno) * atoi(tamPagina);
		while (posicionDeChar
				< ((nodoALeer->IDPaginaInterno) * atoi(tamPagina)
						+ atoi(tamPagina))) {
			punteroADevolver[lugarDeLaCadena] = archivoMappeado[posicionDeChar];
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
	bitMap = malloc(sizeof(bool) * atoi(paginas));
	crearListas();
	while (counter != atoi(paginas)) {
		bitMap[counter] = 0;
		counter++;
	}

}

//Creo espacioAsignado

bool verificarSiHayEspacio(int cantidadDePaginas) {

	int contador = 0;
	int cantidad = 0;
	while (contador < atoi(paginas)) {
		if (bitMap[contador] == 0) {
			cantidad++;
		}
		contador++;
	}
	if (cantidad >= cantidadDePaginas)
		return TRUE;
	else
		return FALSE;
}

//me dice si tengo que compactar. -1 hay que compactar, siino devuelve a partir de que pagina puedo usar
int paginasContiguasDeSwap(int cantidadDePaginas) {
	int paginaADevolver = 0;
	int contador = 0;
	int paginasSeguidasLibres = 0;
	while ((contador < atoi(paginas)) && (paginasSeguidasLibres != 1)) {
		if (bitMap[contador] == 0) {
			paginasSeguidasLibres = 1;
			paginaADevolver = contador;
		}
		contador++;
	}
	if (cantidadDePaginas == paginasSeguidasLibres)
		return paginaADevolver;
	while (contador < atoi(paginas)) {
		if (bitMap[contador] == 0) {
			if (paginasSeguidasLibres == 0) {
				paginasSeguidasLibres = 1;
				paginaADevolver = contador;
			} else {
				paginasSeguidasLibres++;
			}
		} else {
			paginasSeguidasLibres = 0;
		}
		if (cantidadDePaginas == paginasSeguidasLibres) {
			return paginaADevolver;
		}
		contador++;
	}
	return -1;
}

void eliminarProceso(int pid) {
	paginaAsignada *nodoAReventar;
	int enDondeAgregarEspacio = 0;
	int nodoActualAReventar = 0;
	nodoAReventar = list_get(listaEspacioAsignado, nodoActualAReventar);
	while ((nodoAReventar->pid) != pid) {
		nodoActualAReventar++;
		nodoAReventar = list_get(listaEspacioAsignado, nodoActualAReventar);
	}
	int contador;
	int posicion;
	while ((nodoAReventar->pid) == pid) {
		contador = 0;
		posicion = nodoAReventar->pid * atoi(tamPagina);
		while (contador < atoi(tamPagina)) {
			archivoMappeado[posicion] = '\0';
			posicion++;
			contador++;
		}

		bitMap[nodoAReventar->IDPaginaInterno] = 0;
		nodoAReventar = list_remove(listaEspacioAsignado, nodoActualAReventar);
		free(nodoAReventar);
		nodoAReventar = list_get(listaEspacioAsignado, nodoActualAReventar);
	}
}

void reservarPaginas(int paginaDeComienzo, int pid, int cantidadDePaginas,
		int numeroPrimerPagina) {
	int numInternoDePagina = numeroPrimerPagina;
	int paginaActual = paginaDeComienzo; // donde empieza toodo.
	int lugarEnDondeDeboColocarMiNodo = 0; // aca se en donde tengo que meter esto
	int nodosQueDeboReventar = 0; // los nodos que quiero fusilar en donde empiezan
	int contadorDePaginas = 0; //cuento para el while
	paginaAsignada * paginaAReservar; //nodo para agarrar cosas
	if (list_size(listaEspacioAsignado) != 0) {
		paginaAReservar = list_get(listaEspacioAsignado,
				lugarEnDondeDeboColocarMiNodo);
		while (((paginaAReservar->IDPaginaInterno) < paginaDeComienzo)) {
			lugarEnDondeDeboColocarMiNodo++;
			paginaAReservar = list_get(listaEspacioAsignado,
					lugarEnDondeDeboColocarMiNodo);
			if ((lugarEnDondeDeboColocarMiNodo
					== (listaEspacioAsignado->elements_count))) {
				break;
			}
		}
	}

	while (contadorDePaginas < cantidadDePaginas) {

		paginaAReservar = malloc(sizeof(paginaAsignada));
		(paginaAReservar->pid) = pid;
		(paginaAReservar->IDPaginaInterno) = paginaActual;
		(paginaAReservar->numDePag) = numInternoDePagina;
		(paginaAReservar->bitLectura) = 0;
		if (lugarEnDondeDeboColocarMiNodo < list_size(listaEspacioAsignado))
			list_add_in_index(listaEspacioAsignado,
					lugarEnDondeDeboColocarMiNodo, paginaAReservar);
		else
			list_add(listaEspacioAsignado, paginaAReservar);
		bitMap[paginaActual] = 1;
		numInternoDePagina++;
		contadorDePaginas++;
		paginaActual++;
		lugarEnDondeDeboColocarMiNodo++;
	}

}

//Recibe un nuevo programa. 1 si lo puede recibir, 0 si no
bool recibirNuevoPrograma(int pid, int cantidadDePaginasAGuardar,
		int pgDeComienzo) {
	if (verificarSiHayEspacio(cantidadDePaginasAGuardar)) {
		if (paginasContiguasDeSwap(cantidadDePaginasAGuardar) == -1) {
			compactarSwap(); //Tengo que seguir desde acá DR Mengueche
			reservarPaginas(paginasContiguasDeSwap(cantidadDePaginasAGuardar),
					pid, cantidadDePaginasAGuardar, pgDeComienzo);
			return TRUE;
		} else {
			reservarPaginas(paginasContiguasDeSwap(cantidadDePaginasAGuardar),
					pid, cantidadDePaginasAGuardar, pgDeComienzo);
			return TRUE;
		}
	} else
		return FALSE;
}

//Creo listas de espacio libre y espacio asignado

void crearListas() {
	listaEspacioAsignado = list_create();
}

void compactarSwap() {

	int paginasContiguas = 0;
	paginaAsignada* nodoActual = malloc(sizeof(paginaAsignada));
	int contadorParaCadenaActual;
	int contadorParaCadenaVieja;
	nodoActual = list_get(listaEspacioAsignado, paginasContiguas);
	while (paginasContiguas < list_size(listaEspacioAsignado)) {
		if ((nodoActual->IDPaginaInterno) != paginasContiguas) {
			contadorParaCadenaActual = paginasContiguas * atoi(tamPagina);
			contadorParaCadenaVieja = (nodoActual->IDPaginaInterno)
					* atoi(tamPagina);
			while (contadorParaCadenaActual
					< (paginasContiguas + 1) * (atoi(tamPagina))) {
				archivoMappeado[contadorParaCadenaActual] =
						archivoMappeado[contadorParaCadenaVieja];
				archivoMappeado[contadorParaCadenaVieja] = '\0';
				contadorParaCadenaActual++;
				contadorParaCadenaVieja++;
			}
			bitMap[nodoActual->IDPaginaInterno] = 0;
			(nodoActual->IDPaginaInterno) = paginasContiguas;
			bitMap[paginasContiguas] = 1;

		}
		paginasContiguas++;
		nodoActual = list_get(listaEspacioAsignado, paginasContiguas);

	}
	while (paginasContiguas != atoi(paginas)) {

		bitMap[paginasContiguas] = 0;
		paginasContiguas++;

	}
	//usleep(1000 * atoi(retCompactacion)); todo
}
int tamanioCod(char*codigo){
	int i=0;
	while(codigo[i]!='\0'){
		i++;
	}
	return i;
}

void manejoDeConexiones() {
	int tamanioCodigo=0;
	serverSwap = socketCreateServer(swapPort);
	char* paginaLoca = malloc((sizeof(char)*atoi(tamPagina)));

	if (serverSwap == NULL) {
		puts("Error no se pudo crear el server");
	}

	if (!socketListen(serverSwap)) {
		puts("No me pude poner a escuchar");
	} else
		puts("Server creado y escuchando correctamente");

	umcClient = socketAcceptClient(serverSwap);

	while (1) {
		paginaAsignada paginaAMandar;
		buffer = socketReceive(umcClient);
		if (buffer == NULL)
			puts("Error al recibir del cliente");

		streamUmcSwap = unserializeUmcSwa(buffer);

		bool programaRecibido;
		bool escribirPagina2;
		int contadorPaginasRecibidas = 0;
		int ubicacionActual = 0;
		char*guardarPagina = malloc(sizeof(char) * atoi(tamPagina));
		int contador = 0;
		switch (streamUmcSwap->action) {
		case RECIBIR_NUEVO_PROGRAMA:

			programaRecibido = recibirNuevoPrograma(streamUmcSwap->pid,
					streamUmcSwap->cantPage,
					streamUmcSwap->pageComienzo.numDePag);
			if (programaRecibido == 0) {
//(Char id, Char action, paginaAsignada pageComienzo, Int32U cantPage, Byte* data, Int32U dataLen, Int32U pid)
				streamSwapUmc = newStrSwaUmc(SWAP_ID, PROGRAMA_NO_RECIBIDO,
						paginaAMandar, 0, NULL, 0, streamUmcSwap->pid);
				buffer = serializeSwaUmc(streamSwapUmc);
				if (!socketSend(umcClient, buffer))
					puts("Error al enviar el paquete");
			} else {
				contadorPaginasRecibidas = 0;
				while (contadorPaginasRecibidas != streamUmcSwap->cantPage) {
					ubicacionActual = contadorPaginasRecibidas
							* atoi(tamPagina);
					 tamanioCodigo=streamUmcSwap->dataLen;
					while (contador < atoi(tamPagina) && contador < tamanioCodigo) {
						guardarPagina[contador] =
								(streamUmcSwap->data)[ubicacionActual];
						contador++;
						ubicacionActual++;
					}
					 if(contador!=tamanioCodigo){
						 while(contador<atoi(tamPagina)){
							 guardarPagina[contador] = '\0';
									 contador++;
						 }
					 }
					escribirPagina(streamUmcSwap->pid, contadorPaginasRecibidas,
							guardarPagina);
					contadorPaginasRecibidas++;
				}
				streamSwapUmc = newStrSwaUmc(SWAP_ID, 26/*PROGRAMA_RECIBIDO*/ ,
						paginaAMandar, 0, NULL, 0, streamUmcSwap->pid);
				buffer = serializeSwaUmc(streamSwapUmc);
				if (!socketSend(umcClient, buffer))
					puts("Error al enviar el paquete");
			}

			break;

		case LEER_UNA_PAGINA:

			paginaLoca = leerUnaPagina(streamUmcSwap->pid,
					streamUmcSwap->pageComienzo.numDePag);
			//paginaLoca[atoi(tamPagina)]='\0';//todo aca me falta asignarlo a algo para los if que siguen
			paginaAMandar.IDPaginaInterno =
					streamUmcSwap->pageComienzo.IDPaginaInterno;
			paginaAMandar.numDePag = streamUmcSwap->pageComienzo.numDePag;
			paginaAMandar.pid = streamUmcSwap->pid;
			paginaAMandar.bitLectura = 1;
			//int tamanioPaginaLoca = atoi(tamPagina);
			//(Char id, Char action, paginaAsignada pageComienzo, Int32U cantPage, Byte* data, Int32U dataLen, Int32U pid)
			streamSwapUmc = newStrSwaUmc(SWAP_ID, LEER_UNA_PAGINA,
					paginaAMandar, 1, paginaLoca,tamPaginaLoca, streamUmcSwap->pid);
			buffer = serializeSwaUmc(streamSwapUmc);
			if (!socketSend(umcClient, buffer))
				puts("Error al enviar el paquete");

			break;

		case ESCRIBIR_UNA_PAGINA:

			escribirPagina2 = escribirPagina(streamUmcSwap->pid,
					streamUmcSwap->pageComienzo.numDePag, streamUmcSwap->data);
			if (escribirPagina2 == 0) {
				paginaAMandar.IDPaginaInterno =
						streamUmcSwap->pageComienzo.IDPaginaInterno;
				paginaAMandar.numDePag = streamUmcSwap->pageComienzo.numDePag;
				paginaAMandar.pid = streamUmcSwap->pageComienzo.pid;
				paginaAMandar.bitLectura = 1;
				streamSwapUmc = newStrSwaUmc(SWAP_ID, PAGINA_NO_ESCRITA,
						paginaAMandar, 0, NULL, 0, streamUmcSwap->pid);
				buffer = serializeSwaUmc(streamSwapUmc);
				if (!socketSend(umcClient, buffer))
					puts("Error al enviar");
			}

			break;

		case ELIMINAR_PROCESO:

			eliminarProceso(streamUmcSwap->pid);
			puts("Proceso eliminado");

			break;

		default:
			puts("HARRY TIENE UN PROBLEMA");
			break;
		}
	}
}
