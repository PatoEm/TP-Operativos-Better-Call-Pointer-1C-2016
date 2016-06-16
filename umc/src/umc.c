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
	crearListas();
	while (counter != marcos) {
		bitMap[counter] = 0;
		counter++;
	}

}

void crearListas() {
	listaEspacioAsignado = list_create();
}

bool inicializarPrograma(int pid, int paginas, char*codigo) { //todo falta enviar el programa a la umc
	if (verificarSiHayEspacio(paginas)) {
		if (paginasContiguasDeUMC(paginas) == -1) {
			compactarUMC(); //Tengo que seguir desde acá DR Mengueche
			reservarPaginas(paginasContiguasDeSwap(paginas), pid, paginas);
			return TRUE;
		} else {
			reservarPaginas(paginasContiguasDeSwap(paginas), pid, paginas);
			return TRUE;
		}
	} else
		return FALSE;

}

void reservarPaginas(int paginaDeComienzo, int pid, int cantidadDePaginas) {
	int numInternoDePagina = 0;
	int paginaActual = paginaDeComienzo; // donde empieza toodo.
	int lugarEnDondeDeboColocarMiNodo = 0; // aca se en donde tengo que meter esto
	int nodosQueDeboReventar = 0; // los nodos que quiero fusilar en donde empiezan
	int contadorDePaginas = 0; //cuento para el while
	espacioAsignado* paginaAReservar; //nodo para agarrar cosas
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

		paginaAReservar = malloc(sizeof(espacioAsignado));
		(paginaAReservar->pid) = pid;
		(paginaAReservar->IDPaginaInterno) = paginaActual;
		(paginaAReservar->numDePag) = numInternoDePagina;
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

void compactarUMC() {

	int paginasContiguas = 0;
	espacioAsignado* nodoActual = malloc(sizeof(espacioAsignado));
	int contadorParaCadenaActual;
	int contadorParaCadenaVieja;
	nodoActual = list_get(listaEspacioAsignado, paginasContiguas);
	while (paginasContiguas < list_size(listaEspacioAsignado)) {
		if ((nodoActual->IDPaginaInterno) != paginasContiguas) {
			contadorParaCadenaActual = paginasContiguas * marcos;
			contadorParaCadenaVieja = (nodoActual->IDPaginaInterno) * marcos;
			while (contadorParaCadenaActual < (paginasContiguas + 1) * marcos) {
				memoriaReal[contadorParaCadenaActual] =
						memoriaReal[contadorParaCadenaVieja];
				memoriaReal[contadorParaCadenaVieja] = '\0';
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
	while (paginasContiguas != marcos) {

		bitMap[paginasContiguas] = 0;
		paginasContiguas++;

	}

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

char* solicitarBytes(int pid, int pagina, int offset, int cantidad) {

	espacioAsignado* nodoALeer;
	int posicionActualDeNodo = 0;
	nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	while (((nodoALeer->pid) != pid) && (nodoALeer->numDePag != pagina)) {
		posicionActualDeNodo++;
		nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	}
	if (posicionActualDeNodo >= list_size(listaEspacioAsignado)) {
		//IR A BUSCAR AL SWAP todo
	} else {
		int lugarDeLaCadena = 0;
		char paginaADevolver[cantidad];
		char*punteroADevolver = (&paginaADevolver[0]);
		int posicionDeChar = (nodoALeer->IDPaginaInterno) * marco_Size + offset;
		while (lugarDeLaCadena < cantidad) {
			paginaADevolver[lugarDeLaCadena] = memoriaReal[posicionDeChar];
			posicionDeChar++;
			lugarDeLaCadena++;
		}
		return (punteroADevolver);
	}
}

void almacenarBytes(int pid, int pagina, int offset, int tamanio, char*buffer) {
	espacioAsignado* nodoALeer;
	int posicionActualDeNodo = 0;
	nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	while ((((nodoALeer->pid) != pid) && (nodoALeer->numDePag))
			|| (posicionActualDeNodo == list_size(listaEspacioAsignado))) {
		posicionActualDeNodo++;
		nodoALeer = list_get(listaEspacioAsignado, posicionActualDeNodo);
	}
	if (posicionActualDeNodo >= list_size(listaEspacioAsignado)) {
		//todo ir a buscar al swap la pagina
	} else {
		int dondeEscribo = (nodoALeer->IDPaginaInterno) * marco_Size + offset;
		int enDondeEstoyDeLoQueMeMandaron = 0;
		int contador = 0;
		while (contador < tamanio) {
			memoriaReal[dondeEscribo] = buffer[enDondeEstoyDeLoQueMeMandaron];
			dondeEscribo++;
			enDondeEstoyDeLoQueMeMandaron++;
			contador++;
		}
	}
}

void finalizarPrograma(int pid) {//todo avisar al swap que tiene que reventar pid

	espacioAsignado*nodoAReventar;
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
		posicion = nodoAReventar->pid * marco_Size;
		while (contador < marco_Size) {
			memoriaReal[posicion] = '\0';
			posicion++;
			contador++;
		}

		bitMap[nodoAReventar->IDPaginaInterno] = 0;
		nodoAReventar = list_remove(listaEspacioAsignado, nodoActualAReventar);
		free(nodoAReventar);
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
	char * memory = calloc(cantidadFrames, capacidadFrames);
	log_info(logger, "Memoria real reservada", NULL);
	return memory;
}

void liberarMemoria(char * memoria_para_liberar) {
	free(memoria_para_liberar);
	puts("Memoria liberada");
}

void escuchoMuchasConexiones(void) {

//ACA ME TENGO QUE HACER CLIENTE DEL SWAP
	fd_set master; // maestro es el conjunto de file descriptors que están actualmente conectados
	fd_set read_fds; // conjunto temporal de descriptores de fichero para select()

	struct sockaddr_in myaddr;       // dirección del servidor
	struct sockaddr_in remoteaddr;     // dirección del cliente

	struct timeval tv;    //esta es la estructura que regula el tiempo
	tv.tv_sec = 2;    //estos son los segundos que tarda
	tv.tv_usec = 500000;  //estos son los microsegundos que tarda

	int fdmax;        // número máximo de file descriptors
	int listener;     // descriptor de socket a la escucha
	int newfd;        // descriptor de socket de nueva conexión aceptada
	char buf[256]; // buffer para datos del cliente ESTO VA A TENER EL TAMAÑO DE LO QUE MANDEMOS
	int nbytes;
	int yes = 1;        // para setsockopt() SO_REUSEADDR, más abajo
	int addrlen;
	int i, j;

	FD_ZERO(&master);    // borra los conjuntos maestro y temporal
	FD_ZERO(&read_fds);

	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port = htons(atoi(puertoEscucha));
	memset(&(myaddr.sin_zero), '\0', 8);

// obtener socket a la escucha
	if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Error en el socket");
		exit(1);
	}
// obviar el mensaje "address already in use" (la dirección ya se está usando)
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
			== -1) {
		perror("setsockopt");
		exit(1);
	}

// enlazar
	if (bind(listener, (struct sockaddr *) &myaddr, sizeof(myaddr)) == -1) {
		perror("bind");
		exit(1);
	}

// escuchar
	if (listen(listener, 10) == -1) {
		perror("Error en el listen");
		exit(1);
	}

// añadir listener al conjunto maestro
	FD_SET(listener, &master);
// seguir la pista del descriptor de fichero mayor
	fdmax = listener; // por ahora es éste
	printf("Empieza el select\n");
// bucle principal
	for (;;) {
		read_fds = master; // cópialo
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("Error en el select");
			exit(1);
		}
		// explorar conexiones existentes en busca de datos que leer
		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) { // TENEMOS DATOS PAPA
				if (i == listener) {
					// gestionar nuevas conexiones
					addrlen = sizeof(remoteaddr);
					if ((newfd = accept(listener,
							(struct sockaddr *) &remoteaddr, &addrlen)) == -1) {
						perror("Error en el accept");
					} else {
						FD_SET(newfd, &master); // añadir al conjunto maestro
						if (newfd > fdmax) {    // actualizar el máximo
							fdmax = newfd;
						}
						printf("Nueva conexion de %s en " "socket %d\n",
								inet_ntoa(remoteaddr.sin_addr), newfd);
					}
				} else {
					// gestionar datos de un cliente
					printf("Hago el recv\n");
					if ((nbytes = recv(i, (void*) buf, sizeof(buf), 0)) <= 0) { //LOS DATOS LOS RECIBO ACA
						// error o conexión cerrada por el cliente
						if (nbytes == 0) {
							// conexión cerrada
							printf(
									"Servidor select: socket %d acaba de colgar\n",
									i);
						} else {
							perror("recv");
						}
						close(i); // chau gil
						FD_CLR(i, &master); // elimino del conjunto maestro
					} else {

						/*tenemos datos de algún cliente
						 *
						 * ACA HAGO TODAS LAS COSAS OK? SI OK
						 * ACA TENGO QUE MANDAR LOS HILOS DE GESTION DE LOS N CPUS Y DEL NUCLEO
						 *
						 * la variable i es el socket que me manda datos en ese momento
						 * la variable buf es lo que yo recibo de datos
						 * la variable nbytes son los bytes que recibo
						 *
						 * HAGO TOODO ESTO Y SE LO MANDO AL SWAP
						 */

						pthread_t hiloNucleo, hiloCPU;
						int hilo1, hilo2;
						char* argumentoNucleo = "devuelvo hilo nucleo";
						char* argumentoCPU = "devuelvo hilo CPU";

						printf("Me habla el Socket %d: ", i);
						//if (nbytes != 0) printf("%s", buf); //ESTA LINEA

						if (buf[0] == '0') { //Aca vendria el protocolo con el nucleo en la condicion del if uso esto para probar

							hilo1 = pthread_create(&hiloNucleo, NULL,
									(void *) meHablaKernelPrueba,
									(void *) argumentoNucleo);

							hilo1 = pthread_join(hiloNucleo, NULL);
						} else if (buf[0] == '1') { //Aca vendria el protocolo con el cpu en la condicion del if uso esto para probar

							hilo2 = pthread_create(&hiloCPU, NULL,
									(void *) meHablaCPUPrueba,
									(void *) argumentoCPU);

							hilo2 = pthread_join(hiloCPU, NULL);
						} else {

							printf("No me habló ni CPU ni Nucleo\n");
						}

						for (j = 0; j <= fdmax; j++) {
							// le envio a tooodo el mundo
							if (FD_ISSET(j, &master)) {
								// excepto al listener y a nosotros mismos
								if (j != listener && j != i) {
									if (send(j, buf, nbytes, 0) == -1) {
										perror("send");
									}
								}
							}
						}
					}
				}
			}  //La asquerosidad de esto llega a niveles inhumanos
		}
	}
}

