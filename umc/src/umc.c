/*
 * umc.c
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */
#include "umc.h"
#define manejarError(msg) {perror(msg); abort();}


void setearValores(t_config * archivoConfig) {
		//puertoTCP = config_get_string_value(archivoConfig, "PUERTO");
		puertoNucleo = config_get_string_value(archivoConfig, "PUERTO_NUCLEO");
		puertoCPU = config_get_string_value(archivoConfig, "PUERTO_CPU");
		ip_Swap = config_get_string_value(archivoConfig, "IP_SWAP");
		puerto_Swap = config_get_string_value(archivoConfig, "PUERTO_SWAP");
		marcos = config_get_int_value(archivoConfig, "MARCOS");
		marco_Size = config_get_int_value(archivoConfig,"MARCO_SIZE");
		marco_x_proc = config_get_int_value(archivoConfig,"MARCO_X_PROC");
		entradas_TLB = config_get_int_value(archivoConfig,"ENTRADAS_TLB");
		espera = config_get_int_value(archivoConfig,"RETARDO");
		//puertoTCP = config_get_string_value(archivoConfig, "PUERTO");
}

char * reservarMemoria(int cantidadFrames, int capacidadFrames)
{
	// Si lo hago con calloc me la llena de \0 papa
	char * memory = calloc(cantidadFrames, capacidadFrames);
	printf ("Memoria real reservada OK.\n\n");
	return memory;
}

//********************************************************SI, SEPARO LAS FUNCIONES ASI, AL QUE NO LE GUSTE ME LO COMO EN UN PATY

void liberarMemoria(char * memoria_para_liberar)
{
	free(memoria_para_liberar);
	puts("Memoria liberada");
}

//******************************************************************************

/*
int escuchoMuchasConexiones()
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
    char buf[256];    // buffer para datos del cliente
    int nbytes;
    int yes=1;        // para setsockopt() SO_REUSEADDR, más abajo
    int addrlen;
    int i, j;
    FD_ZERO(&master);    // borra los conjuntos maestro y temporal
    FD_ZERO(&read_fds);

    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons();
    memset(&(myaddr.sin_zero), '\0', 8);

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

    return 0;
}
*/
