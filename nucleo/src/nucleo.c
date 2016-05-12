/*
 * nucleo.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#include "nucleo.h"
#include <pthread.h>
#define manejarError(msg) {perror(msg); abort();}


typedef struct{
	int id;
	t_puntero_instruccion programCounter;
	int paginasDeCodigo;
	int indiceDeEtiquetas;
	int indiceDelStack;
	t_medatada_program metaProgram;
}pcb;


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

}

pcb crearNuevoPcb(char * programaAnsisop)
{
	pcb pcbNuevoPrograma;
	idProgramas=idProgramas+1;
	pcbNuevoPrograma.id = idProgramas;

	t_medatada_program metaNuevoPrograma = metadata_desde_literal(programaAnsisop);

	pcbNuevoPrograma.metaProgram = metaNuevoPrograma;

	pcbNuevoPrograma.programCounter = metaNuevoPrograma.instruccion_inicio;

	return  pcbNuevoPrograma;
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
                        // tenemos datos de algún cliente
                        for(j = 0; j <= fdmax; j++) {
                            // le envio a tooodo el mundo
                        	//pthread_t hilo1;
                        	//int a = pthread_create(&hilo1,NULL,*funcionQueHaceElHIlo, argumentos);

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



