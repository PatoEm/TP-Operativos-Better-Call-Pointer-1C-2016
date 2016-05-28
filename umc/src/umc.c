#include "umc.h"
//#define manejarError(msg) {perror(msg); abort();} Quien garcha hizo esto?


void setearValores(t_config * archivoConfig) {
		puertoEscucha = config_get_string_value(archivoConfig, "PUERTO");
		ip_Swap = config_get_string_value(archivoConfig, "IP_SWAP");
		puerto_Swap = config_get_string_value(archivoConfig, "PUERTO_SWAP");
		marcos = config_get_int_value(archivoConfig, "MARCOS");
		marco_Size = config_get_int_value(archivoConfig,"MARCO_SIZE");
		marco_x_proc = config_get_int_value(archivoConfig,"MARCO_X_PROC");
		entradas_TLB = config_get_int_value(archivoConfig,"ENTRADAS_TLB");
		espera = config_get_int_value(archivoConfig,"RETARDO");
}

char * reservarMemoria(int cantidadFrames, int capacidadFrames)
{
	// Si lo hago con calloc me la llena de \0 papa
	char * memory = calloc(cantidadFrames, capacidadFrames);
	printf ("Memoria real reservada OK.\n\n");
	return memory;
}


void liberarMemoria(char * memoria_para_liberar)
{
	free(memoria_para_liberar);
	puts("Memoria liberada");
}

void escuchoMuchasConexiones(void)
{

		//ACA ME TENGO QUE HACER CLIENTE DEL SWAP
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
	    myaddr.sin_port = htons(atoi(puertoEscucha));
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
	    printf("Empieza el select\n");
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
	            	    printf("Hago el recv\n");
	                    if ((nbytes = recv(i, (void*) buf, sizeof(buf), 0)) <= 0) { //LOS DATOS LOS RECIBO ACA
	                        // error o conexión cerrada por el cliente
	                        if (nbytes == 0) {
	                            // conexión cerrada
	                            printf("Servidor select: socket %d acaba de colgar\n", i);
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

							printf("Me habla el Socket %d: ",i);
							//if (nbytes != 0) printf("%s", buf); //ESTA LINEA

							if(buf[0]=='0'){ //Aca vendria el protocolo con el nucleo en la condicion del if uso esto para probar

								hilo1 = pthread_create(&hiloNucleo, NULL, (void *)meHablaKernelPrueba, (void *)argumentoNucleo);

								hilo1 = pthread_join(hiloNucleo, NULL);
							}
							else if(buf[0]=='1'){ //Aca vendria el protocolo con el cpu en la condicion del if uso esto para probar

								hilo2 = pthread_create(&hiloCPU, NULL, (void *)meHablaCPUPrueba, (void *)argumentoCPU);

								hilo2 = pthread_join(hiloCPU, NULL);
							}
							else{

								printf("No me habló ni CPU ni Nucleo\n");
							}


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
	            }  //La asquerosidad de esto llega a niveles inhumanos
	        }
	    }
	}

void meHablaCPUPrueba (){

	printf("Hola soy el CPU\n");

}

void meHablaKernelPrueba (){

	printf("Hola soy el Nucleo\n");
}
