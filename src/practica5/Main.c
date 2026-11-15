#include <stdio.h>
#include <stdlib.h>
#include <iostring.h>
#include <arbol.h>
#include <Nodo.h>

#include "Lista.h"

int *crearEntero(int);
int compararEntero(void *a, void *b);
void imprimir(void*);
void insertarArbolO(Arbol *arbol, void *dato, fn_comparar comparar);
void flipArbol(Arbol *arbol);
void* buscarArbol(Arbol *arbol,void *dato, fn_comparar comparar);
void equilibrarArbol(Arbol *arbol, fn_comparar comparar);
void eliminarDatoArbol(Arbol *arbol, void *dato, fn_comparar comparar);



int main(void)
{
	int opc;
/*	Nodoa *raiz = crearNodoa( crearEnterequilibrarArbol(Arbol *arbol);o(4) );
	raiz->izq = crearNodoa(crearEntero(2) );
	raiz->izq->izq = crearNodoa(crearEntero(1) );
	raiz->izq->dch = crearNodoa(crearEntero(3) );
	raiz->dch =  crearNodoa(crearEntero(6) );	
	raiz->dch->izq =  crearNodoa(crearEnoid* buscarArbol(Arbol *arbol,void *dato);tero(5) );
	raiz->dch->dch = crearNodoa(crearEntero(7) );*/

	Arbol arbol={NULL,0,ASCENDENTE};
	int dato;
	//arbol.raiz = raiz;
	do{
	//menu
		printf("\n---MENU---");
		printf("\n1. Insertar Arbol.");
		printf("\n2. Eliminar dato de un Arbol.");
		printf("\n3. Equilibrar el arbol.");
		printf("\n4. Intercambiar ramas del arbol");
		printf("\n5. Buscar dato en el arbol.");
		printf("\n6. Mostrar arboles.");
		printf("\n7. Terminar Programa.");
		printf("\nSelecciona una opcion: ");
		scanf("%d", &opc);

	switch (opc) {
		case 1:
			printf("\nIngresa un nuevo valor al arbol: ");
			scanf("%d",&dato);
			insertarArbolO(&arbol, crearEntero(dato), compararEntero);
			break;
		case 2:
			printf("Ingrese el valor a eliminar: ");
			scanf("%d", &dato);
			eliminarDatoArbol(&arbol, &dato, compararEntero);
			break;
		case 3:
			equilibrarArbol(&arbol, compararEntero);
			printf("\nArbol equilibrado.");
			break;
		case 4:
			flipArbol(&arbol);
			printf("\nRamas del arbol intercambiadas.");
			break;
		case 5:
			printf("\nIngrese el dato que desea buscar?: ");
			scanf("%d",&dato);
			int *comparar = (int*)buscarArbol(&arbol, &dato, compararEntero);

			if (comparar !=NULL)
				printf("\nEl dato %d, si se encuentra en el arbol." , *comparar);
			else
				printf("\nEl dato %d, no se encuentra", dato);
			break;
		case 6:
			printf("\n Altura %zu", altura(arbol));
			printf("\n Altura %zu", alturaI(arbol));
			printf("\n PREORDEN");
			imprimirOrden(arbol,imprimir,PREORDEN);
			printf("\n ORDEN");
			imprimirOrden(arbol,imprimir,ORDEN);
			printf("\n POSTORDEN");
			imprimirOrden(arbol,imprimir,POSTORDEN);


			printf("\n Arbol\n");
			imprimirArbol(arbol,imprimir);
			break;
		case 7:
			printf("\n\n FIN DE PROGRAMA\n\n");
			break;
		default:
			printf("\n Opcion no valida");
			break;
	}
	}while(opc != 7);
    return 0;
}


int *crearEntero(int num)
{
	int *dato = calloc(1,sizeof(int));
	*dato = num;
	return dato;
}
int compararEntero(void *a, void *b)
{
	int *aa = (int*)a;
	int *bb = (int*)b;

	if (*aa < *bb)
		return -1;
	else if (*aa > *bb)
		return 1;
	else
		return 0;
}

void imprimir(void *dato)
{
	printf("%d",*(int*)dato);
}
void insertarArbolO(Arbol *arbol, void *dato, fn_comparar comparar)
{
	if (!arbol) return;
	Nodoa *nuevo = crearNodoa(dato);
	if (!arbol->raiz) {
		arbol->raiz = nuevo;
		arbol->cant++;
		return;
	}
	Pila pila = inicializarPila(-1);
	Nodoa *actual = arbol->raiz;
	Nodoa *padre = NULL;
	int comparacion = 0;

	while (actual != NULL) {
		push(&pila, actual);
		padre = actual;
		comparacion = comparar(dato,actual->dato);

		if (comparacion == 0) {
			free(nuevo);
			eliminarPila(&pila, NULL);
			return;
		}
		if (comparacion <0)
			actual = actual->izq;
		else
			actual = actual->dch;
	}
	if (comparacion <0)
		padre->izq = nuevo;
	else
		padre->dch= nuevo;
	arbol->cant++;
	eliminarPila(&pila, NULL);
}

