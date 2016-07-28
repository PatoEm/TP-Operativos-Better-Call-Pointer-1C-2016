/*
 * nucleo.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#include "nucleo.h"

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
		stackSize = config_get_int_value(archivoConfig, "STACK_SIZE");
		primeraLectura = false;
	}
}
//testeada
pcb* crearNuevoPcb(Socket* consola, char * programaAnsisop, int tamanioArchivo) {

	t_medatada_program* metaNuevoPrograma = metadata_desde_literal(
			programaAnsisop);

	t_intructions* instrucciones = metaNuevoPrograma->instrucciones_serializado;

	t_size cantidadInstrucciones = metaNuevoPrograma->instrucciones_size;

	pcb* pcbNuevoPrograma = malloc(sizeof(pcb));
	idProgramas = idProgramas + 1;
	puts("hola");

	pcbNuevoPrograma->consola = consola;

	pcbNuevoPrograma->cpu=consola;

	pcbNuevoPrograma->id = idProgramas;

	pcbNuevoPrograma->tamanioArchivoOriginal = tamanioArchivo;

	pcbNuevoPrograma->cantPagCod=calcularCantPagCod(tamanioArchivo);

	pcbNuevoPrograma->programCounter = metaNuevoPrograma->instruccion_inicio;

	pcbNuevoPrograma->cantElementsStack = 0;

	//todo fijarse que tengo que pedirle a la memoria
	//pcbNuevoPrograma->paginasDeCodigo = cantidadPaginas(tamanioPaginas,tamanioArchivo);
	pcbNuevoPrograma->paginasDeCodigo = cantidadPaginasArchivo(tamanioArchivo);
	//pcbNuevoPrograma.indiceDeCodigo = crearIndiceCodigo(cantidadInstrucciones, instrucciones);

	arrayBidimensional * array = (arrayBidimensional*) malloc(
			cantidadInstrucciones * sizeof(arrayBidimensional));

	int i = 0;
	for (i = 0; i < cantidadInstrucciones; i++) {
		array[i].comienzo = instrucciones[i].start;
		array[i].longitud = (instrucciones[i].offset);
	}

	pcbNuevoPrograma->indiceDeCodigo = (arrayBidimensional*) malloc(
			cantidadInstrucciones * sizeof(arrayBidimensional));

	for (i = 0; i < cantidadInstrucciones; i++) {
		memcpy(&(pcbNuevoPrograma->indiceDeCodigo[i]), &array[i],
				sizeof(arrayBidimensional));
	}
//free(array);

	pcbNuevoPrograma->indiceDeCodigoSize = cantidadInstrucciones
			* sizeof(arrayBidimensional);

	if (metaNuevoPrograma->etiquetas != NULL) {
		pcbNuevoPrograma->indiceDeEtiquetas = malloc(
				sizeof(char) * (metaNuevoPrograma->etiquetas_size));

		memcpy((pcbNuevoPrograma->indiceDeEtiquetas), metaNuevoPrograma->etiquetas, (metaNuevoPrograma->etiquetas_size));
		pcbNuevoPrograma->indiceDeEtiquetasSize = strlen(
				metaNuevoPrograma->etiquetas) + 1;
		pcbNuevoPrograma->etiquetaSize = metaNuevoPrograma->etiquetas_size;
	} else {
		pcbNuevoPrograma->indiceDeEtiquetas = "MUERTE A WINDOWS";
		pcbNuevoPrograma->indiceDeEtiquetasSize = 17;
		pcbNuevoPrograma->etiquetaSize=17;
	}



	pcbNuevoPrograma->instruccionesTotales =
			metaNuevoPrograma->instrucciones_size;

	pcbNuevoPrograma->instruccionesRestantes =
			metaNuevoPrograma->instrucciones_size;

	pcbNuevoPrograma->estado = 0; //NEW

	pcbNuevoPrograma->indiceDelStack = list_create();

	pthread_mutex_lock(mutexColaNew);
	list_add(listaNew, pcbNuevoPrograma);
	pthread_mutex_unlock(mutexColaNew);
	return pcbNuevoPrograma;
}
//testeadas

void moverAColaReady(pcb * programa) {

	switch (programa->estado) {
	case NEW:
		pthread_mutex_lock(mutexColaNew);
		buscarYEliminarPCBEnLista(listaNew, programa);
		pthread_mutex_unlock(mutexColaNew);
		break; //0 NEW
	case READY:
		break;
	case EXEC:
		pthread_mutex_lock(mutexListaExec);
		buscarYEliminarPCBEnLista(listaExec, programa);
		pthread_mutex_unlock(mutexListaExec);
		break; //2 EXEC
	case BLOCK:
		pthread_mutex_lock(mutexListaBlock);
		buscarYEliminarPCBEnLista(listaBlock, programa);
		pthread_mutex_unlock(mutexListaBlock);
		break; //3 BLOCK
	case EXIT:
		break;
	}

	programa->estado = 1; //1 READY
	pthread_mutex_lock(mutexColaReady);
	list_add(listaReady, programa);
	pthread_mutex_unlock(mutexColaReady);

	satisfacerCpuAlPedo();

//	if(listaCpu->elements_count != 0){
//		puts("PCB READY, HAY UNA CPU AL PEDO");
//		Socket* cpuLoca = list_remove(listaCpu, 0);
//		printf("CPU al pedo removida, LISTACPU = %d\n", listaCpu->elements_count);
//		if(!enviarPcbACpu(cpuLoca)){
//			puts("No se pudo enviar pcb");
//		} else {
//			puts("PCB enviado");
//		}
//	}
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
	buscarYEliminarPCBEnLista(listaReady, programa);
	pthread_mutex_unlock(mutexColaReady);

	programa->estado = EXEC; //2 EXEC

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
	list_add(listaExit, programa);
	pthread_mutex_unlock(mutexColaExit);
}
void finalizarProcesosColaExit() {
	//ACA DEBO ENVIAR MENSAJE A LA CONSOLA DE QUE FINALIZARON SUS PROGRAMAS

	//COMO LO HAGO?

	//todo LUEGO BORRO ABSOLUTAMENTE Y DESTRUYO

	list_clean(listaExit);

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
	for (i = 0; i< (*cantidadDispositivosIO); i++) {

		if ((strcmp(idIO[i], identificador)) == 0) {
			j = i;

			//retardoPeriferico = (int) retardoIO[i];

			retardoPeriferico = atoi(retardoIO[i]);
			abortar++;
		}

	}

	if (abortar == 0) {
		//ACA PARA MATAR todo

	}
	totalRetardo = retardoPeriferico * cantidad * 1000;
	//usleep(totalRetardo*1000);

	moverAListaBlock(pcbPrograma);
	if (pthread_mutex_lock(mutexIO[j]) == 0) {
		ejecutarIO(j, pcbPrograma, totalRetardo);

	}

}
//testeada
void ejecutarIO(int posicion, pcb* pcbDelPrograma, int retardo) {

	usleep(retardo);
	pthread_mutex_unlock(mutexIO[posicion]);
	moverAColaReady(pcbDelPrograma);
}
//testeada
int obtener_valor(char* identificador) {

	int i;
	int abortar = 0; //SI es 0 Aborta.
	int valor;


	for (i = 0; i < cantVarCompartidas; i++) {

		if ((strcmp(idVariableCompartida[i], identificador)) == 0) {

			if (pthread_mutex_trylock(mutexVariables[i]) == 0) {
				;
				valor = variableCompartidaValor[i];
				pthread_mutex_unlock(mutexVariables[i]);
			} else {
				pthread_mutex_lock(mutexVariables[i]);
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
void grabar_valor(char* identificador, int valor) {

	int i;
	int abortar = 0; //SI es 0 Aborta.
	for (i = 0; i<cantVarCompartidas; i++) {

		if ((strcmp(idVariableCompartida[i], identificador)) == 0) {
			if (pthread_mutex_trylock(mutexVariables[i]) == 0) {
				variableCompartidaValor[i] = valor;
				pthread_mutex_unlock(mutexVariables[i]);
			} else {
				pthread_mutex_lock(mutexVariables[i]);
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
void waitAnsisop(char * identificador, pcb* pcbPrograma, Socket* cpuSocket) {

	int i;
	int abortar = 0; //SI es 0 Aborta.

	StrKerCpu* StrKernelCpu;
	SocketBuffer* buffer;
	// (Char id, Char action, pcb pcb, Int8U quantum, Byte* data, Int32U dataLen, Byte* nombreDispositivo, Int32U lenNomDispositivo)
	//buffer = serializeKerCpu(StrKernelCpu);

	//char* aux;

	for (i = 0; (i < cantSemaforos); i++) {


		if ((strcmp(idSemaforos[i], identificador)) == 0) {

			if (sem_trywait(semaforosAnsisop[i]) == 0) {
				StrKernelCpu = newStrKerCpu(1/*KERNEL_ID*/, WAIT_REALIZADO/*WAIT_REALIZADO*/,
							*pcbPrograma, 0, 0, NULL, 0, NULL, 0);
				buffer = serializeKerCpu(StrKernelCpu);
				socketSend(cpuSocket, buffer);

				//moverAColaReady(pcbPrograma);

			} else {

				StrKernelCpu = newStrKerCpu(1/*KERNEL_ID*/, PROGRAMA_BLOQUEADO,
							*pcbPrograma, 0, 0, NULL, 0, NULL, 0);
				buffer = serializeKerCpu(StrKernelCpu);
				socketSend(cpuSocket, buffer);

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
void signalAnsisop(char* identificador) {

	int i;
	int abortar = 0; //SI es 0 Aborta.
	for (i = 0; i < cantSemaforos; i++) {
		if ((strcmp(idSemaforos[i], identificador)) == 0) {

			sem_post(semaforosAnsisop[i]);

			abortar++;

		}

	}
	if (abortar == 0) {
		//todo MATAR
	}

}
//testeada
int inicializarVariables(char* ruta) {

	// LOG
	nucleolog = malloc(sizeof(t_log));
	//nucleolog = log_create("nucleo.log", "NUCLEO", 1, LOG_LEVEL_INFO);

	memcpy(nucleolog, log_create("nucleo.log", "NUCLEO", 1, LOG_LEVEL_INFO),
			sizeof(t_log));

	//tamanioPaginas=pedirTamanioDePagina();

	//Variables de lectura de archivo
	puertoPropio = (char*) malloc(sizeof(puertoPropio));
	cpuPort = (char*) malloc(sizeof(cpuPort));
	quantum = (int) malloc(sizeof(quantum));
	quantumSleep = (int) (sizeof(quantumSleep));
	idSemaforos = (char**) malloc(sizeof(idSemaforos));
	viSemaforos = (char**) malloc(sizeof(viSemaforos));
	cantSemaforos = (int) malloc(sizeof(cantSemaforos)); //No se lee por config
	idIO = (char**) malloc(sizeof(idIO));
	retardoIO = (char**) malloc(sizeof(retardoIO));
	int cantIO = (int) malloc(sizeof(cantIO));	//No se lee por config

	idVariableCompartida = (char**) malloc(sizeof(idVariableCompartida));
	cantVarCompartidas = (int) malloc(sizeof(cantVarCompartidas));
	//variableCompartidaValor=(int*)malloc(sizeof(variableCompartidaValor));
	ipUMC = (char*) malloc((sizeof(ipUMC)));
	UMCPort = (char*) malloc((sizeof(UMCPort)));
	stackSize = (int) malloc((sizeof(stackSize)));
	tamanioPaginas = (int) malloc((sizeof(tamanioPaginas)));

	//Otras Variables
	idProgramas = (int) malloc(sizeof(idProgramas)); //Contador de programa
	primeraLectura = (bool) malloc(sizeof(primeraLectura));

	//Sincronizacion
	//pthread_mutex_t** mutexIO;
	//pthread_mutex_t** mutexVariables;
	mutexQuantum = malloc(sizeof(pthread_mutex_t));

	mutexColaNew = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	mutexColaReady = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	mutexColaExit = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	mutexListaExec = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	mutexListaBlock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	mutexListaCpu = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));

	primeraLectura = true;

	int i;

	//Leo el archivo de configuracion
	leerArchivoDeConfiguracion(ruta);

	//Inicio Semaforos
	cantSemaforos = cantidadPalabrasEnArrayDeStrings(idSemaforos);
	char* valorInicial;
	//char algo;
	unsigned int algo2 = 0;

	//sem_t semaforoPrueba;
	//sem_init(&semaforoPrueba, 0, 4);

	//semaforosAnsisop=malloc(sizeof(pthread_mutex_t)*cantSemaforos);

	for (i = 0; i < cantSemaforos; i++) {
		valorInicial = viSemaforos[i];
		//algo=*valorInicial;
		algo2 = atoi(valorInicial);
		semaforosAnsisop[i] = malloc(sizeof(sem_t));
		if (sem_init((semaforosAnsisop[i]), 0, algo2) != 0) {
			printf("\n init semaforoAnsisop %d fallo\n", i);
			return -1;
		}
	}

	//Inicio Semaforos de Sincro

	//inicio cantIO
	cantidadDispositivosIO = malloc(sizeof(int));
	cantIO = cantidadPalabrasEnArrayDeStrings(idIO);
	memcpy(cantidadDispositivosIO,&cantIO,sizeof(int));
	//mutexIO=malloc(sizeof(pthread_mutex_t)*cantIO);

	for (i = 0; i < cantIO; i++) {

		mutexIO[i] = malloc(sizeof(pthread_mutex_t));
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
	//mutexVariables=malloc(sizeof(pthread_mutex_t)*cantVarCompartidas);
	for (i = 0; i < cantVarCompartidas; i++) {
		mutexVariables[i] = malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(mutexVariables[i], NULL) != 0) {
			printf("\n init mutexVariables %d fallo\n", i);
			return -1;
		}
	}

	if (pthread_mutex_init(mutexListaCpu, NULL) != 0) {
		printf("\n init mutexListaCpu fallo\n");
		return -1;
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
	listaNew = list_create();
	//colaNew = queue_create();
	listaReady = list_create();
	//colaReady = queue_create();
	listaExec = list_create();
	listaBlock = list_create();
	listaExit = list_create();
	//colaExit = queue_create();
	listaCpu = list_create();

	umcServer = socketCreateClient();
	do {
		puts("**********************************");
		puts("Intentando conectar con la UMC ppal.");
		printf("IP: %s, PUERTO: %d\n", ipUMC, atoi(UMCPort));
		sleep(3);
	} while (!socketConnect(umcServer, ipUMC, atoi(UMCPort)));
	StrKerUmc* out_umc_msg = newStrKerUmc(KERNEL_ID, HANDSHAKE, NULL, 0, 0, 0,
			0, 0, 0);
	SocketBuffer* sb = serializeKerUmc(out_umc_msg);
	socketSend(umcServer->ptrSocket, sb);
	puts("Mensaje enviado a la UMC ppal.");

	sb = socketReceive(umcServer->ptrSocket);
	StrUmcKer* in_umc_msg = unserializeUmcKer(sb);

	printf("Nuevo UMC es %d.\n", in_umc_msg->size);

	int nuevoPuertoUmc = in_umc_msg->size;
	tamanioPaginas = pedirTamanioDePagina(nuevoPuertoUmc);

	return 0;
}

