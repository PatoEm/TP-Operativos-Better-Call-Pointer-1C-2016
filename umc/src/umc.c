#include "umc.h"
//#define manejarError(msg) {perror(msg); abort();} Quien garcha hizo esto?

//Ideas locas para crear las estructuras para manejar la memoria DR Mengueche

int calcularIDPagina(int inicio) {

	if (inicio == 0)
		return 0;
	else {
		return (inicio / marcos);
	}

}

void iniciarEstructurasUMC() {
	int counter = 0;
	bitMap = malloc(sizeof(bool) * marcos);

	// todo ver porqué rompe pthread_mutex_init(mutexPedidos,NULL);

	crearListas();
	inicioTLB();
	listaPaginasPorPrograma = list_create();
	while (counter != marcos) {
		bitMap[counter] = 0;
		counter++;
	}

}

void crearListas() {
	listaEspacioAsignado = list_create();
}

int tamanioCodigo(char*codigo) {
	int contador = 0;
	while (codigo[contador] != '\0') {
		contador++;
	}
	return contador;
}

bool inicializarPrograma(int pid, int paginas, char*codigo) {
	SocketBuffer*buffer;
	StrUmcSwa*streamUmcSwap;
	espacioAsignado pagina;
	StrSwaUmc * streamSwapUmc;
	pagina.numDePag = 0;
	//StrUmcSwa* newStrUmcSwa(Char id, Char action, espacioAsignado pageComienzo, Int32U cantPage, Byte* data, Int32U dataLen, Int32U pid)
	streamUmcSwap = newStrUmcSwa(UMC_ID, RECIBIR_NUEVO_PROGRAMA, pagina,
			paginas, codigo, tamanioCodigo(codigo), pid); // SI HAY PROBLEMAS MIRAR TAMANIO CODIGO TODO
	buffer = serializeUmcSwa(streamUmcSwap);
	socketSend(socketSwap->ptrSocket, buffer);

	buffer = socketReceive(socketSwap->ptrSocket);
	streamSwapUmc = unserializeSwaUmc(buffer->data);

	if (streamSwapUmc->action == PROGRAMA_RECIBIDO) {
		int i = 0;
		paginasPorPrograma*pagina = malloc(sizeof(paginasPorPrograma));
		pagina->pid = pid;
		pagina->cantPaginasEnMemoria = 0;
		list_add(listaPaginasPorPrograma, pagina);
		while (i < paginas) {
			espacioAsignado*nuevoMarco = malloc(sizeof(espacioAsignado));
			nuevoMarco->bitDePresencia = 0;
			nuevoMarco->bitModificado = 0;
			nuevoMarco->bitUso = 0;
			nuevoMarco->numDePag = i;
			nuevoMarco->pid = pid;
			if (i == 0)
				nuevoMarco->punteroAPagina = 1;
			else
				nuevoMarco->punteroAPagina = 0;
			list_add(listaEspacioAsignado, nuevoMarco);
			i++;
		}
		return TRUE;
	} else
		return FALSE;

}