void meHablaCPUPrueba() {

	printf("Hola soy el CPU\n");

}

void meHablaKernelPrueba() {

	printf("Hola soy el Nucleo\n");
}

void comandosUMC() {

	int opcionPrincipal;
	int opcionmemoryoTLB;
	int retardo;

	while (1) {
		puts("Opcion 1: Retardo\n");
		puts("Opcion 2: Dump\n");
		puts("Opcion 3: Flush\n");

		scanf("%d", &opcionPrincipal);

		switch (opcionPrincipal) {
		case 1:
			puts("Ingresar retardo\n");
			scanf("%d", &retardo);
			retardoUMC(retardo);
			break;

		case 2:
			dump();
			break;

		case 3:
			puts("Opcion 1: TLB\n");
			puts("Opcion 2: Memory\n");
			scanf("%d", &opcionmemoryoTLB);

			switch (opcionmemoryoTLB) {
			case 1:
				flushTLB();
				break;

			case 2:
				flushMemory();
				break;

			default:

				break;
			}
			break;

		default:

			break;
		}
	}
}

void retardoUMC(int retardo) {
	espera = retardo;
}

void dump() {
	int i = 0;
	espacioAsignado * nodoActualDeAsignados;

//IMPRIMO EN PANTALLA
	puts("Paginas Asignadas:\n");
	while (nodoActualDeAsignados->IDPaginaInterno < marcos) {
		nodoActualDeAsignados = (espacioAsignado*) list_get(listaEspacioAsignado, i);
		printf("ID Frame: %d\n", nodoActualDeAsignados->IDPaginaInterno);
		printf("PID: %d\n", nodoActualDeAsignados->pid);
		i++;
	}

	puts("Paginas Libres:\n");
	i = 0;

//IMPRIMO EN EL LOG
	i = 0;

	while (nodoActualDeAsignados->IDPaginaInterno < marcos) {
		nodoActualDeAsignados = (espacioAsignado*) list_get(listaEspacioAsignado, i);
		log_info(logger, "Frame: %d \nPID: %d\n\n",
				nodoActualDeAsignados->IDPaginaInterno, nodoActualDeAsignados->pid);
	}

//IMPRIMO EN EL ARCHIVO
	/*
	 archivo = fopen("Dump","rw+");
	 while(nodoActualDeAsignados->IDFrame < marcos){

	 }

	 while(nodoActualDeLibres->IDFrame < marcos){

	 }
	 fclose(archivo);
*/
}

