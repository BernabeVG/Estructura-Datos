
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostring.h>
#include <HashQ.h>
#include "Libro.h"


int folding(void *);

int main(void)
{
	HashQTable tablahash;
	tablahash.table = calloc(1000,sizeof(void*));
	tablahash.len = 1000;
	tablahash.cant = 0;
	tablahash.min = 30;
	tablahash.max = 70;
	tablahash.fixed = (tablahash.min+tablahash.max)/2;
	tablahash.folding = folding;
	tablahash.imprimir = imprimirLibro;
	tablahash.comparar = compararTitulo;
	tablahash.verbose = 0;
	
	
	Acervo acervo = obtener_libros();
	
	for(int i = 0; i < acervo.cantidad ; i++)
	{
		printf("\n");
		imprimirLibro(acervo.libros+i);
		fn_insertKey(&tablahash,acervo.libros+i);		
	}
	remap(&tablahash);

	printf("\nTABLA");
	for(int i = 0; i < tablahash.len ; i++)
	{		
		printf("\n[%d]",i);
		if( tablahash.table[i]!=NULL)		
		{
			imprimirLibro(tablahash.table[i]);			
		}
	}
	
	tablahash.verbose = 1;
	Libro libro = {"","Dracula","",0,0};
	Libro *aqui = fn_searchKey(&tablahash,&libro);
	if(aqui)
	{
		printf("\n LIBRO ENCONTRADO:");
		imprimirLibro(aqui);
	}
	printf("\n\n FIN DE PROGRAMA\n");
	
	
	
	return 0;
}


int folding(void *data)
{
	char *cadena = ((Libro*)data)->titulo;
	int key = 0,sector = 1,sumatoria=0;
	for(int i = 0; cadena[i]!='\0'; i++)
	{
		//abc de
		sumatoria += cadena[i];
		if( i!=0 && (i+1)%3 == 0)
		{
			key+= sector*sumatoria;
			sumatoria = 0;
			sector++;
		}
	}	
	key+= sector*sumatoria;
	return key;
}
