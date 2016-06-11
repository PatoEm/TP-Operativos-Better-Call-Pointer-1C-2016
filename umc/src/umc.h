#ifndef UMC_H_
#define UMC_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/config.h>
#include <commons/log.h>
#include <libreriasCompartidas/archivosYLogsYMas.h>
#include <libreriasCompartidas/socket.h>
#include <pthread.h>
#include <commons/collections/list.h>
#define FAIL -1
#define PROGRAMA_NO_INICIADO -2

//parametros
char* memoriaReal;
char* puertoEscucha;
char* ip_Swap;
char* puerto_Swap;
int marcos;
int marco_Size;
int marco_x_proc;
char* algoritmoDeReemplazo;
int entradas_TLB;
int espera;

  typedef struct{
	int IDFrame;
  	int pid;
  	int numPagina;
  	int frameDelPrograma; //ESTE CAMPO SE TIENE QUE BORRAR PERO NO LA BORRO AHORA PARA QUE NO ROMPA TODO
  	int posicionDePag; //ESTE CAMPO SE TIENE QUE BORRAR PERO NO LA BORRO AHORA PARA QUE NO ROMPA TODO
  }espacioAsignado;

  //ESTA ESTRUCTURA SE TIENE QUE BORRAR PERO NO LA BORRO AHORA PARA QUE NO ROMPA TODO
  typedef struct{
	int IDFrame;
	int inicio;
  }espacioLibre;

  typedef struct{
	  int pid;
	  int frame;
	  int pagina;
  }t_tlb;

bool* bitMap;
t_list * listaEspacioAsignado;
//ESTA LISTA SE TIENE QUE BORRAR PERO NO LA BORRO PARA QUE NO ROMPA TODO
t_list * listaEspacioLibre;

//estructuras para los hilos de CPU y nucleo

typedef struct{
	//en estos campos van las cosas que recibe el hilo para laburar con el nucleo
}t_nucleo;

typedef struct{
	//en estos campos van las cosas que recibe el hilo para laburar con el CPU
}t_CPU;

//Prototipos
void insertarNodoOrdenadoLibre(int inicio, int cantidad, int IDFrame);

void eliminarFramesOcupadosContiguos(int cantidad, int frame);

//espacioLibre * crearEspacioLibre(int inicio) ;

void agregarEspacioLibre(int inicio);

int calcularIDPagina(int inicio);

void iniciarEstructuras();

int cantidadDePaginasLibres();

bool hayMemoriaSuficiente(int paginas);

int paginasContiguasDeMemoria(int cantidadDePaginas);

bool insertarEnListaAsignadoOrdenado(int pid, int cantidadDeNodos, int idFrame) ;

char * reservarMemoria(int , int );

void comandosUMC ();

void menuUMC();

void retardoUMC (int);

void dump ();

void flushTLB ();

void flushMemory ();

void liberarMemoria(char * );

void escuchoMuchasConexiones();

void meHablaKernel();

void meHablaCPU();

void meHablaKernelPrueba ();

void meHablaCPUPrueba ();

void elDestructorDeNodosTLB();

void inicioTLB(t_list *, int, int);

t_list * creoTLB();

#endif /* UMC_H_ */
