#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <Nodo.h>
#include <Pila.h>
#include <arbol.h>

typedef enum
{
	HEAPMIN = -1,
	HEAPMAX = 1	
}HeapType;

typedef struct
{
	void **datos;
	size_t cant;
	HeapType type;
	fn_comparar comparar;
}Heap;

void insertarHeap(Heap*,void*);
void* removerHeap(Heap*);
void imprimirHeap(Heap*,fn_imprimir);

void intercambio(Heap*,Pila,int*);
void heapify_up(Heap *);
void heapify(Heap*) __attribute__((alias("heapify_up")));
/*
typedef void (*Heapify)(Heap*);
const Heapify heapify = heapify_up;
*/
void heapify_down(Heap *);
#endif