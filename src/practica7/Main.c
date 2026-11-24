#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostring.h>
#include <Nodo.h>
#include <heap.h>

typedef struct {
	char nombre[50];
	int paginas;
} Archivo;

int compararArchivos(void*, void*);
void imprimirArchivo(void *dato);
Nodoa* crearNodoArbolDesdeHeap(Heap *heap, int i);
void liberarArbol(Nodoa *raiz);
Archivo* crearArchivoAleatorio(void);

int main(void)
{
	srand((unsigned int)time(NULL));
	int opc = 0;
	Heap heap = {NULL, 0, HEAPMIN, compararArchivos};

	do {
		// Vista previa de los primeros 5 archivos arriba del menu
		printf("\n\n----PRIMEROS 5 ARCHIVOS EN COLA (%s)----",
			   heap.type == HEAPMIN ? "HEAP MINIMO" : "HEAP MAXIMO");
		if (heap.cant == 0)
		{
			printf("\n(Cola vacia)");
		}
		else
		{
			size_t limite = heap.cant < 5 ? heap.cant : 5;
			for (size_t i = 0; i < limite; i++)
			{
				printf("\n  %zu. ", i + 1);
				imprimirArchivo(heap.datos[i]);
			}
		}

		printf("\n\n     ---MENU---");
		printf("\n0. Cambiar Prioridad.");
		printf("\n1. Mostrar cola de impresion.");
		printf("\n2. Agregar Archivo.");
		printf("\n3. Procesar/Imprimir Archivo");
		printf("\n4. Eliminar Archivo.");
		printf("\n5. Eliminar todos los archivos.");
		printf("\n6. Terminar Programa.");
		printf("\nSelecciona una Opcion: ");
		scanf("%d", &opc);
		while (getchar() != '\n');

		switch (opc)
		{
			case 0:
				if (heap.type == HEAPMIN) {
					heap.type = HEAPMAX;
					printf("\nPrioridad Cambiada: Mayor numero de paginas primero.");
				}
				else {
					heap.type = HEAPMIN;
					printf("\nPrioridad Cambiada: Menor numero de paginas primero.");
				}
				if (heap.cant > 1) {
					heapify(&heap);
				}
				break;

			case 1:
				if (heap.cant == 0) {
					printf("\nLa cola de impresion esta vacia.");
				}
				else {
					// arreglo
					imprimirHeap(&heap, imprimirArchivo);
					// arbol
					printf("\n\n---Heap version arbol---\n");
					Arbol arbol = {NULL, 0, ASCENDENTE};
					arbol.raiz = crearNodoArbolDesdeHeap(&heap, 0);
					arbol.cant = (int)heap.cant;

					imprimirArbol(arbol, imprimirArchivo);
					liberarArbol(arbol.raiz);
				}
				break;

			case 2:
			{
				Archivo *nuevo = crearArchivoAleatorio();
				printf("\nArchivo Generado Aleatorio: ");
				imprimirArchivo(nuevo);
				insertarHeap(&heap, nuevo);
				printf("\nAgregado a la cola correctamente.");
				break;
			}

			case 3:
			{
				if (heap.cant == 0)
				{
					printf("\nNo hay archivos en la cola para procesar.");
				}
				else
				{
					Archivo *procesado = (Archivo *)heap.datos[0];
					printf("\nProcesando e imprimiendo: ");
					imprimirArchivo(procesado);
					free(procesado);

					heap.datos[0] = heap.datos[heap.cant - 1];
					heap.cant--;

					if (heap.cant == 0)
					{
						free(heap.datos);
						heap.datos = NULL;
					}
					else if (heap.cant > 1)
					{
						heapify_down(&heap);
					}
					printf("\nArchivo impreso y eliminado de la cola.");
				}
				break;
			}

			case 4:
			{
				if (heap.cant == 0)
				{
					printf("\nLa cola de impresion esta vacia.");
				}
				else
				{
					printf("\n--- Archivos en la cola ---");
					for (size_t i = 0; i < heap.cant; i++)
					{
						printf("\n  [%zu] ", i);
						imprimirArchivo(heap.datos[i]);
					}

					int idx = -1;
					printf("\nIngresa el indice a eliminar (0 - %zu): ", heap.cant - 1);
					scanf("%d", &idx);
					while (getchar() != '\n');

					if (idx < 0 || idx >= (int)heap.cant)
					{
						printf("\nIndice invalido.");
					}
					else
					{
						printf("\nEliminando archivo: ");
						imprimirArchivo(heap.datos[idx]);

						free(heap.datos[idx]);
						heap.datos[idx] = heap.datos[heap.cant - 1];
						heap.cant--;

						if (heap.cant == 0)
						{
							free(heap.datos);
							heap.datos = NULL;
						}
						else if (heap.cant > 1)
						{
							heapify(&heap);
						}
						printf("\nArchivo eliminado correctamente.");
					}
				}
				break;
			}

			case 5:
			{
				if (heap.cant == 0)
				{
					printf("\nLa cola ya esta vacia.");
				}
				else
				{
					for (size_t i = 0; i < heap.cant; i++)
					{
						free(heap.datos[i]);
					}
					free(heap.datos);
					heap.datos = NULL;
					heap.cant = 0;
					printf("\nTodos los archivos han sido eliminados.");
				}
				break;
			}

			case 6:
			{
				for (size_t i = 0; i < heap.cant; i++)
				{
					free(heap.datos[i]);
				}
				free(heap.datos);
				heap.datos = NULL;
				heap.cant = 0;
				break;
			}
		}
	} while (opc != 6);

	printf("\n\n FIN DE PROGRAMA\n\n");
	return 0;
}

Nodoa* crearNodoArbolDesdeHeap(Heap *heap, int i)
{
	if (i >= (int)heap->cant)
		return NULL;
	Nodoa *nodo = crearNodoa(heap->datos[i]);
	nodo->izq = crearNodoArbolDesdeHeap(heap, 2 * i + 1);
	nodo->dch = crearNodoArbolDesdeHeap(heap, 2 * i + 2);
	return nodo;
}

void liberarArbol(Nodoa *raiz)
{
	if (!raiz) return;
	liberarArbol(raiz->izq);
	liberarArbol(raiz->dch);
	free(raiz);
}

Archivo* crearArchivoAleatorio(void)
{
	Archivo *nuevo = calloc(1, sizeof(Archivo));
	const char *nombres[] = {
		"Tarea", "Reporte", "Assembly", "Lenguaje C",
		"Crepusculo", "Guia Examen", "48 leyes del poder", "Tesis", "Manga"
	};

	strcpy(nuevo->nombre, nombres[rand() % 9]);
	nuevo->paginas = (rand() % 100) + 1;
	return nuevo;
}

void imprimirArchivo(void *dato)
{
	Archivo *arch = (Archivo*)dato;
	printf("[%s | %d paginas]", arch->nombre, arch->paginas);
}

int compararArchivos(void *pa, void *pb)
{
	Archivo *a = (Archivo*)pa;
	Archivo *b = (Archivo*)pb;
	if (a->paginas < b->paginas) return -1;
	if (a->paginas > b->paginas) return 1;
	return 0;
}