#include "cpuHandler.h"
#include "cpu.h"

fd_set master;
fd_set read_fds;

int fdmax;

Socket* serverSocket;

Boolean initServer() {

	serverSocket = socketCreateServer(2020); //la comento por lo mismo de arriba, paja
	if (serverSocket == NULL) {
		puts("No se pudo crear el server escucha.");
		return FALSE;
	}

	if (!socketListen(serverSocket)) {
		puts("No se pudo poner a escuchar al server.");
	}

	puts("Server creado con exito y escuchando.");
	return TRUE;
}



//
//void checkConections() {
//
//	//SETEO A "0" EL MASTER Y EL TEMPORAL
//	FD_ZERO(&master);
//	FD_ZERO(&read_fds);
//
//	//CARGO EL SOCKET DE ESCUCHA A MASTER
//	FD_SET(serverSocket->descriptor, &master);
//
//	//CARGO EL SOCKET MAS GRANDE
//	fdmax = serverSocket->descriptor;
//
//	struct timeval timeout;
//
//	//BUCLE PRINCIPAL
//	while (TRUE) {
//
//		read_fds = master;
//		timeout.tv_sec = 10;
//		timeout.tv_usec = 0;
//
//		//HAGO EL SELECT
//		int selectResult = select(fdmax + 1, &read_fds, NULL, NULL, &timeout);
//
//		if (selectResult == -1) {
//
//			break;
//
//
//		} else {
//
//			//RECORRO TODOS LOS DESCRIPTORES MONITOREADOS PARA VER QUIEN LLAMO
//			int i;
//			for (i = 0; i <= fdmax; i++) {
//
//				if (FD_ISSET(i, &read_fds)) {
//
//					//FUE EL SOCKET DE ESCUCHA??
//					if (i == serverSocket->descriptor) {
//
//						//SI, ENTONCES GESTIONO LA NUEVA CONEXION ENTRANTE
//						Socket* clientSocket = socketAcceptClient(serverSocket);
//						newClientHandler(clientSocket);
//
//						//LO CARGO A LA LISTA DE DESCRIPTORES A MONITOREAR Y ACTUALIZO EL MAXIMO
//						FD_SET(clientSocket->descriptor, &master);
//
//						if (clientSocket->descriptor > fdmax) {
//							fdmax = clientSocket->descriptor;
//						}
//
//					} else {
//						//NO, ENTONCES GESTIONO EL SOCKET QUE HABLO...
//						clientHandler(i);
//					}
//				}
//			}
//		}
//	}
//}
