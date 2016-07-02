/*
  * nucleo.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#include "nucleo.h"
//#define manejarError(msg) {perror(msg); abort();}



//testeada
void setearValores(t_config * archivoConfig) {

	pthread_mutex_lock(mutexQuantum);
	quantum = config_get_int_value(archivoConfig, "QUANTUM");
	quantumSleep = config_get_int_value(archivoConfig, "QUANTUM_SLEEP");
	pthread_mutex_unlock(mutexQuantum);

	if (primeraLectura == true) {
		puertoPropio = config_get_string_value(archivoConfig, "PUERTO_PROPIO");
		cpuPort = config_get_string_value(archivoConfig, "PUERTO_CPU");
		ipUMC = config_get_string_value(archivoConfig, "IP_UMC");
		UMCPort = config_get_string_value(archivoConfig, "PUERTO_UMC");
		idSemaforos = config_get_array_value(archivoConfig, "SEM_IDS");
		viSemaforos = config_get_array_value(archivoConfig, "SEM_INIT");
		idIO = config_get_array_value(archivoConfig, "IO_IDS");
		retardoIO = config_get_array_value(archivoConfig, "IO_SLEEP");
		idVariableCompartida = config_get_array_value(archivoConfig,
				"SHARED_VARS");
		tamanioPaginas = config_get_int_value(archivoConfig, "MARCO_SIZE");
		stackSize = config_get_int_value(archivoConfig, "STACK_SIZE");
		primeraLectura = false;
	}
}
//testeada
pcb* crearNuevoPcb(char * programaAnsisop, int tamanioArchivo) {

	t_medatada_program* metaNuevoPrograma = metadata_desde_literal(
			programaAnsisop);

	t_intructions* instrucciones = metaNuevoPrograma->instrucciones_serializado;

	t_size cantidadInstrucciones = metaNuevoPrograma->instrucciones_size;

	pcb* pcbNuevoPrograma=malloc(sizeof(pcb));
	idProgramas = idProgramas + 1;
	puts("hola");


	pcbNuevoPrograma->id = idProgramas;

	pcbNuevoPrograma->tamanioArchivoOriginal = tamanioArchivo;

	pcbNuevoPrograma->metaProgram = metaNuevoPrograma;

	pcbNuevoPrograma->programCounter = metaNuevoPrograma->instruccion_inicio;

	pcbNuevoPrograma->paginasDeCodigo = cantidadPaginas(tamanioPaginas,
			tamanioArchivo);

	//pcbNuevoPrograma.indiceDeCodigo = crearIndiceCodigo(cantidadInstrucciones, instrucciones);

	arrayBidimensional * array = (arrayBidimensional*) malloc(
			cantidadInstrucciones * sizeof(arrayBidimensional));

	int i = 0;
	for (i = 0; i < cantidadInstrucciones; i++) {
		array[i].comienzo = instrucciones[i].start;
		array[i].longitud = (instrucciones[i].offset);
	}

	pcbNuevoPrograma->indiceDeCodigo = array;

	pcbNuevoPrograma->indiceDeEtiquetas = metaNuevoPrograma->etiquetas;

	pcbNuevoPrograma->estado = 0; //NEW

	pcbNuevoPrograma->indiceDelStack = (paginaDeStack*) malloc(
			sizeof(paginaDeStack) * stackSize);

	pthread_mutex_lock(mutexColaNew);
	queue_push(colaNew,pcbNuevoPrograma);
	pthread_mutex_unlock(mutexColaNew);
	return pcbNuevoPrograma;
}
//testeadas
void moverAColaReady(pcb * programa) {

	switch (programa->estado) {
	case 0:
		pthread_mutex_lock(mutexColaNew);
		queue_pop(colaNew);
		pthread_mutex_unlock(mutexColaNew);
		break; //0 NEW
	case 1:
		break;
	case 2:
		pthread_mutex_lock(mutexListaExec);
		buscarYEliminarPCBEnLista(listaExec, programa);
		pthread_mutex_unlock(mutexListaExec);
		break; //2 EXEC
	case 3:
		pthread_mutex_lock(mutexListaBlock);
		buscarYEliminarPCBEnLista(listaBlock, programa);
		pthread_mutex_unlock(mutexListaBlock);
		break; //3 BLOCK
	case 4:
		break;
	}

	programa->estado = 1; //1 READY
	queue_push(colaReady, programa);
}
void moverAListaBlock(pcb* programa) {
	pthread_mutex_lock(mutexListaExec);
	buscarYEliminarPCBEnLista(listaExec, programa);
	pthread_mutex_unlock(mutexListaExec);

	programa->estado = 3; //3 BLOCK

	pthread_mutex_lock(mutexListaBlock);
	list_add(listaBlock, programa);
	pthread_mutex_unlock(mutexListaBlock);

}
void moverAListaExec(pcb* programa) {
	pthread_mutex_lock(mutexColaReady);
	queue_pop(colaReady);
	pthread_mutex_unlock(mutexColaReady);

	programa->estado = 2; //3 BLOCK

	pthread_mutex_lock(mutexListaExec);
	list_add(listaExec, programa);
	pthread_mutex_unlock(mutexListaExec);
}
void moverAColaExit(pcb* programa) {
	pthread_mutex_lock(mutexListaExec);
	buscarYEliminarPCBEnLista(listaExec, programa);
	pthread_mutex_unlock(mutexListaExec);

	programa->estado = 4; // 4 EXIT

	pthread_mutex_lock(mutexColaExit);
	queue_push(colaExit, programa);
	pthread_mutex_unlock(mutexColaExit);
}
void finalizarProcesosColaExit() {
	//ACA DEBO ENVIAR MENSAJE A LA CONSOLA DE QUE FINALIZARON SUS PROGRAMAS

	//COMO LO HAGO?

	//todo LUEGO BORRO ABSOLUTAMENTE Y DESTRUYO
	queue_clean_and_destroy_elements(colaExit, NULL); //ESTO NO SE SI ESTA BIEN
}

void escuchoMuchasConexiones() {

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
	myaddr.sin_port = htons(puertoPropio);

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
					if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0) {
						// error o conexión cerrada por el cliente
						if (nbytes == 0) {
							// conexión cerrada
							printf("Servidor select: socket %d cuelga\n", i);
						} else {
							perror("recv");
						}
						close(i); // chau gil
						FD_CLR(i, &master); // elimino del conjunto maestro
					} else {
						//pthread_t hilo1;
						//int a = pthread_create(&hilo1,NULL,*funcionQueHaceElHIlo, argumentos);
						// tenemos datos de algún cliente
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
			}
		}
	}
}

//Testeada
void funcionHiloQuantum() {

	for (;;) {

		verificarModificacionesArchivoConfig();
		usleep(3);
		leerArchivoDeConfiguracion("confignucleo");
		printf("Valores Seteados Nuevamente.\n");
	}

}
//Testeada
void verificarModificacionesArchivoConfig() {

	int length;
	int fd;
	int wd;
	char buffer[EVENT_BUF_LEN];

	/*creating the INOTIFY instance*/
	fd = inotify_init();

	/*checking for error*/
	if (fd < 0) {
		perror("inotify_init");
	}

	/*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
	wd = inotify_add_watch(fd, "confignucleo", IN_MODIFY);

	/*read to determine the event change happens on “/tmp” directory. Actually this read blocks until the change event occurs*/

	length = read(fd, buffer, EVENT_BUF_LEN);

	/*checking for error*/
	if (length < 0) {
		perror("read");
	} else {

		printf("El archivo de Configuracion Fue Modificado.\n");
		//leerArchivoDeConfiguracion("confignucleo");
		//printf("Valores Seteados Nuevamente.\n");
	}

	/*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/

	/*removing the “/tmp” directory from the watch list.*/
	inotify_rm_watch(fd, wd);

	/*closing the INOTIFY instance*/
	close(fd);

}
//testeada
void entrada_salida(char * identificador, int cantidad, pcb *pcbPrograma) {

	int i;
	int j;
	int abortar = 0;
	int retardoPeriferico;
	int totalRetardo;
	for (i = 0; (idIO[i] != '\0'); i++) {

		if ((strcmp(idIO[i], identificador)) == 0) {
			j = i;

			//retardoPeriferico = (int) retardoIO[i];

			retardoPeriferico = atoi( retardoIO[i]);
			abortar++;
		}

	}

	if (abortar == 0) {
		//ACA PARA MATAR todo

	}
	totalRetardo = retardoPeriferico * cantidad * 1000;
	//usleep(totalRetardo*1000);

	if (pthread_mutex_trylock(mutexIO[j]) == 0) {

		ejecutarIO(j, pcbPrograma, totalRetardo);

	} else {

		moverAListaBlock(pcbPrograma);

		pthread_mutex_lock(mutexIO[j]);
		ejecutarIO(j, pcbPrograma, totalRetardo);

	}

}
//testeada
void ejecutarIO(int posicion, pcb* pcbDelPrograma, int retardo) {

	usleep(retardo);
	pthread_mutex_unlock(mutexIO[posicion]);
	//todo Mover el programCounter.
	moverAColaReady(pcbDelPrograma);
}
//testeada
int obtener_valor(char* identificador, pcb* pcbPrograma) {

	int i;
	int abortar = 0; //SI es 0 Aborta.
	int valor;
	for (i = 0; (idVariableCompartida[i] != '\0'); i++) {

		if ((strcmp(idVariableCompartida[i], identificador)) == 0) {

			if (pthread_mutex_trylock(mutexVariables[i]) == 0) {
				moverAColaReady(pcbPrograma);
				valor = variableCompartidaValor[i];
				pthread_mutex_unlock(mutexVariables[i]);
			} else {
				moverAListaBlock(pcbPrograma);

				pthread_mutex_lock(mutexVariables[i]);
				moverAColaReady(pcbPrograma);
				valor = variableCompartidaValor[i];
				pthread_mutex_unlock(mutexVariables[i]);
			}
			abortar++;

		}

	}
	if (abortar == 0) {
		//todo MATAR
	}
	return valor;

}
//testeada
void grabar_valor(char* identificador, int valor, pcb* pcbPrograma) {

	int i;
	int abortar = 0; //SI es 0 Aborta.
	for (i = 0; (idVariableCompartida[i] != '\0'); i++) {

		if ((strcmp(idVariableCompartida[i], identificador)) == 0) {
			if (pthread_mutex_trylock(mutexVariables[i]) == 0) {
				moverAColaReady(pcbPrograma);
				variableCompartidaValor[i] = valor;
				pthread_mutex_unlock(mutexVariables[i]);
			} else {
				moverAListaBlock(pcbPrograma);

				pthread_mutex_lock(mutexVariables[i]);
				moverAColaReady(pcbPrograma);
				variableCompartidaValor[i] = valor;
				pthread_mutex_unlock(mutexVariables[i]);
			}
			abortar++;

		}

	}
	if (abortar == 0) {
		//todo MATAR
	}

}
//testeada
void waitAnsisop(char * identificador, pcb *pcbPrograma) {

	int i;
	int abortar = 0; //SI es 0 Aborta.
	for (i = 0; (idSemaforos[i] != '\0'); i++) {

		if ((strcmp(idSemaforos[i], identificador)) == 0) {

			if (sem_trywait(semaforosAnsisop[i]) == 0) {

				moverAColaReady(pcbPrograma);
			} else {

				moverAListaBlock(pcbPrograma);
				sem_wait(semaforosAnsisop[i]);
				moverAColaReady(pcbPrograma);
				//todo avisar a cpu que se bloqueo
			}

			abortar++;

		}

	}
	if (abortar == 0) {
		//todo MATAR
	}

}
//testeada
void signalAnsisop(char* identificador, pcb*pcbPrograma) {

	int i;
	int abortar = 0; //SI es 0 Aborta.
	for (i = 0; (idSemaforos[i] != '\0'); i++) {

		if ((strcmp(idSemaforos[i], identificador)) == 0) {

			sem_post(semaforosAnsisop[i]);

			moverAColaReady(pcbPrograma);

			abortar++;

		}

	}
	if (abortar == 0) {
		//todo MATAR
	}

}
//testeada
int inicializarVariables() {

	// LOG
	nucleolog = log_create("nucleo.log", "NUCLEO", 1, LOG_LEVEL_INFO);


	//Variables de lectura de archivo
	puertoPropio=(char*)malloc(sizeof(puertoPropio));
	cpuPort=(char*)malloc(sizeof(cpuPort));
	quantum=(int)malloc(sizeof(quantum));
	quantumSleep=(int)(sizeof(quantumSleep));
	idSemaforos=(char**)malloc(sizeof(idSemaforos));
	viSemaforos=(char**)malloc(sizeof(viSemaforos));
	cantSemaforos=(int)malloc(sizeof(cantSemaforos)); //No se lee por config
	idIO=(char**)malloc(sizeof(idIO));
	retardoIO=(char**)malloc(sizeof(retardoIO));
	int cantIO=(int)malloc(sizeof(cantIO));	//No se lee por config

	idVariableCompartida=(char**)malloc(sizeof(idVariableCompartida));
	cantVarCompartidas=(int)malloc(sizeof(cantVarCompartidas));
	//variableCompartidaValor=(int*)malloc(sizeof(variableCompartidaValor));
	ipUMC=(char*)malloc((sizeof(ipUMC)));
	UMCPort=(char*)malloc((sizeof(UMCPort)));
	stackSize=(int)malloc((sizeof(stackSize)));
	tamanioPaginas=(int)malloc((sizeof(tamanioPaginas)));

	//Otras Variables
	idProgramas=(int)malloc(sizeof(idProgramas)); //Contador de programa
	primeraLectura=(bool)malloc(sizeof(primeraLectura));

	//Sincronizacion
	//pthread_mutex_t** mutexIO;
	//pthread_mutex_t** mutexVariables;
	mutexQuantum=malloc(sizeof(mutexQuantum));

	mutexColaNew=(pthread_mutex_t*)malloc(sizeof(mutexColaNew));
	mutexColaReady=(pthread_mutex_t*)malloc(sizeof(mutexColaReady));
	mutexColaExit=(pthread_mutex_t*)malloc(sizeof(mutexColaExit));
	mutexListaExec=(pthread_mutex_t*)malloc(sizeof(mutexListaExec));
	mutexListaBlock=(pthread_mutex_t*)malloc(sizeof(mutexListaBlock));

	//Colas y listas

	//t_queue *colaNew;

	//t_queue *colaReady;

	//t_list *listaExec;

	//t_list *listaBlock;

	//t_queue *colaExit;


	primeraLectura = true;

	int i;

	//Leo el archivo de configuracion
	leerArchivoDeConfiguracion("confignucleo");


	//Inicio Semaforos
	cantSemaforos = cantidadPalabrasEnArrayDeStrings(idSemaforos);
	char* valorInicial;
	//char algo;
	unsigned int algo2=0;

	//sem_t semaforoPrueba;
	//sem_init(&semaforoPrueba, 0, 4);

	semaforosAnsisop=malloc(sizeof(char));



	for (i = 0; i < cantSemaforos; i++) {
		valorInicial=viSemaforos[i];
		//algo=*valorInicial;
		algo2=atoi(valorInicial);
		semaforosAnsisop[i]=malloc(sizeof(sem_t));
		if (sem_init((semaforosAnsisop[i]), 0, algo2) != 0) {
			printf("\n init semaforoAnsisop %d fallo\n", i);
			return -1;
		}
	}

	//Inicio Semaforos de Sincro

	//inicio cantIO

	cantIO = cantidadPalabrasEnArrayDeStrings(idIO);
	mutexIO=malloc(sizeof(char));

	for (i = 0; i < cantIO; i++) {

		mutexIO[i]=malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(mutexIO[i], NULL) != 0) {
			printf("\n init mutexIO %d fallo\n", i);
			return -1;
		}
	}

	//inicio cantVarsCompartidas
		cantVarCompartidas = cantidadPalabrasEnArrayDeStrings(idVariableCompartida);

		variableCompartidaValor = (int*) malloc(sizeof(int) * cantVarCompartidas);

		for (i = 0; i < cantVarCompartidas; i++) {
			variableCompartidaValor[i] = 0;
		}
		mutexVariables=malloc(sizeof(char));
	for (i = 0; i < cantVarCompartidas; i++) {
		mutexVariables[i]=malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(mutexVariables[i], NULL) != 0) {
			printf("\n init mutexVariables %d fallo\n", i);
			return -1;
		}
	}

	if (pthread_mutex_init(mutexQuantum, NULL) != 0) {
		printf("\n init mutexQuamtum fallo\n");
		return -1;
	}

	if (pthread_mutex_init(mutexColaNew, NULL) != 0) {
		printf("\n init mutexCOlaNew fallo\n");
		return -1;
	}
	if (pthread_mutex_init(mutexColaReady, NULL) != 0) {
		printf("\n init mutexColaReady fallo\n");
		return -1;
	}
	if (pthread_mutex_init(mutexColaExit, NULL) != 0) {
		printf("\n init mutexColaExit fallo\n");
		return -1;
	}
	if (pthread_mutex_init(mutexListaBlock, NULL) != 0) {
		printf("\n init mutexListaBlock fallo\n");
		return -1;
	}
	if (pthread_mutex_init(mutexListaExec, NULL) != 0) {
		printf("\n init mutexListaExec fallo\n");
		return -1;
	}

	//inicio El contador de ids
	idProgramas = 0;

	//InicioLasColas
	colaNew = queue_create();
	colaReady = queue_create();
	listaExec = list_create();
	listaBlock = list_create();
	colaExit = queue_create();

	return 0;
}

//testeada
void crearHilos(){

	 	 pthread_t hiloQuantum;
		 pthread_attr_t attrHiloQuantum;
		 pthread_attr_init(&attrHiloQuantum);
		 pthread_attr_setdetachstate(&attrHiloQuantum, PTHREAD_CREATE_DETACHED);
		 pthread_create(&hiloQuantum, &attrHiloQuantum, &funcionHiloQuantum, NULL);
		 pthread_attr_destroy(&attrHiloQuantum);

		 cpuHandlerThread();


}

void buscarYEliminarPCBEnLista(t_list * lista, pcb* pcbLoco) {

	int i;

	pcb * pcbComparar;
	for (i = 0; i < list_size(lista); i++) {

		pcbComparar = ((pcb*) list_get(lista, i));

		if ((pcbComparar->id) == (pcbLoco->id)) {

			list_remove(lista, i);

		}

	}

}


