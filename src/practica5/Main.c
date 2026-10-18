#include <stdio.h>
#include <stdlib.h>
#include <iostring.h>
#include <arbol.h>
#include <Nodo.h>

int *crearEntero(int);
void imprimir(void*);


int main(void)
{
	int opc;

	Nodoa *raiz = crearNodoa( crearEntero(4) );
	raiz->izq = crearNodoa(crearEntero(2) );
	raiz->izq->izq = crearNodoa(crearEntero(1) );
	raiz->izq->dch = crearNodoa(crearEntero(3) );
	raiz->dch =  crearNodoa(crearEntero(6) );	
	raiz->dch->izq =  crearNodoa(crearEntero(5) );
	raiz->dch->dch = crearNodoa(crearEntero(7) );
	Arbol arbol;
	arbol.raiz = raiz;

	do{
	//menu
	printf("\n---MENU---");
	printf("\n1. Insertar Arbol.");
	printf("\n2. Eliminar dato de un Arbol.");
	printf("\n3. Equilibrar el arbol.");
	printf("\n4. Mostrar arboles.");
	printf("\n5. Terminar Programa.");
	printf("\nSelecciona una opcion: ");
	scanf("%d", &opc);

	switch (opc) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
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
		case 5:
			printf("\n\n FIN DE PROGRAMA\n\n");
			break;
		default:
			printf("\n Opcion no valida");
			break;
	}
	}while(opc != 5);
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