bool verificarSiHayEspacio(int cantidadDePaginas) {

	int contador = 0;
	int cantidad = 0;
	while (contador < marcos) {
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

int paginasContiguasDeUMC(int cantidadDePaginas) {
	int paginaADevolver = 0;
	int contador = 0;
	int paginasSeguidasLibres = 0;
	while ((contador < marcos) && (paginasSeguidasLibres != 1)) {
		if (bitMap[contador] == 0) {
			paginasSeguidasLibres = 1;
			paginaADevolver = contador;
		}
		contador++;
	}
	if (cantidadDePaginas == paginasSeguidasLibres)
		return paginaADevolver;
	while (contador < marcos) {
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

espacioAsignado*buscarBitDeUsoEn0(int pid) {
	int inicio = lugarAsignadoInicial(pid);
	int fin = lugarAsignadoFinal(pid);
	int contador = encontrarPuntero(pid);
	espacioAsignado*nodoActual = list_get(listaEspacioAsignado, contador);
	while ((nodoActual->bitUso) == 1) {
		nodoActual->bitUso = 0;
		contador++;
		if (contador == fin)
			contador = inicio;
		nodoActual = list_get(listaEspacioAsignado, contador);
	}
	return nodoActual;
}

//devuelve el nodo del espacio en memoria
int reemplazarPaginaClock(int pid, int pagina) {

	//int inicio = lugarAsignadoInicial(pid);
	//int fin = lugarAsignadoFinal(pid);
	int posicionDePaginaLibre;
	StrUmcSwa*streamUmcSwap;
	espacioAsignado*nodoActual;
	nodoActual = buscarBitDeUsoEn0(pid);
	while (nodoActual->bitDePresencia == 0) {
		nodoActual = buscarBitDeUsoEn0(pid);
	}
	nodoActual->bitDePresencia = 0;
	posicionDePaginaLibre = nodoActual->IDPaginaInterno;
	char* paginaAEnviar = malloc(sizeof(char) * marco_Size);
	int inicioLectura = posicionDePaginaLibre * marco_Size;
	int counter = 0;
	while (counter < marco_Size) {
		paginaAEnviar[counter] = memoriaReal[inicioLectura];
		inicioLectura++;
		counter++;
	}
	int i = 0;
	espacioAsignado*nodoBuscado = list_get(listaEspacioAsignado, i);
	while (!(nodoBuscado->pid == pid && nodoBuscado->numDePag == pagina)) {
		i++;
		nodoBuscado = list_get(listaEspacioAsignado, i);
	}
	nodoBuscado->bitUso = 1;
	nodoBuscado->bitDePresencia = 1;
	nodoBuscado->IDPaginaInterno = nodoActual->IDPaginaInterno;
	espacioAsignado pageToSend;
	pageToSend.IDPaginaInterno = nodoActual->IDPaginaInterno;
	pageToSend.numDePag = nodoActual->numDePag;
	pageToSend.pid = nodoActual->pid;
	streamUmcSwap = newStrUmcSwa(UMC_ID, ESCRIBIR_UNA_PAGINA, pageToSend, 1,
			paginaAEnviar, marco_Size, nodoActual->pid);
	SocketBuffer*buffer = serializeUmcSwa(streamUmcSwap);
	if (!socketSend(socketSwap->ptrSocket, buffer))
		puts("error al enviar al swap");
	limpiarPagina(nodoActual->IDPaginaInterno * marco_Size);
	pageToSend.numDePag = pagina;
	streamUmcSwap = newStrUmcSwa(UMC_ID, LEER_UNA_PAGINA, pageToSend, 1, NULL,
			0, nodoActual->pid);
	buffer = serializeUmcSwa(streamUmcSwap);
	if (!socketSend(socketSwap->ptrSocket, buffer))
		puts("error al enviar al swap");
	buffer = socketReceive(socketSwap->ptrSocket);
	StrSwaUmc* streamSwapUmc = unserializeSwaUmc(buffer);
	inicioLectura = posicionDePaginaLibre * marco_Size;
	counter = 0;
	while (counter < marco_Size) {
		memoriaReal[inicioLectura] = streamSwapUmc->data[counter];
		counter++;
		inicioLectura++;
	}
	return posicionDePaginaLibre;
}

int reemplazarPagina(int pid, int pagina, bool lectoEscritura) {
	if (!strcmp(algoritmoDeReemplazo, "CLOCK"))
		return reemplazarPaginaClock(pid, pagina);
	else
		return reemplazarPaginaClockModificado(pid, pagina, lectoEscritura);
}

void actualizarPuntero(espacioAsignado*nodoActual, int contador, int inicio,
		int pid) {
	nodoActual->punteroAPagina = 0;
	espacioAsignado*nodoSiguiente;
	if (contador + 1 == listaEspacioAsignado->elements_count)
		nodoSiguiente = (list_get(listaEspacioAsignado, inicio));
	else
		nodoSiguiente = (list_get(listaEspacioAsignado, (contador + 1)));
	if (nodoSiguiente->pid != pid)
		nodoSiguiente = (list_get(listaEspacioAsignado, inicio));
	nodoSiguiente->punteroAPagina = 1;

}

espacioAsignado*buscarPaginaClockModificado(int pid, int pagina) {
	int inicio = lugarAsignadoInicial(pid);
	int fin = lugarAsignadoFinal(pid);
	int comienzoDelPuntero = encontrarPuntero(pid);
	int contador = comienzoDelPuntero;
	int posicionDePaginaLibre;
	espacioAsignado*nodoActual = list_get(listaEspacioAsignado, contador);
	while (!(((nodoActual->bitUso) == 0) && ((nodoActual->bitModificado) == 0))) {
		contador++;
		if (contador == fin)
			contador = inicio;
		if (contador == comienzoDelPuntero)
			break;
		nodoActual = list_get(listaEspacioAsignado, contador);
	}
	if (nodoActual->bitDePresencia == 1
			&& (((nodoActual->bitUso) == 0)
					&& ((nodoActual->bitModificado) == 0))) {
		actualizarPuntero(nodoActual, contador, inicio, pid);
		return nodoActual;
	}

	while (!(((nodoActual->bitUso) == 1) && ((nodoActual->bitModificado) == 0))) {
		nodoActual->bitUso = 0;
		nodoActual = list_get(listaEspacioAsignado, contador);
		contador++;
		if (contador == fin)
			contador = inicio;
		if (contador == comienzoDelPuntero)
			break;
	}
	if (nodoActual->bitDePresencia == 1
			&& (((nodoActual->bitUso) == 0)
					&& ((nodoActual->bitModificado) == 0))) {
		actualizarPuntero(nodoActual, contador, inicio, pid);
		return nodoActual;

	}
	while (!(((nodoActual->bitUso) == 0) && ((nodoActual->bitModificado) == 0))) {

		nodoActual = list_get(listaEspacioAsignado, contador);
		contador++;
		if (contador == fin)
			contador = inicio;
		if (contador == comienzoDelPuntero)
			break;
	}
	if (nodoActual->bitDePresencia == 1
			&& (((nodoActual->bitUso) == 0)
					&& ((nodoActual->bitModificado) == 0))) {
		actualizarPuntero(nodoActual, contador, inicio, pid);
		return nodoActual;
	}

	while (!(((nodoActual->bitUso) == 0) && ((nodoActual->bitModificado) == 1))) {

		nodoActual = list_get(listaEspacioAsignado, contador);
		contador++;
		if (contador == fin)
			contador = inicio;
		if (contador == comienzoDelPuntero)
			break;
	}
	actualizarPuntero(nodoActual, contador, inicio, pid);
	return nodoActual;
}

//devuelve el nodo del espacio en memoria. 0 lectura 1 escritura
int reemplazarPaginaClockModificado(int pid, int pagina,
bool lectoEscritura) {

	int inicio = lugarAsignadoInicial(pid);
	int fin = lugarAsignadoFinal(pid);
	int comienzoDelPuntero = encontrarPuntero(pid);
	int contador = comienzoDelPuntero;
	int posicionDePaginaLibre;
	espacioAsignado*nodoActual;
	nodoActual = buscarPaginaClockModificado(pid, pagina);
	while (nodoActual->bitDePresencia == 0) {
		nodoActual = buscarPaginaClockModificado(pid, pagina);
	}
	posicionDePaginaLibre = nodoActual->IDPaginaInterno;
	nodoActual->bitDePresencia = 0;
	int i = 0;
	espacioAsignado*nodoBuscado = list_get(listaEspacioAsignado, i);
	while (!(nodoBuscado->pid == pid && nodoBuscado->numDePag == pagina)) {
		i++;
		nodoBuscado = list_get(listaEspacioAsignado, i);
	}
	nodoBuscado->bitUso = 1;
	nodoBuscado->bitDePresencia = 1;
	nodoBuscado->IDPaginaInterno = nodoActual->IDPaginaInterno;

	char* paginaAEnviar = malloc(sizeof(char) * marco_Size);
	int inicioLectura = posicionDePaginaLibre * marco_Size;
	int counter = 0;
	while (counter < marco_Size) {
		paginaAEnviar[counter] = memoriaReal[inicioLectura];
		inicioLectura++;
		counter++;
	}
	espacioAsignado pageToSend;
	pageToSend.IDPaginaInterno = nodoActual->IDPaginaInterno;
	pageToSend.numDePag = nodoActual->numDePag;
	pageToSend.pid = nodoActual->pid;
	StrUmcSwa*streamUmcSwap = newStrUmcSwa(UMC_ID, ESCRIBIR_UNA_PAGINA,
			pageToSend, 1, paginaAEnviar, marco_Size, nodoActual->pid);
	SocketBuffer*buffer = serializeUmcSwa(streamUmcSwap);
	if (!socketSend(socketSwap->ptrSocket, buffer))
		puts("error al enviar al swap");

	limpiarPagina(nodoActual->IDPaginaInterno * marco_Size);
	pageToSend.numDePag = pagina;
	streamUmcSwap = newStrUmcSwa(UMC_ID, LEER_UNA_PAGINA, pageToSend, 1,
	NULL, 0, nodoActual->pid);
	buffer = serializeUmcSwa(streamUmcSwap);
	if (!socketSend(socketSwap->ptrSocket, buffer))
		puts("error al enviar al swap");
	buffer = socketReceive(socketSwap->ptrSocket);
	StrSwaUmc* streamSwapUmc = unserializeSwaUmc(buffer);
	inicioLectura = posicionDePaginaLibre * marco_Size;
	counter = 0;
	while (counter < marco_Size) {
		memoriaReal[inicioLectura] = streamSwapUmc->data[counter];
		counter++;
		inicioLectura++;
	}
	nodoBuscado->bitUso = 1;
	nodoBuscado->bitModificado = lectoEscritura;
	return posicionDePaginaLibre;
}

void limpiarPagina(int comienzoDePagina) {
	int contador = comienzoDePagina;
	int finDePagina = comienzoDePagina + marco_Size;
	while (contador != finDePagina) {
		memoriaReal[contador] = '\0';
		contador++;
	}

}

int encontrarPuntero(int pid) {
	int inicio = lugarAsignadoInicial(pid);
	int fin = lugarAsignadoFinal(pid);
	int contador = inicio;
	espacioAsignado*nodoActual = list_get(listaEspacioAsignado, inicio);
	while ((nodoActual->punteroAPagina) != 1) {
		contador++;
		if (contador == fin)
			contador = inicio;
		nodoActual = list_get(listaEspacioAsignado, contador);
	}
	nodoActual->punteroAPagina = 0;
	espacioAsignado*nodoSiguiente;
	if (listaEspacioAsignado->elements_count == contador + 1) {
		nodoSiguiente = list_get(listaEspacioAsignado, inicio);
	} else
		nodoSiguiente = (list_get(listaEspacioAsignado, (contador + 1)));
	if (nodoSiguiente->pid == pid)
		nodoSiguiente->punteroAPagina = 1;
	else {
		nodoSiguiente = list_get(listaEspacioAsignado, inicio);
		nodoSiguiente->punteroAPagina = 1;
	}

	return contador;
}

int lugarAsignadoFinal(int pid) {
	int inicio = lugarAsignadoInicial(pid);
	espacioAsignado*nodoFinal;
	nodoFinal = list_get(listaEspacioAsignado, inicio);
	while (nodoFinal->pid == pid) {
		inicio++;
		nodoFinal = list_get(listaEspacioAsignado, inicio);
		if (inicio == list_size(listaEspacioAsignado))
			return inicio;
	}
	return (inicio - 1);
}

int lugarAsignadoInicial(int pid) {
	int contador = 0;
	espacioAsignado*nodoActual = list_get(listaEspacioAsignado, contador);
	while (nodoActual->pid != pid) {
		contador++;
		nodoActual = list_get(listaEspacioAsignado, contador);
	}
	return contador;
}

int paginasOcupadasPorPid(int pid) {
	int contador = 0;
	paginasPorPrograma*paginaAEncontrar = list_get(listaPaginasPorPrograma,
			contador);
	while (paginaAEncontrar->pid != pid) {
		contador++;
		paginaAEncontrar = list_get(listaPaginasPorPrograma, contador);
	}
	return paginaAEncontrar->cantPaginasEnMemoria;
}

char* solicitarBytes(int pid, int pagina, int offset, int cantidad) { //todo ver que hago si no puedo pedir
	char*paginaADevolver = malloc(sizeof(char) * cantidad);
	espacioAsignado* nodoALeer;
	int posicionActualDeNodo = 0;
	nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	while (!(((nodoALeer->pid) == pid) && (nodoALeer->numDePag == pagina))) {
		posicionActualDeNodo++;
		nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	}
	if (nodoALeer->bitDePresencia == 1) {
		(nodoALeer->bitUso) = 1;
		int lugarDeLaCadena = 0;
		int posicionDeChar = (nodoALeer->IDPaginaInterno) * marco_Size + offset;
		while (lugarDeLaCadena < cantidad) {
			paginaADevolver[lugarDeLaCadena] = memoriaReal[posicionDeChar];
			posicionDeChar++;
			lugarDeLaCadena++;
		}
		if (tlbHabilitada()) {
			llevarPaginaATLB(pid, pagina, NULL);
		}
		paginaADevolver[cantidad] = '\0';
		return (paginaADevolver);

	} else {

		if (paginasOcupadasPorPid(pid) < marco_x_proc
				&& paginasContiguasDeUMC(1) != -1) {
			int contador = 0;
			paginasPorPrograma*paginaAEncontrar = list_get(
					listaPaginasPorPrograma, contador);
			while (paginaAEncontrar->pid != pid) {
				contador++;
				paginaAEncontrar = list_get(listaPaginasPorPrograma, contador);
			}
			paginaAEncontrar->cantPaginasEnMemoria++;
			nodoALeer->IDPaginaInterno = paginasContiguasDeUMC(1);
			bitMap[nodoALeer->IDPaginaInterno] = 1;
			nodoALeer->bitUso = 1;
			nodoALeer->bitModificado = 0;
			nodoALeer->bitDePresencia = 1;
			espacioAsignado pageToSend;
			pageToSend.numDePag = pagina;
			//(Char id, Char action, espacioAsignado pageComienzo, Int32U cantPage, Byte* data, Int32U dataLen, Int32U pid)
			StrUmcSwa*streamUmcSwap = newStrUmcSwa(UMC_ID,
			LEER_UNA_PAGINA, pageToSend, 1,
			NULL, 0, nodoALeer->pid);
			SocketBuffer*buffer = serializeUmcSwa(streamUmcSwap);
			if (!socketSend(socketSwap->ptrSocket, buffer))
				puts("error al enviar al swap");
			buffer = socketReceive(socketSwap->ptrSocket);
			StrSwaUmc* streamSwapUmc = unserializeSwaUmc(buffer);
			int inicioLectura = nodoALeer->IDPaginaInterno * marco_Size;
			int counter = 0;
			while (counter < marco_Size) {
				memoriaReal[inicioLectura] = streamSwapUmc->data[counter];
				counter++;
				inicioLectura++;
			}
			int comCadena = nodoALeer->IDPaginaInterno * marco_Size + offset;
			int lugCad = 0;
			while (lugCad < cantidad) {
				paginaADevolver[lugCad] = memoriaReal[comCadena];
				comCadena++;
				lugCad++;
			}
			paginaADevolver[cantidad] = '\0';
			return (paginaADevolver);
		} else {
			int frame = reemplazarPagina(pid, pagina, 1);
			int comienzoDeCadena = frame * marco_Size + offset;
			int lugarDeLaCadena = 0;
			while (lugarDeLaCadena < cantidad) {
				paginaADevolver[lugarDeLaCadena] =
						memoriaReal[comienzoDeCadena];
				comienzoDeCadena++;
				lugarDeLaCadena++;
			}
			paginaADevolver[cantidad] = '\0';
			return (paginaADevolver);

		}
	}
}

void almacenarBytes(int pid, int pagina, int offset, int tamanio, char*buffer) {
	espacioAsignado* nodoALeer;
	int posicionActualDeNodo = 0;
	nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	while (!(((nodoALeer->pid) == pid) && (nodoALeer->numDePag) == pagina)) {
		posicionActualDeNodo++;
		nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	}
	if (nodoALeer->bitDePresencia == 1) {
		(nodoALeer->bitUso) = 1;
		nodoALeer->bitModificado = 1;
		int lugarDeLaCadena = 0;
		int posicionDeChar = (nodoALeer->IDPaginaInterno) * marco_Size + offset;
		while (lugarDeLaCadena < tamanio) {
			memoriaReal[posicionDeChar] = buffer[lugarDeLaCadena];
			posicionDeChar++;
			lugarDeLaCadena++;
		}
	} else {

		if (paginasOcupadasPorPid(pid) < marco_x_proc
				&& paginasContiguasDeUMC(1) != -1) {
			int contador = 0;
			paginasPorPrograma*paginaAEncontrar = list_get(
					listaPaginasPorPrograma, contador);
			while (paginaAEncontrar->pid != pid) {
				contador++;
				paginaAEncontrar = list_get(listaPaginasPorPrograma, contador);
			}
			paginaAEncontrar->cantPaginasEnMemoria++;
			nodoALeer->IDPaginaInterno = paginasContiguasDeUMC(1);
			bitMap[nodoALeer->IDPaginaInterno] = 1;
			nodoALeer->bitUso = 1;
			nodoALeer->bitModificado = 1;
			nodoALeer->bitDePresencia = 1;
			espacioAsignado pageToSend;
			pageToSend.numDePag = pagina;
			StrUmcSwa*streamUmcSwap = newStrUmcSwa(UMC_ID,
			LEER_UNA_PAGINA, pageToSend, 1,
			NULL, 0, nodoALeer->pid);
			SocketBuffer*buf = serializeUmcSwa(streamUmcSwap);
			if (!socketSend(socketSwap->ptrSocket, buf))
				puts("error al enviar al swap");
			buf = socketReceive(socketSwap->ptrSocket);
			StrSwaUmc* streamSwapUmc = unserializeSwaUmc(buf);
			int inicioLectura = nodoALeer->IDPaginaInterno * marco_Size;
			int counter = 0;
			while (counter < marco_Size) {
				memoriaReal[inicioLectura] = streamSwapUmc->data[counter];
				counter++;
				inicioLectura++;
			}
			int comCadena = nodoALeer->IDPaginaInterno * marco_Size + offset;
			int lugCad = 0;
			while (lugCad < tamanio) {
				memoriaReal[comCadena] = buffer[lugCad];
				comCadena++;
				lugCad++;
			}
		} else {

			int frame = reemplazarPagina(pid, pagina, 1);
			int dondeEscribo = frame * marco_Size
					+ offset;
			int enDondeEstoyDeLoQueMeMandaron = 0;
			int contador = 0;
			while (contador < tamanio) {
				memoriaReal[dondeEscribo] =
						buffer[enDondeEstoyDeLoQueMeMandaron];
				dondeEscribo++;
				enDondeEstoyDeLoQueMeMandaron++;
				contador++;
			}
		}
		if (tlbHabilitada()) {
			llevarPaginaATLB(pid, pagina, NULL);
		}

	}
}

void finalizarPrograma(int pid) {
	StrUmcSwa*streamUmcSwa;
	espacioAsignado pagina;
	streamUmcSwa = newStrUmcSwa(UMC_ID, ELIMINAR_PROCESO, pagina, NULL,
	NULL,
	NULL, pid);
	SocketBuffer*buffer = serializeUmcSwa(streamUmcSwa);
	if (!socketSend(socketSwap->ptrSocket, buffer))
		puts("error al enviar al swap");
	espacioAsignado*nodoAReventar;
	//int enDondeAgregarEspacio = 0;
	int nodoActualAReventar = 0;
	nodoAReventar = list_get(listaEspacioAsignado, nodoActualAReventar);
	while ((nodoAReventar->pid) != pid) {
		nodoActualAReventar++;
		nodoAReventar = list_get(listaEspacioAsignado, nodoActualAReventar);
	}
	int contador;
	int posicion;
	while ((nodoAReventar->pid) == pid) {
		if (nodoAReventar->bitDePresencia == 1) {
			contador = 0;
			posicion = nodoAReventar->IDPaginaInterno * marco_Size;
			while (contador < marco_Size) {
				memoriaReal[posicion] = '\0';
				posicion++;
				contador++;
			}
			bitMap[nodoAReventar->IDPaginaInterno] = 0;
		}
		nodoAReventar = list_remove(listaEspacioAsignado, nodoActualAReventar);
		free(nodoAReventar);
		if (list_size(listaEspacioAsignado) >= nodoActualAReventar)
			break;
		nodoActualAReventar++;
		nodoAReventar = list_get(listaEspacioAsignado, nodoActualAReventar);
	}
}

//Funcion básica del tp
//void cambioDeProcesoActivo(int pid, int fd) {
//	espacioAsignado*espacio;
//	int contador = 0;
//	espacio = list_get(listaEspacioLibre, contador);
//	while (pid != (espacio->pid)) {
//		contador++;
//		espacio = list_get(listaEspacioLibre, contador);
//	}
//	while (pid == (espacio->pid)) {
//		espacio->socket = fd;
//		contador++;
//		espacio = list_get(listaEspacioLibre, contador);
//	}
//}

void setearValores(t_config * archivoConfig) {
	puertoEscucha = config_get_string_value(archivoConfig, "PUERTO");
	ip_Swap = config_get_string_value(archivoConfig, "IP_SWAP");
	puerto_Swap = config_get_string_value(archivoConfig, "PUERTO_SWAP");
	marcos = config_get_int_value(archivoConfig, "MARCOS");
	marco_Size = config_get_int_value(archivoConfig, "MARCO_SIZE");
	marco_x_proc = config_get_int_value(archivoConfig, "MARCO_X_PROC");
	algoritmoDeReemplazo = config_get_string_value(archivoConfig, "ALGORITMO");
	entradas_TLB = config_get_int_value(archivoConfig, "ENTRADAS_TLB");
	espera = config_get_int_value(archivoConfig, "RETARDO");
}

char * reservarMemoria(int cantidadFrames, int capacidadFrames) {
	char * memory = calloc(1, cantidadFrames * capacidadFrames);
//todo	log_info(logger, "Memoria real reservada", NULL);
	return memory;
}

int cantidadDePaginasLibres() {
	int contadorDePaginas;
	int i = 0;
	for (i = 0; i < marcos; i++) {
		if (bitMap[i] == 0) {
			contadorDePaginas++;
		}
	}
	return contadorDePaginas;
}

void liberarMemoria(char * memoria_para_liberar) {
	free(memoria_para_liberar);
	puts("Memoria liberada");
}

void manageSocketConnections() {
	conexionSockets = list_create();
	Socket* s = socketCreateServer(puertoEscucha);
	while (TRUE) {
		pthread_t socketConnection;
		puts("Escuchando conexiones del Kernel o CPUs.");
		socketListen(s);
		Socket* socketClient;
		socketClient = socketAcceptClient(s);
		if (socketClient != NULL) {
			puts("Alguien se conecto.");
			pthread_create(&socketConnection, NULL, manageSocketConnection,
					(void*) socketClient);
			list_add(conexionSockets, &socketConnection);
		}
	}

}

void* manageSocketConnection(void* param) {
	Socket* socket = (Socket*) param;
	Boolean connected = TRUE;
	puts("Gestion de conexiones.");
	while (TRUE) {
		puts("Esperando el request.");
		SocketBuffer* sb = socketReceive(socket);
		puts("Entro el request.");
		if (sb != NULL) {
			Char id = getStreamId((Stream) sb->data);
			StrKerUmc* sku = NULL;
			StrCpuUmc* scu = NULL;
			switch (id) {
			case KERNEL_ID:
				sku = unserializeKerUmc((Stream) sb->data);
				//connected =
				manageKernelRequest(socket, sku);
				break;
			case CPU_ID:
				scu = unserializeCpuUmc((Stream) sb->data);
				connected = manageCpuRequest(socket, scu);
				break;
			default:
				connected = FALSE; //todo loggear algun error.
				break;
			}
		} else {
			puts("No pudo recibir request, desconectando al cliente.");
			connected = FALSE;
		}
	}
	return NULL;
}

void manageCpuRequest(Socket* socket, StrCpuUmc* scu) {
	int pidActivo;
	espacioAsignado unaPagina;
	SocketBuffer*buffer;
	StrCpuUmc*streamCpuUmc = scu;
	StrUmcCpu*streamUmcCpu;
	char* bytes;
	while (!24/*CIERRE_CONEXION_CPU*/) {
		switch (streamCpuUmc->action) {
		case 36 /*TAMANIO_DE_MARCOS*/:
			streamUmcCpu = newStrUmcCpu(UMC_ID, TAMANIO_DE_MARCOS, 0, 0, marco_Size, NULL, 0);
			buffer = serializeUmcCpu(streamUmcCpu);
			socketSend(socket, buffer);
			break;
		case 23/*CAMBIO_PROCESO_ACTIVO*/:
			pidActivo = streamCpuUmc->pid;
			break;
		case 25/*SOLICITAR_BYTES*/:
			pthread_mutex_lock(mutexPedidos);
			if (paginasOcupadasPorPid(pidActivo) == 0
					|| cantidadDePaginasLibres() == 0) {
				streamUmcCpu = newStrUmcCpu(UMC_ID, 35 /*ABORTAR_PROGRAMA*/,
						unaPagina, scu->offset, scu->dataLen, bytes, pidActivo);
				buffer = serializeUmcCpu(streamUmcCpu);
				socketSend(socket, buffer);
			} else {
				if (tlbHabilitada()) {
					bytes = leerEnTLB(pidActivo, scu->pageComienzo.numDePag,
							scu->offset, scu->dataLen);
				} else
					bytes = solicitarBytes(pidActivo,
							scu->pageComienzo.numDePag, scu->offset,
							scu->dataLen);
				streamUmcCpu = newStrUmcCpu(UMC_ID, 25/*SOLICITAR_BYTES*/,
						unaPagina, scu->offset, scu->dataLen, bytes, pidActivo);
				buffer = serializeUmcCpu(streamUmcCpu);
				socketSend(socket, buffer);
			}
			pthread_mutex_unlock(mutexPedidos);
			break;
		case 34 /*ALMACENAR_BYTES*/:
			pthread_mutex_lock(mutexPedidos);
			if (paginasOcupadasPorPid(pidActivo) == 0
					|| cantidadDePaginasLibres() == 0) {
				streamUmcCpu = newStrUmcCpu(UMC_ID, 35 /*ABORTAR_PROGRAMA*/,
						unaPagina, scu->offset, scu->dataLen, bytes, pidActivo);
				buffer = serializeUmcCpu(streamUmcCpu);
				socketSend(socket, buffer);
			} else {

				if (tlbHabilitada()) {
					if (!escribirEnTLB(pidActivo, scu->pageComienzo.numDePag,
							scu->offset, scu->dataLen, scu->data)) {
						almacenarBytes(pidActivo, scu->pageComienzo.numDePag,
								scu->offset, scu->dataLen, scu->data);
					}
				} else
					almacenarBytes(pidActivo, scu->pageComienzo.numDePag,
							scu->offset, scu->dataLen, scu->data);
				streamUmcCpu = newStrUmcCpu(UMC_ID, 34 /*ALMACENAR_BYTES*/,
						unaPagina, scu->offset, scu->dataLen, bytes, pidActivo);
				buffer = serializeUmcCpu(streamUmcCpu);
				socketSend(socket, buffer);
			}
			pthread_mutex_unlock(mutexPedidos);
			break;
		default:
			printf("No se pudo identificar la accion de la CPU");
			break;
		}
		buffer = socketReceive(socket);
		if (buffer == NULL) {
			puts("Problemas al recibir del cpu");
			break;
		}
		streamCpuUmc = unserializeCpuUmc(buffer);
	}
//StrUmcCpu* suc;/*= newStrUmcCpu();*/
//Boolean result = sendResponse(CPU_ID, suc, socket);

//return result;
}

void manageKernelRequest(Socket* socket, StrKerUmc* sku) {
	StrUmcKer*streamAlKerner;
	SocketBuffer*buffer;
	switch (sku->action) {
	case 36 /*TAMANIO_DE_MARCOS*/:
		//(Char id, Char action, Byte* data, Int32U size, Int32U pid, Int32U cantPage)
		streamAlKerner = newStrUmcKer(UMC_ID, 36/*TAMANIO_DE_MARCOS*/,
		NULL, marco_Size, 0, 0);
		buffer = serializeUmcKer(streamAlKerner);
		socketSend(socket, buffer);
		break;
	case 20 /*INICIALIZAR_PROGRAMA*/:
		pthread_mutex_lock(mutexPedidos);

		if (0 == inicializarPrograma(sku->pid, sku->cantPage, sku->data)) {
			streamAlKerner = newStrUmcKer(UMC_ID,
					21/*PROGRAMA_NO_INICIALIZADO*/,
					NULL, 0, 0, 0);
			buffer = serializeUmcKer(streamAlKerner);
			socketSend(socket, buffer);
		} else {
			streamAlKerner = newStrUmcKer(UMC_ID,
			PROGRAMA_RECIBIDO,
			NULL, 0, 0, 0);
			buffer = serializeUmcKer(streamAlKerner);
			socketSend(socket, buffer);
		}
		pthread_mutex_unlock(mutexPedidos);
		break;

//todo hablar con pato el almacenar y leer bytes
	case 22 /*FINALIZAR_PROGRAMA*/:
		pthread_mutex_lock(mutexPedidos);
		finalizarPrograma(sku->pid);
		pthread_mutex_unlock(mutexPedidos);
		break;
	default:
		printf("No se pudo identificar la accion del Kernel");
		break;
	}

//StrUmcKer* suk ;/*= newStrUmcKer();*/
//Boolean result = sendResponse(KERNEL_ID, suk, socket);

//return result;
}

Boolean sendResponse(Char target, void* stream, Socket* socket) {
	SocketBuffer* sb = NULL;
	puts("Serializando.");
	switch (target) {
	case CPU_ID:
		sb = serializeUmcCpu((StrUmcCpu*) stream);
		puts("Enviando respuesta a la CPU.");
		break;
	case KERNEL_ID:
		sb = serializeUmcKer((StrUmcKer*) stream);
		puts("Enviando respuesta al KERNEL.");
		break;
	default:
		return FALSE;
	}

//Envio respuesta
	if (!socketSend(socket, sb)) {
		puts("No se pudo enviar el Stream.");
		if (sb != NULL) {
			free(sb);
			sb = NULL;
		}
		return FALSE;
	}
	puts("Enviado con exito");

	return TRUE;
}

void comandosUMC() {

	int opcionPrincipal;
	int opcionmemoryoTLB;
	int retardo;
	int opcionDump;
	int procesoElegido; //Los declaro aca porque no podes declarar algo adentro de un case de un switch
	int procesoQueElijo; //Los declaro aca porque no podes declarar algo adentro de un case de un switch

	while (1) {
		puts("Opcion 1: Retardo\n");
		puts("Opcion 2: Dump\n");
		puts("Opcion 3: Flush\n");

		scanf("%d", &opcionPrincipal);

		switch (opcionPrincipal) {
		case 1:
			system("clear");
			puts("Ingresar retardo\n");
			scanf("%d", &retardo);
			retardoUMC(retardo);
			system("clear");
			break;

		case 2:
			puts("seleccione el tipo de dump:\n");
			puts(
					"opción 1 : Dump de estructuras de memoria de todos los procesos\n");
			puts(
					"opción 2 : Dump de estructuras de memoria de un proceso en particular\n");
			puts(
					"opción 3 : Dump del contenido de la memoria de todos los procesos\n");
			puts(
					"opción 4 : Dump del contenido de la memoria de un proceso en particular\n");

			scanf("%d", &opcionDump);

			switch (opcionDump) {
			case 1:
				system("clear");
				dumpEstructuraDeMemoriaTodosLosProcesos();
				system("clear");
				break;
			case 2:
				system("clear");
				puts("Coloque el número de proceso: \n");
				scanf("%d", &procesoElegido);
				system("clear");
				dumpEstructuraDeMemoriaProcesoEnParticular(procesoElegido);
				break;
			case 3:
				system("clear");
				dumpContenidoDeMemoriaTodosLosProcesos();
				system("clear");
				break;
			case 4:
				system("clear");
				puts("Ingresar process ID: \n");
				scanf("%d", &procesoQueElijo);
				system("clear");
				dumpContenidoDeMemoriaProcesoEnParticular(procesoQueElijo);
				break;
			}
			break;

		case 3:
			system("clear");
			puts("Opcion 1: TLB\n");
			puts("Opcion 2: Memory\n");
			scanf("%d", &opcionmemoryoTLB);

			switch (opcionmemoryoTLB) {
			case 1:
				system("clear");
				flushTLB();
				system("clear");
				break;

			case 2:
				system("clear");
				flushMemory();
				system("clear");
				break;

			default:

				break;
			}
			break;

		default:

			break;
		}
		system("clear");
	}
}

void retardoUMC(int retardo) {
	espera = retardo;
	log_info(logger, "El retardo ha sido modificado\n");
}

void dumpEstructuraDeMemoriaProcesoEnParticular(int pid) {
	int i = 0;
	espacioAsignado * nodoActualDeAsignados;
//IMPRIMO EN PANTALLA
	printf("Paginas Asignadas al proceso: %d\n", pid);
	while (i < list_size(listaEspacioAsignado)) {
		nodoActualDeAsignados = list_get(listaEspacioAsignado, i);
		if (nodoActualDeAsignados->pid == pid) {
			printf("ID Frame: %d\n", nodoActualDeAsignados->IDPaginaInterno);
			printf("PID: %d\n", nodoActualDeAsignados->pid);
			printf("Pagina: %d\n", nodoActualDeAsignados->numDePag);
			log_info(logger, "Frame Asignado: %d \nPID: %d\n Pagina: %d\n\n",
					nodoActualDeAsignados->IDPaginaInterno,
					nodoActualDeAsignados->pid);
		}
		i++;
	}

}

void dumpEstructuraDeMemoriaTodosLosProcesos() {
	int i = 0;
	espacioAsignado * nodoActualDeAsignados;

//IMPRIMO EN PANTALLA
	puts("Paginas Asignadas:\n");
	while (i < list_size(listaEspacioAsignado)) {
		nodoActualDeAsignados = list_get(listaEspacioAsignado, i);
		printf("ID Frame: %d\n", nodoActualDeAsignados->IDPaginaInterno);
		printf("PID: %d\n", nodoActualDeAsignados->pid);
		printf("Pagina: %d\n", nodoActualDeAsignados->numDePag);
		log_info(logger, "Frame Asignado: %d \nPID: %d\n Pagina: %d\n\n",
				nodoActualDeAsignados->IDPaginaInterno,
				nodoActualDeAsignados->pid);
		i++;
	}

	puts("Paginas Libres:\n");
	i = 0;
	while (i < marcos) {
		if (bitMap[i] == 0) {
			printf("ID Frame: %d \n", i);
			log_info(logger, "Frame libre: %d\n", i);
		}
	}

}

void dumpContenidoDeMemoriaTodosLosProcesos() {
	int i = 0;
	int direccionFisica;
	int hastaDondeLeer;
	espacioAsignado * nodoActual;

	while (i < list_size(listaEspacioAsignado)) {
		nodoActual = list_get(listaEspacioAsignado, i);
		direccionFisica = nodoActual->IDPaginaInterno * marco_Size;
		hastaDondeLeer = (nodoActual->IDPaginaInterno + 1) * marco_Size;

		printf("Frame: %d\nPID: %d\nPagina: %d\nContenido:\n",
				nodoActual->IDPaginaInterno, nodoActual->pid, nodoActual,
				nodoActual->numDePag);

		log_info(logger, "Frame: %d\nPID: %d\nPagina: %d\nContenido: \n",
				nodoActual->IDPaginaInterno, nodoActual->pid, nodoActual,
				nodoActual->numDePag);

		while (direccionFisica < hastaDondeLeer) {
			printf("%c", memoriaReal[direccionFisica]);
			log_info(logger, "%c", memoriaReal[direccionFisica]);
			direccionFisica++;
		}
		printf("\n");
		log_info(logger, "\n");

		i++;
	}
}

void dumpContenidoDeMemoriaProcesoEnParticular(int PID) {
	int i = 0;
	int direccionFisica;
	int hastaDondeLeer;
	espacioAsignado * nodoActual;

	while (i < list_size(listaEspacioAsignado)) {
		nodoActual = list_get(listaEspacioAsignado, i);
		if (nodoActual->pid == PID) {
			direccionFisica = nodoActual->IDPaginaInterno * marco_Size;
			hastaDondeLeer = (nodoActual->IDPaginaInterno + 1) * marco_Size;

			printf("Frame: %d\nPID: %d\nPagina: %d\nContenido:\n",
					nodoActual->IDPaginaInterno, nodoActual->pid,
					nodoActual->numDePag);

			log_info(logger, "Frame: %d\nPID: %d\nPagina: %d\nContenido: \n",
					nodoActual->IDPaginaInterno, nodoActual->pid,
					nodoActual->numDePag);

			while (direccionFisica < hastaDondeLeer) {
				printf("%c", memoriaReal[direccionFisica]);
				log_info(logger, "%c", memoriaReal[direccionFisica]);
				direccionFisica++;
			}
			printf("\n");
			log_info(logger, "\n");
		}
		i++;
	}
}

void flushTLB() {
	if (entradas_TLB == 0) {
//log_info(logger, "TLB Deshabilitado"); todo que loguee otro
	} else {
		int i = 0;
		while (i < TLB->elements_count) {
			elDestructorDeNodosTLB(i);
			i++;
		}
		log_info(logger, "TLB acaba de vaciarse");
	}
}

void flushMemory() {
	int i = 0;
	espacioAsignado*nodoAModificar;
	while (i < listaEspacioAsignado->elements_count) {
		nodoAModificar = list_get(listaEspacioAsignado, i);
		nodoAModificar->bitModificado = 1;
		i++;
	}
}

void menuUMC(pthread_t hiloComandos, pthread_attr_t attrhiloComandos) {

	pthread_attr_init(&attrhiloComandos);

	pthread_attr_setdetachstate(&attrhiloComandos,
	PTHREAD_CREATE_DETACHED);
	int hiloParaComandos = pthread_create(&hiloComandos, &attrhiloComandos,
			(void *) comandosUMC, NULL);

	pthread_attr_destroy(&attrhiloComandos);
}

void inicioTLB() {
	int habilitada = tlbHabilitada();
	if (habilitada == 0) {
//log_info(logger, "TLB Deshabilitada"); que loguee otro x2 todo
	} else {
		// todo log_info(logger, "TLB Habilitada con %d entradas", entradas_TLB);
		TLB = creoTLB();
//Inicializo la cantidad de aciertos y accesos
		aciertosTLB = 0;
		accesosTLB = 0;
	}
}

t_list * creoTLB() {
	t_list * TLB = list_create();
	return TLB;
}

void elDestructorDeNodosTLB(int i) {
	list_remove(TLB, i);
}

void elDestructorDeNodosMemoria(int i) {
	list_remove(listaEspacioAsignado, i);
}

bool escribirEnTLB(int pid, int pagina, int offset, int cantidad, char*buffer) {
	accesosTLB++;
	int i = 0;
	t_tlb*nodoActual = list_get(TLB, i);
	while (i < list_size(TLB)) {

		if ((nodoActual->pid) == pid && nodoActual->pagina == pagina)
			break;
		i++;
	}
	if (i >= list_size(TLB))
		return 0;
	else {
		int lugarActual = nodoActual->frameTLB * marco_Size + offset;
		for (i = 0; i < cantidad; i++) {
			memoriaReal[lugarActual] = buffer[i];
			i++;
			lugarActual++;
		}
		return 1;
	}

}

int buscarFrameEnMemoria(int PID, int pagina) {

	espacioAsignado * paginas;
	int i = 0;
	while (i < list_size(listaEspacioAsignado)) {
		paginas = list_get(listaEspacioAsignado, i);
		if (paginas->pid == PID) {
			if (paginas->numDePag == pagina) {
				return paginas->IDPaginaInterno;
			}
		}
	}

}

void llevarPaginaATLB(int PID, int pagina, char* buffer) {
	accesosTLB++;
	if (tlbHabilitada()) {
		int TLBLlena = tlbLlena();
		if (TLBLlena) {
			int paginaAEscribir = reemplazarPaginaLRU();
			t_tlb* tlb = malloc(sizeof(t_tlb));
			tlb->pid = PID;
			tlb->pagina = pagina;
			tlb->momentoEntrada = accesosTLB;
			tlb->frameTLB = buscarFrameEnMemoria(PID, pagina);
			insertarNodoOrdenadoEnTLB(tlb);

		} else {
			t_tlb* tlb = malloc(sizeof(t_tlb));
			tlb->pid = PID;
			tlb->pagina = pagina;
			tlb->momentoEntrada = accesosTLB;
			tlb->frameTLB = buscarFrameEnMemoria(PID, pagina);
			insertarNodoOrdenadoEnTLB(tlb);
		}
	}

}

//detecta Que pagina debe reemplazar y la lleva a memoria si debe
int reemplazarPaginaLRU() {
	int paginaLibre;
	int contadorPrimerMomento = 0;
	t_tlb*paginaAComparar;
	t_tlb*paginaAMatar;
	//char* buffer = malloc(sizeof(char) * marco_Size);
	paginaAMatar = list_get(TLB, contadorPrimerMomento);
	int lugarDePaginaAMatar = 0;
	for (contadorPrimerMomento = 0; contadorPrimerMomento < list_size(TLB);
			contadorPrimerMomento++) {
		paginaAComparar = list_get(TLB, contadorPrimerMomento);
		if (paginaAComparar->momentoEntrada < paginaAMatar->momentoEntrada) {
			paginaAMatar = paginaAComparar;
			lugarDePaginaAMatar = contadorPrimerMomento;
		}
	}
	paginaLibre = paginaAMatar->frameTLB;
	free(list_remove(TLB, lugarDePaginaAMatar));
	return paginaLibre;
}

//nodo ordenado por PID
void insertarNodoOrdenadoEnTLB(t_tlb*unNodo) {
	int contador = 0;
	t_tlb*nodoActual = list_get(TLB, contador);
	while (nodoActual->pid < unNodo->pid && contador < list_size(TLB)) {
		contador++;
		nodoActual = list_get(TLB, contador);
	}
	list_add_in_index(TLB, contador, unNodo);
}

int buscarPaginaVaciaEnTLB() {
	int i = 0;
	while (bitMapTLB[i] != 0) {
		i++;
	}
	return i;
}

char* leerEnTLB(int PID, int pagina, int posicion, int tamanio) {
	accesosTLB++;
	int habilitada = tlbHabilitada();
	char* buffer = malloc(sizeof(char) * tamanio);
	if (habilitada != 0) {
		t_tlb * entradaTLB = buscarEnTLB(PID, pagina);
		if (entradaTLB != NULL) {
			log_info(logger, "Acierto de TLB en el frame %d y pagina %d",
					entradaTLB->frameTLB, entradaTLB->pagina);
			int inicioLectura = entradaTLB->frameTLB * marco_Size + posicion;
			int i;
			for (i = 0; i < tamanio; i++) {
				buffer[i] = memoriaReal[inicioLectura];
				inicioLectura++;
			}
			return buffer;
		}

	}
	return solicitarBytes(PID, pagina, posicion, tamanio);
}

t_tlb * buscarEnTLB(int PID, int pagina) {
	accesosTLB++;
	int i = 0;
	int sizeTLB = list_size(TLB);
	while (i < sizeTLB) {
		t_tlb * entradaTLB = list_get(TLB, i);
		if (entradaTLB->pid == PID) {
			if (entradaTLB->pagina == pagina) {
				return entradaTLB; //La encontro
			}
		}
		i++;
	}
	return NULL;
}

//Devuelve 1 si esta llena, devuelve 0 si no esta llena
int tlbLlena() {
	if (TLB->elements_count == entradas_TLB) {
		return 1;
	}

	return 0;
}

//DEVUELVE 1 SI ESTA HABILITADA, SI NO ESTA HBILITADA DEVUELVE 0
int tlbHabilitada() {
	if (entradas_TLB == 0) {
		return 0;
	}
	return 1;
}
