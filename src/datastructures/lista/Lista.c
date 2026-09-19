#include "Lista.h"

Lista inicializarLista(void)
{
	return (Lista){.inicio=NULL,.fin=NULL,.cant=0,.orden=NINGUNO};
}

void imprimirLista(Lista lista,fn_imprimir imprimir)
{
	Nodo *aux;
	if(!lista.inicio)
	{
		printf("\n Lista vacia");
		return;
	}	
	printf("\n [CANT %lu]Lista: \n",lista.cant);
	aux = lista.inicio;
	while( aux )
	{
		imprimir(aux->dato);
		printf("\n");
		aux = aux->sig;
	}
}

void eliminarLista(Lista *lista,fn_free liberar)
{
	if( !lista->inicio) return;
	Nodo *aux;
	while( lista->inicio)
	{
		aux = lista->inicio;
		lista->inicio = lista->inicio->sig;
		if(liberar)
			liberar(aux->dato);
		free(aux);
	}
	lista->fin = lista->inicio;
	lista->cant = 0;
}

void insertarFinal(Lista *lista,void *dato)
{
	if(!lista->inicio)
	{
		lista->inicio = lista->fin = crearNodo(dato);
		lista->cant++;
		return;
	}
	lista->fin->sig = crearNodo(dato);
	lista->fin = lista->fin->sig;
	lista->cant++;
}

void eliminarDato(Lista *lista,void *dato,fn_comparar comparar)
{	
	if(!lista->inicio)
		return;
	
	for(Nodo *ant=NULL,*elim=lista->inicio ; elim ; ant = elim , elim = elim->sig)
	{
		if( comparar( dato , elim->dato ) == 0)
		{
			if( lista->inicio == elim)
				lista->inicio = elim->sig;
			else
				ant->sig = elim->sig;
			if( lista->fin == elim)
				lista->fin = ant;
			free(elim);
			lista->cant--;
			return;
		}
	}	
}

int buscarDato(Lista lista,void *dato,fn_comparar comparar)
{
	if(!lista.inicio)
		return 0;
	while(lista.inicio)
	{
		if(comparar( dato, lista.inicio->dato ) == 0)
			return 1;
		lista.inicio = lista.inicio->sig;
	}
	
	return 0;
}

void ordenarLista(Lista *lista,fn_comparar comparar,Orden orden)
{
	lista->comparar = comparar;
	lista->orden = orden;
	if(orden == NINGUNO)
		return;
	for( Nodo *i = lista->inicio ; i ; i = i->sig)
	{
		Nodo *cmp = i;
		for( Nodo *j = i->sig ; j ; j = j->sig)
		{
			if(orden == ASCENDENTE && comparar(cmp->dato,j->dato)>0)
				cmp = j;
			else if(orden == DESCENDENTE && comparar(cmp->dato,j->dato)<0)
				cmp = j;			
		}
		if(cmp!=i)
			swapNodo(i,cmp);
	}
}
void insertarOrdenado(Lista *lista, void *dato)
{
	if (!lista) return;

	// Si no se ha ordenado, se va al final
	if (!lista->comparar || lista->orden == NINGUNO)
	{
		insertarFinal(lista, dato);
		return;
	}

	if (!lista->inicio)
	{
		insertarFinal(lista, dato);
		return;
	}
	//Ordenamiento de comparacion
	fn_comparar comparar = lista->comparar;
	Nodo *anterior = NULL;
	Nodo *actual = lista->inicio;

	for ( ; actual != NULL; anterior = actual, actual = actual->sig)
	{
		int comp = comparar(dato, actual->dato);


		if (lista->orden == ASCENDENTE && comp < 0)
		{
			break;
		}

		if (lista->orden == DESCENDENTE && comp > 0)
		{
			break;
		}
	}

	if (anterior == NULL)
	{
		Nodo *nuevo = crearNodo(dato);
		if (!nuevo) return;
		nuevo->sig = lista->inicio;
		lista->inicio = nuevo;
		lista->cant++;
	}
	else if (actual == NULL)
	{
		insertarFinal(lista, dato);
	}
				else
	{
		Nodo *nuevo = crearNodo(dato);
		if (!nuevo) return;
		nuevo->sig = actual;
		anterior->sig = nuevo;
		lista->cant++;
	}
}