void flushTLB() {
	if (entradas_TLB == 0) {
		log_info(logger, "TLB Deshabilitado");
	} else {
		int i = 0;
		while (i < TLB->elements_count) {
			elDestructorDeNodosTLB(i);
			bitMapTLB[i]=false; //Vacio todo el bitMap de la tlb
			i++;
		}
		log_info(logger, "TLB acaba de vaciarse");
	}
}

void flushMemory() {
	int i = 0;
	while (i < listaEspacioAsignado->elements_count){
		elDestructorDeNodosMemoria(i);
		bitMap[i] = false; //vacio todo el bitMap
	}
}

void menuUMC(pthread_t hiloComandos, pthread_attr_t attrhiloComandos) {

	pthread_attr_init(&attrhiloComandos);

	pthread_attr_setdetachstate(&attrhiloComandos, PTHREAD_CREATE_DETACHED);
	int hiloParaComandos = pthread_create(&hiloComandos, &attrhiloComandos,
			(void *) comandosUMC, NULL);

	pthread_attr_destroy(&attrhiloComandos);
}

void inicioTLB() {
	int habilitada = tlbHabilitada();
	if (habilitada == 0) {
		log_info(logger, "TLB Deshabilitada");
	} else {
		log_info(logger, "TLB Habilitada con %d entradas", entradas_TLB);

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

void leerEnTLB(int PID, int pagina,int * posicion){

	t_tlb * entradaTLB = buscarEnTLB(PID, pagina, posicion);
	int habilitada=tlbHabilitada();

	if(habilitada != 0){
		if(entradaTLB != NULL){
			log_info("Acierto de TLB en el frame %d y pagina %d",entradaTLB->frame,entradaTLB->pagina);
			//Creo que no hay ningun retardo pero si lo hay lo pongo en esta linea
			//Aca le tengo que mandar al CPU o al kernel que el pedido de lectura esta hecho
		}
		//Si no lo encontró leo en memoria real
	}
}

t_tlb * buscarEnTLB(int PID, int pagina, int* posicion){
	int i = 0;
	int sizeTLB = list_size(TLB);
	while ( i < sizeTLB){
		t_tlb * entradaTLB = list_get(TLB, i);
		if (entradaTLB->pid == PID){
			if (entradaTLB->pagina == pagina){
				*posicion = i;
				return entradaTLB; //La encontro
			}
		}
		i++;
	}
	*posicion = -1;
	return NULL;
}

//Devuelve 1 si esta llena, devuelve 0 si no esta llena
int tlbLlena(){
	if(TLB->elements_count == entradas_TLB){
	return 1;
	}

	return 0;
}

//DEVUeLVE 1 SI ESTA HABILITADA, SI NO ESTA HBILITADA DEVUELVE 0
int tlbHabilitada(){
	if(entradas_TLB == 0){
		return 0;
	}
	return 1;
}

void leerEnMemoriaReal(int PID, int pagina){

	espacioAsignado * paginaActual = buscarEnMemoriaReal(PID, pagina);
	int habilitada=tlbHabilitada();
	//Tengo que seguir esto

}

espacioAsignado * buscarEnMemoriaReal(int PID, int pagina){
	int i=0;
	int sizeMemory = list_size(listaEspacioAsignado);
	while(i<sizeMemory){
		espacioAsignado * paginaActual = list_get(listaEspacioAsignado, i);
		if(paginaActual->pid == PID){
			return paginaActual; //devuelvo la pagina que encontre
		}
		i++;
	}
	return NULL; //No la encontro en memoria real
}