//testeada
void crearHilos() {

	pthread_t hiloQuantum;
	pthread_attr_t attrHiloQuantum;
	pthread_attr_init(&attrHiloQuantum);
	pthread_attr_setdetachstate(&attrHiloQuantum, PTHREAD_CREATE_DETACHED);
	pthread_create(&hiloQuantum, &attrHiloQuantum, &funcionHiloQuantum, NULL);
	pthread_attr_destroy(&attrHiloQuantum);

	cpuHandlerThread();

}

void* buscarYEliminarPCBEnLista(t_list * lista, pcb* pcbLoco) {

	int i;

	pcb * pcbComparar;
	for (i = 0; i < list_size(lista); i++) {

		pcbComparar = ((pcb*) list_get(lista, i));

		if ((pcbComparar->id) == (pcbLoco->id)) {

			return list_remove(lista, i);

		}

	}

	return NULL;

}

bool buscarPCB(t_list * lista, pcb* pcbLoco) {

	int i;

	pcb * pcbComparar;
	for (i = 0; i < list_size(lista); i++) {

		pcbComparar = ((pcb*) list_get(lista, i));

		if ((pcbComparar->id) == (pcbLoco->id)) {

			return TRUE;

		}

	}

	return FALSE;

}

pcb* buscarPCBPorConsola(t_list * lista, Socket* consola) {

	int i;

	pcb * pcbComparar;
	for (i = 0; i < list_size(lista); i++) {

		pcbComparar = ((pcb*) list_get(lista, i));

		if ((int)(pcbComparar->consola->descriptor) == (int)(consola->descriptor)) {

			return list_get(lista, i);

		}

	}

	return NULL;

}

