/*
 * nucleo.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#include "nucleo.h"
//#define manejarError(msg) {perror(msg); abort();}



void setearValores(t_config * archivoConfig) {
		puertoPropio = config_get_string_value(archivoConfig, "PUERTO_PROPIO");
		cpuPort= config_get_string_value(archivoConfig, "PUERTO_CPU");
		ipUMC= config_get_string_value(archivoConfig, "IP_UMC");
		UMCPort=config_get_string_value(archivoConfig, "PUERTO_UMC");
		quantum= config_get_int_value(archivoConfig, "QUANTUM");
		quantumSleep = config_get_int_value(archivoConfig, "QUANTUM_SLEEP");
		idSemaforos = config_get_array_value(archivoConfig,"SEM_IDS");
		viSemaforos = config_get_array_value(archivoConfig,"SEM_INIT");
		idIO = config_get_array_value(archivoConfig,"IO_IDS");
		retardoIO = config_get_array_value(archivoConfig,"IO_SLEEP");
		idVariableCompartida = config_get_array_value(archivoConfig,"SHARED_VARS");
		tamanioPaginas=config_get_int_value(archivoConfig, "MARCO_SIZE");

}

pcb crearNuevoPcb(char * programaAnsisop, int tamanioArchivo)
{

	t_medatada_program* metaNuevoPrograma = metadata_desde_literal(programaAnsisop);

	t_intructions* instrucciones = metaNuevoPrograma->instrucciones_serializado;

	t_size cantidadInstrucciones = metaNuevoPrograma->instrucciones_size;


	pcb  pcbNuevoPrograma;
	idProgramas=idProgramas+1;

	pcbNuevoPrograma.id = idProgramas;

	pcbNuevoPrograma.tamanioArchivoOriginal=tamanioArchivo;

	pcbNuevoPrograma.metaProgram = metaNuevoPrograma;

	pcbNuevoPrograma.programCounter = metaNuevoPrograma->instruccion_inicio;

	pcbNuevoPrograma.paginasDeCodigo = cantidadPaginas(tamanioPaginas, tamanioArchivo);

	//pcbNuevoPrograma.indiceDeCodigo = crearIndiceCodigo(cantidadInstrucciones, instrucciones);

	arrayBidimensional * array=(arrayBidimensional*)malloc(cantidadInstrucciones*sizeof(arrayBidimensional));

	int i=0;
	for ( i = 0;   i < cantidadInstrucciones;  i++)
		{
		array[i].comienzo=instrucciones[i].start;
		array[i].longitud=(instrucciones[i].offset);
		}

	pcbNuevoPrograma.indiceDeCodigo=array;

	pcbNuevoPrograma.indiceDeEtiquetas = metaNuevoPrograma->etiquetas;

	//El indice de stack no lo toco cuando creo?

	pcbNuevoPrograma.estado=0; //NEW

	return  pcbNuevoPrograma;
}

void moverAColaReady(pcb * programa){

	  switch(programa->estado) {
	    case 0: queue_pop(colaNew); break; //0 NEW
	    case 2: queue_pop(colaExec); break; //2 EXEC
	    case 3:  queue_pop(colaBlock); break; //3 BLOCK
	  }

	programa->estado=1; //1 READY
	queue_push(colaReady,programa);
}
void moverAColaBlock(pcb* programa){
	queue_pop(colaExec);
	programa->estado=3;//3 BLOCK
	queue_push(colaBlock,programa);

}
void moverAColaExit(pcb* programa)
{
		queue_pop(colaExec);
		programa->estado=4; // 4 EXIT
		queue_push(colaExit,programa);
}
void finalizarProcesosColaExit()
{
	//ACA DEBO ENVIAR MENSAJE A LA CONSOLA DE QUE FINALIZARON SUS PROGRAMAS

	//COMO LO HAGO?

	//LUEGO BORRO ABSOLUTAMENTE Y DESTRUYO TODO
	queue_clean_and_destroy_elements(colaExit,NULL); //ESTO NO SE SI ESTA BIEN
}

void escuchoMuchasConexiones()
{
    fd_set master;      // maestro es el conjunto de file descriptors que están actualmente conectados
    fd_set read_fds;    // conjunto temporal de descriptores de fichero para select()
    struct sockaddr_in myaddr;       // dirección del servidor
    struct sockaddr_in remoteaddr;     // dirección del cliente
    struct timeval tv;    //esta es la estructura que regula el tiempo
    tv.tv_sec = 2;    //estos son los segundos que tarda
    tv.tv_usec = 500000;  //estos son los microsegundos que tarda
    int fdmax;        // número máximo de file descriptors
    int listener;     // descriptor de socket a la escucha
    int newfd;        // descriptor de socket de nueva conexión aceptada
    char buf[256];    // buffer para datos del cliente ESTO VA A TENER EL TAMAÑO DE LO QUE MANDEMOS
    int nbytes;
    int yes=1;        // para setsockopt() SO_REUSEADDR, más abajo
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
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    // enlazar
    if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) {
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
    for(;;) {
        read_fds = master; // cópialo
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("Error en el select");
            exit(1);
        }
        // explorar conexiones existentes en busca de datos que leer
        for(i = 0; i <= fdmax; i++){
            if (FD_ISSET(i, &read_fds)) { // TENEMOS DATOS PAPA
                if (i == listener) {
                    // gestionar nuevas conexiones
                    addrlen = sizeof(remoteaddr);
                    if ((newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen)) == -1) {
                        perror("Error en el accept");
                    } else {
                        FD_SET(newfd, &master); // añadir al conjunto maestro
                        if (newfd > fdmax) {    // actualizar el máximo
                            fdmax = newfd;
                        }
                        printf("Nueva conexion de %s en ""socket %d\n", inet_ntoa(remoteaddr.sin_addr), newfd);
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
                        for(j = 0; j <= fdmax; j++) {
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

void verificarModificacionesArchivoConfig()
{
	char buffer[BUF_LEN];
	// Al inicializar inotify este nos devuelve un descriptor de archivo
	int file_descriptor = inotify_init();
	if (file_descriptor < 0) {
	perror("inotify_init");
	}
	// Creamos un monitor sobre un path indicando que eventos queremos escuchar
	int watch_descriptor = inotify_add_watch(file_descriptor, "confignucleo", IN_MODIFY);
	// El file descriptor creado por inotify, es el que recibe la información sobre los eventos ocurridos
	// para leer esta información el descriptor se lee como si fuera un archivo comun y corriente pero
	// la diferencia esta en que lo que leemos no es el contenido de un archivo sino la información
	// referente a los eventos ocurridos
	int length = read(file_descriptor, buffer, BUF_LEN);
	if (length < 0) {
	perror("read");
	}
	int offset = 0;
	// Luego del read buffer es un array de n posiciones donde cada posición contiene
	// un eventos ( inotify_event ) junto con el nombre de este.
	while (offset < length) {
	// El buffer es de tipo array de char, o array de bytes. Esto es porque como los
	// nombres pueden tener nombres mas cortos que 24 caracteres el tamaño va a ser menor
	// a sizeof( struct inotify_event ) + 24.
	struct inotify_event *event = (struct inotify_event *) &buffer[offset];
	// El campo "len" nos indica la longitud del tamaño del nombre
	if (event->len) {
	// Dentro de "mask" tenemos el evento que ocurrio y sobre donde ocurrio
	// sea un archivo o un directorio


	if (event->mask & IN_MODIFY) {
	if (event->mask & IN_ISDIR) {
	printf("The directory %s was modified.\n", event->name);
	} else {
	printf("The file %s was modified.\n", event->name);
	}
	}
	}
	offset += sizeof (struct inotify_event) + event->len;
	}
	inotify_rm_watch(file_descriptor, watch_descriptor);
	close(file_descriptor);





}


