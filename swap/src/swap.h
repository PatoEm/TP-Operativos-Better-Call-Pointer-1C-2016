/*

 * swap.h
 *
 *  Created on: 23/4/2016
 *      Author: utnso
 */

#ifndef SWAP_H_
#define SWAP_H_

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <libreriasCompartidas/archivosYLogsYMas.h>
#include <libreriasCompartidas/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <net/if.h>
#include <wait.h>
#include <errno.h>          /* errno, ECHILD            */

//define
#define FAIL -1
#define RECIBIRTAMANIO 1
#define FRAGMENTACION_EXTERNA -2
#define TRUE 1
#define FALSE 0

//variables
char* ipUMC;
char* umcPort;
char* puerto_Escucha;
char* nombreSwap;
char* paginas;
char* tamPagina;
char* retCompactacion;
char* tamArchivo;
char* archivoMappeado;

//estructuras

typedef struct {
	int IDPaginaInterno;
	int pid;
	bool bitLectura;
	int numDePag;

} espacioAsignado;

bool*bitMap;
t_list * listaEspacioAsignado;

//prototipos
char* crearArchivo(char*, char*);

void* mappearArchivo(char*);

void setearValores(t_config *);

bool escribirPagina(int pid, int numeroDePagina, char*pagina);

char* leerUnaPagina(int pid, int numeroDePagina);

void iniciarEstructurasSwap();

int calcularIDPagina(int);

void agregarEspacioLibre(int);

espacioAsignado* crearEspacioAsignado(bool, int, int, int);

void agregarEspacioAsigando(bool, int, int, int, int);

bool verificarSiHayEspacio(int);

int paginasContiguasDeSwap(int);

void eliminarProceso(int pid);

void reservarPaginas(int paginaDeComienzo, int pid, int cantidadDePaginas,
		int numeroPrimerPagina);

bool recibirNuevoPrograma(int pid, int cantidadDePaginasAGuardar,
		int pgDeComienzo);

void crearListas();

void compactarSwap();

#endif /* SWAP_H_ */