void funcionHiloIO(atributosIO *atributos) {

	entrada_salida(atributos->identificador, atributos->cantidad,
			atributos->pcbLoca);

}

void funcionHiloWait(atributosWait* atributos) {

	waitAnsisop(atributos->identificador, &atributos->pcbLoca,
			atributos->cpuSocket);

}

bool satisfacerCpuAlPedo(){

	pthread_mutex_lock(mutexListaCpu);
	puts("Hay CPUs al pedo?");
	if(listaCpu->elements_count != 0){
		puts("CPU al pedo encontrada. Intentando enviar PCB.");
		Socket* cpuLoca = list_remove(listaCpu, 0);
		printf("CPU al pedo removida, LISTACPU = %d\n", listaCpu->elements_count);
		if(!enviarPcbACpu(cpuLoca)){
			puts("No se pudo enviar pcb");
			list_add(listaCpu, cpuLoca);
			printf("CPU al pedo añadida, LISTACPU = %d\n", listaCpu->elements_count);
		} else {
			puts("PCB enviado");
		}
		pthread_mutex_unlock(mutexListaCpu);
		return TRUE;
	} else {
		puts("No hay CPUs al pedo.");
		pthread_mutex_unlock(mutexListaCpu);
		return FALSE;
	}
}


pcb* buscarPcbPorDescriptor(t_list* lista, int descriptor){
  int i;
  pcb * pcbComparar;
  for (i = 0; i < list_size(lista); i++) {
    pcbComparar = ((pcb*) list_get(lista, i));
    if ((int)(pcbComparar->cpu->descriptor) == (int)(descriptor) ||
      (int)(pcbComparar->consola->descriptor) == (int)(descriptor)) {
      return list_get(lista, i);
    }
  }
  return NULL;
}


bool eliminarCpuPorDescriptorYDevuelveUnBool(t_list* lista, int descriptor){
  int i;
  Socket * cpu;
  for (i = 0; i < list_size(lista); i++) {
    cpu = ((Socket*) list_get(lista, i));
    if ((int)(cpu->descriptor) == (int)(descriptor)) {
      list_remove(lista, i);
      return TRUE;
    }
  }
  return FALSE;
}
