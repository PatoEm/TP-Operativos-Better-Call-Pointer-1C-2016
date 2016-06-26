#include "swap.h"

int main(void) {

	leerArchivoDeConfiguracion("swapconfig");

	archivoMappeado = crearArchivo(tamArchivo, nombreSwap);

	serverSwap = socketCreateServer(swapPort);


	if (serverSwap == NULL){
		puts("Error no se pudo crear el server");
	}

	if(!socketListen(serverSwap)){
		puts("No me pude poner a escuchar");
	}else
		puts("Server creado y escuchando correctamente");

	umcClient =socketAcceptClient(serverSwap);
	while(1){
		buffer = socketReceive(umcClient);
		if (buffer == NULL)
			puts("Error al recibir del cliente");
		streamUmcSwap=unserializeUmcSwa(buffer);

		bool programaRecibido;

		switch(streamUmcSwap->action){
			case /*RECIBIR_NUEVO_PROGRAMA*/12:
				programaRecibido = recibirNuevoPrograma(streamUmcSwap->pid,streamUmcSwap->cantPage,streamUmcSwap->pageComienzo);
				if(programaRecibido == 0){
					streamSwapUmc=newStrSwaUmc(SWAP_ID,/*PROGRAMA_NO_RECIBIDO*/"16",NULL,0,NULL,0,streamUmcSwap->pid);
					buffer = serializeSwaUmc(streamSwapUmc);
					if(!socketSend(umcClient,buffer))
						puts("Error al enviar el paquete");
				}
				break;

			case /*LEER_UNA_PAGINA*/13:
				break;

			case /*ESCRIBIR_UNA_PAGINA*/14:
				break;

			case /*ELIMINAR_PROCESO*/15:
				break;

			default:
				puts("HARRY TIENE UN PROBLEMA");
				break;
		}
	}


}
