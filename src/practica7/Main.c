#include <stdio.h>
#include <stdlib.h>
#include <iostring.h>
#include <Nodo.h>
#include <heap.h>

int *crearEntero(int);
int compararEntero(void*,void*);
void imprimir(void*);


int main(void)
{
	Heap heap = {NULL,0,HEAPMIN,compararEntero};
	int array[11] =  { 3, 5, 8, 10, 17, 11, 13, 19, 22, 24, 29};
	for(int i=0;i<11;i++)
	{
		insertarHeap(&heap,&array[i]);
	}
	imprimirHeap(&heap,imprimir);
	heapify_up(&heap);
	imprimirHeap(&heap,imprimir);
	
	printf("\n\n FIN DE PROGRAMA\n\n");
    return 0;
}


int *crearEntero(int num)
{
	int *dato = calloc(1,sizeof(int));
	*dato = num;
	return dato;
}

void imprimir(void *dato)
{
	printf("%d",*(int*)dato);
}

int compararEntero(void *pa,void *pb)
{
	int a = *(int*)pa;
	int b = *(int*)pb;
	if(a<b)return -1;
	if(a>b)return 1;
	else return 0;
}