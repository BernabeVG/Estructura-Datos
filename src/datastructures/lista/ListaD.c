#include "ListaD.h"

ListaD inicializarListaD(void)
{
	return (ListaD){.inicio=NULL,.fin=NULL,.cant=0,.orden=NINGUNO,.comparar=NULL};
}
void insertarOrdenadoD(ListaD *lista,void *dato)
{
	if(!lista->inicio || lista->comparar(dato,lista->inicio->dato) < 0)
		insertarInicioD(lista,dato);
	else if( lista->comparar(dato,lista->fin->dato) > 0)
		insertarFinalD(lista,dato);
	else
	{
		Nodod *nuevo = crearNodod(dato);
		Nodod *q = lista->inicio;
		Nodod *p = q->sig;
		while( lista->comparar(dato,p->dato) > 0) 
		{
			q = p;
			p = p->sig;
		}
		q->sig = nuevo;
		nuevo->ant = q;
		nuevo->sig = p;
		p->ant = nuevo;
		lista->cant++;
	}	
}

void imprimirListaIFD(ListaD lista,fn_imprimir imprimir)
{
	if(!lista.inicio) return;
	Nodod *aux = lista.inicio;
	do
	{
		printf("\n");
		imprimir(aux->dato);
		aux = aux->sig;
	}while( aux && aux!= lista.inicio);
}
void imprimirListaFID(ListaD lista,fn_imprimir imprimir)
{
	if(!lista.inicio) return;
	Nodod *aux = lista.fin;
	do
	{
		printf("\n");
		imprimir(aux->dato);
		aux = aux->ant;
	}while( aux && aux!= lista.fin);
}

void eliminarListaD(ListaD *lista,fn_free liberar)
{
	if (!lista->inicio) {
		return;
	}
	// Romper enlace circular si el modo bucle estuviera activo
	if (lista->fin) {
		lista->fin->sig = NULL;
	}

	Nodod *aux;

	while (lista->inicio)
	{
		aux = lista->inicio;
		lista->inicio = lista->inicio->sig;

		if (liberar && aux->dato)
		{
			liberar(aux->dato);
		}
		free(aux);
	}
	lista->inicio = NULL;
	lista->fin = NULL;
	lista->cant = 0;

}

void insertarInicioD(ListaD *lista,void *dato)
{
	Nodod *nuevo = crearNodod(dato);
	if(!lista->inicio)
		lista->inicio = lista->fin = nuevo;
	else
	{
		nuevo->sig = lista->inicio;
		lista->inicio->ant = nuevo;
		lista->inicio = nuevo;
	}
	lista->cant++;
}

void insertarFinalD(ListaD *lista,void *dato)
{
	Nodod *nuevo = crearNodod(dato);
	if(!lista->inicio)
		lista->inicio = lista->fin = nuevo;
	else
	{
		nuevo->ant = lista->fin;
		lista->fin->sig = nuevo;
		lista->fin = nuevo;
	}
	lista->cant++;
	
}
void eliminarDatoD(ListaD *lista,void *dato,fn_comparar comparar)
{
	if (!lista->inicio)
	{
		return;
	}

	for (Nodod *elim = lista->inicio ; elim ;elim=elim->sig)
	{
		if (comparar(dato, elim->dato)==0)
		{
			if (lista->inicio == elim)
			{
				lista->inicio->ant = NULL;
			}
			else {
				lista->fin=NULL;
			}
		}
		else if (lista->fin==NULL)
		{
			lista->fin = elim->ant;
			lista->fin->sig = NULL;
		}
		else
		{
			elim->ant->sig = elim->sig;
			elim->sig->ant = elim->ant;
		}
		free(elim);
		lista->cant--;
		return;
	}

}
int buscarDatoD(ListaD lista,void *dato,fn_comparar comparar)
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
void ordenarListaD(ListaD *lista,fn_comparar comparar,Orden orden)
{
	lista->comparar = comparar;
	lista->orden = orden;

	if (orden == NINGUNO)
	{
		return;
	}
	for (Nodod *i = lista->inicio; i ;i = i->sig)
	{
		Nodod *cmp = i;
		for (Nodod *j = i->sig; j; j = j->sig)
		{
			if (orden == ASCENDENTE && comparar(cmp->dato,j->dato)>0)
			{
				cmp->ant =j;
			}
			else if (orden == DESCENDENTE && comparar(cmp->dato, j->dato)<0)
			{
				cmp->sig =j;
			}
			if (cmp!=i)
			{
				swapNodod(i,cmp);
			}
		}
	}
}

void convertirBucleD(ListaD *lista, int dato){

	if (!lista->inicio)
	{
		return;
	}
	//activar cola == 1 , desactivar cola == 0
	if (!dato)
	{
		//desactivar
		lista->fin->sig =NULL;
		lista->inicio->ant = NULL;
		printf("Modo Repeticion: DESACTIVADO");

	}
	if (dato) {
			//activar
			lista->fin->sig = lista->inicio;
			lista->inicio->ant = lista->fin;
			printf("Modo Repeticion: ACTIVADO");
	}
}
