/*
 * holaLoco.c
 *
 *  Created on: 3/5/2016
 *      Author: utnso
 */

#include "archivosAnsisop.h"

char * leerProgramaAnsisop(char * direccionArchivo){


	long tamanio = tamanioArchivo(direccionArchivo);

	FILE * fp;

	fp=fopen(direccionArchivo,"r");

	if (fp==NULL) puts ("Error al abrir archivo");
	else{
	char * buffer=(char *)malloc(tamanio);
	fseek( fp,0 ,0);
	int n=0;
	while(!feof(fp)){
	buffer[n]=getc(fp);
	n++;
	}
	fclose(fp);
	printf("%s",buffer);
	return buffer;

	}
	return "";
}

long tamanioArchivo(char * direccionArchivo)
{
	FILE * fp;
	  long tamanio;

	  fp = fopen (direccionArchivo,"r");
	  if (fp==NULL) puts ("Error al abrir archivo");
	  else
	  {
	    fseek (fp, 0, SEEK_END);
	    tamanio=ftell (fp);
	    fclose (fp);
	    printf ("El tamaño de %s es: %ld bytes.\n\n",direccionArchivo,tamanio);
	  }

return tamanio;
}