void flipArbol(Arbol *arbol)
{
	if (!arbol || !arbol->raiz) {return;}
	Pila pila = inicializarPila(-1);
	push(&pila, arbol->raiz);
	while (!pilaVacia(pila))
	{
		Nodoa *actual = pop(&pila);

		Nodoa *aux = actual->izq;
		actual->izq = actual->dch;
		actual->dch = aux;

		if (actual->izq!=NULL)
			push(&pila, actual->izq);
		if (actual->dch!=NULL)
			push(&pila, actual->dch);
	}
	if (arbol->orden == ASCENDENTE)
		arbol->orden = DESCENDENTE;
	else if (arbol->orden == DESCENDENTE)
		arbol->orden = ASCENDENTE;
	eliminarPila(&pila,NULL);;
}

void* buscarArbol(Arbol *arbol, void *dato, fn_comparar comparar)
{
	if (!arbol || !arbol->raiz) {return NULL;}

	Pila pila = inicializarPila(-1);
	Nodoa *actual = arbol->raiz;
	int comparacion=0;

	while (actual != NULL){

		push(&pila, actual);
		comparacion = comparar(dato,actual->dato) * arbol->orden;

		if (comparacion == 0) {
			eliminarPila(&pila,NULL);
			return actual->dato;
		}
		if (comparacion <0)
			actual=actual->izq;
		if (comparacion >0)
			actual=actual->dch;
	}
	eliminarPila(&pila,NULL);
	return NULL;
}
void equilibrarArbol(Arbol *arbol, fn_comparar comparar)
{
	if (!arbol || !arbol->raiz || arbol->cant <= 2)
		return;

	size_t total = arbol->cant;
	void *datos[total]; // Guarda los punteros a los datos sin malloc extra

	//Sacar los datos ya ordenados (Inorden) y liberar los nodos viejos
	Pila pila = inicializarPila(-1);
	Nodoa *actual = arbol->raiz;
	size_t i = 0;

	while (actual != NULL || !pilaVacia(pila))
	{
		while (actual != NULL)
		{
			push(&pila, actual);
			actual = actual->izq;
		}
		actual = (Nodoa*)pop(&pila);
		datos[i++] = actual->dato;

		Nodoa *derecho = actual->dch;
		free(actual);
		actual = derecho;
	}

	//Reiniciar el árbol y reinsertar desde el centro usando tu insertarArbolO
	arbol->raiz = NULL;
	arbol->cant = 0;

	push(&pila, crearEntero(0));
	push(&pila, crearEntero((int)total - 1));

	while (!pilaVacia(pila))
	{
		int *ptrFin = (int*)pop(&pila);
		int *ptrIni = (int*)pop(&pila);
		int ini = *ptrIni;
		int fin = *ptrFin;
		free(ptrFin);
		free(ptrIni);

		if (ini <= fin)
		{
			int mitad = (ini + fin) / 2;
			insertarArbolO(arbol, datos[mitad], comparar);

			if (mitad + 1 <= fin)
			{
				push(&pila, crearEntero(mitad + 1));
				push(&pila, crearEntero(fin));
			}
			if (ini <= mitad - 1)
			{
				push(&pila, crearEntero(ini));
				push(&pila, crearEntero(mitad - 1));
			}
		}
	}
	eliminarPila(&pila, free);
}

void eliminarDatoArbol(Arbol *arbol, void *dato, fn_comparar comparar)
{
    if (!arbol || !arbol->raiz || !comparar)
    {
        return;
    }

    int sentido = arbol->orden;
    if (sentido == NINGUNO)
    {
        sentido = ASCENDENTE;
    }

    Pila pila = inicializarPila(-1);
    Nodoa *actual = arbol->raiz;
    int comparacion = 0;

    // 1. Buscar el nodo guardandotodo el recorrido de ancestros en la Pila
    while (actual != NULL)
    {
        comparacion = comparar(dato, actual->dato) * sentido;

        if (comparacion == 0)
        {
            break;
        }

        push(&pila, actual);

        if (comparacion < 0)
        {
            actual = actual->izq;
        }
        else
        {
            actual = actual->dch;
        }
    }

    // Si no se encontró el dato, limpiar pila y salir
    if (actual == NULL)
    {
        eliminarPila(&pila, NULL);
        printf("\nEl dato no se encuentra en el arbol.\n");
        return;
    }

    // 2. Caso con 2 hijos: bajar al sucesor inorden apilando el camino
    if (actual->izq != NULL && actual->dch != NULL)
    {
        push(&pila, actual);
        Nodoa *sucesor = actual->dch;

        while (sucesor->izq != NULL)
        {
            push(&pila, sucesor);
            sucesor = sucesor->izq;
        }

        // Liberar dato original y transferir el del sucesor
        free(actual->dato);
        actual->dato = sucesor->dato;

        // Ahora el nodo físico a eliminar es 'sucesor'
        actual = sucesor;
    }
    else
    {
        // En casos de 0 o 1 hijo, liberar su dato directamente
        free(actual->dato);
    }

    // 3. Obtener al padre directo desde la cima de la Pila (NULL si era la raíz)
    Nodoa *padre = NULL;
    if (!pilaVacia(pila))
    {
        padre = (Nodoa*)peek(pila);
    }

    Nodoa *hijo = NULL;
    if (actual->izq != NULL)
    {
        hijo = actual->izq;
    }
    else
    {
        hijo = actual->dch;
    }

    if (padre == NULL)
    {
        arbol->raiz = hijo;
    }
    else if (padre->izq == actual)
    {
        padre->izq = hijo;
    }
    else
    {
        padre->dch = hijo;
    }

    free(actual);
    arbol->cant--;
    eliminarPila(&pila, NULL);
    printf("\nDato eliminado correctamente.\n");
}