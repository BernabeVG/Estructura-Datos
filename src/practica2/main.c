#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostring.h>
#include <ListaD.h>
#include "Cancion.h"


int main(void)
{
        ListaD lista = inicializarListaD();
		Canciones canciones = cargarCanciones();
		int idCancion=0;
        int activar=0;
        int option,corre_programa;
		corre_programa=1;
		Nodod *actual=NULL;

    while(corre_programa)
    {
        printf("\n\r");
        printf("\n\r");
        printf("\n\r [0] BUCLE");
        printf("\n\r [1] ANTERIOR");
        printf("\n\r [2] SIGUIENTE");
        printf("\n\r [3] AGREGAR CANCION");
        printf("\n\r [4] MOSTRAR LISTA DE REPRODUCCION");
        printf("\n\r [5] QUITAR CANCION");
        printf("\n\r [6] REINICIAR REPRODUCCION");
        printf("\n\r [7] TERMINAR PROGRAMA");

        printf("\n\r Escoge opcion: ");
        scanf("%d",&option);
		clear_buffer();
		switch(option)
                {
                        case 0:
                                activar = !activar;
								 convertirBucleD(&lista,activar);
                                break;
                        case 1:
								if (!actual && lista.inicio)
								{
									actual = lista.inicio;
								}
                                if (!actual)
                                {
                                        printf("\nNo hay canciones en la lista de reproduccion.");
                                }
		                else if (actual->ant != NULL)
		                {
		                        actual = actual->ant;
		                        printf("\nCancion anterior: ");
		                        imprimirCancion(actual->dato);
		                		printf("\n");
		                }
		                else
		                {
		                    printf("\nNo hay canciones anteriores.");
		                	printf("\nActual: ");
		                	imprimirCancion(actual->dato);
		                	printf("\n");
		                }
                                break;
                        case 2:
								if (!actual && lista.inicio)
								{
								actual = lista.inicio;
								}
                                if (!actual)
                                {
                                	printf("\nNo hay canciones en la lista de reproduccion.");
                                }
				                else if (actual->sig != NULL)
				                {
				                        actual = actual->sig;
				                		printf("\nCancion siguiente:" );
				                		imprimirCancion(actual->dato);
				                		printf("\n");
				                }
				                else
				                {
				                    printf("\nNo hay canciones siguientes.");
				                	printf("\nActual: ");
				                	imprimirCancion(actual->dato);
				                	printf("\n");
				                }
		                                break;
                        case 3:
                                //SE MUESTRA UN MENU CON LAS CANCIONES DISPONIBLES
								printf("---Canciones Dsiponibles---\n");
								imprimirCanciones(canciones);
                                //SE SELECCIONA LA CANCION
								printf("\n\nIngresa el iD de la cancion que deseas: ");
								scanf("%d",&idCancion);
								clear_buffer();
								//apuntador a estructura cancion
                                Cancion *cancion = obtenerCancion(canciones,(size_t)idCancion);
								//SE MUESTRA LA COLA DE REPRODUCCION
								printf("---Cola de Reproduccion---\n");
								if (!lista.inicio) {
									printf("La Lista de Reproduccion esta vacia.");
								}
								else {
									Nodod *aux = lista.inicio;
									for (size_t i=0; i< lista.cant && aux != NULL;i++) {
										printf("\n [%zu] ", i);
										imprimirCancion(aux->dato);
										aux = aux->sig;
									}
								}
                                //SE SELECCIONA EN QUE POSICION DE LA COLA SE AGREGARA
								if (lista.cant == 0) {
									insertarInicioD(&lista, cancion);
								}
								else {
									int cancionPos;
									printf("\n---En que posicion desea ingresar la cancion--- ");
									printf("\n1. Insertar al inicio.");
									printf("\n2. Inserir al final.");
									printf("\nSeleccione una opcion: ");
									scanf("%d",&cancionPos);
									if (cancionPos == 1) {
										insertarInicioD(&lista, cancion);
									}
									else if (cancionPos == 2) {
										insertarFinalD(&lista, cancion);
									}
								}
                                break;
                        case 4:

								if (!lista.inicio) {
									printf("Cola de reproduccion vacia.");
								}
								else {
									//SE MUESTRA LA COLA DE REPRODUCCION
									printf("---Cola de Reproduccion---\n");
									Nodod *aux = lista.inicio;
									for (size_t  i=0; i<lista.cant &&aux != NULL;i++) {
										printf("\n [%zu] ", i);
										imprimirCancion(aux->dato);
										aux= aux->sig;
									}
								}
                                break;
                        case 5:
                                //SE MUESTRA UN MENU CON LAS CANCIONES DE LA COLA DE REPRODUCCION
                                //SE SELECCIONA UNA OPCION Y SE ELIMINA
                                break;
                        case 6:
                        //LA POSICION ACTUAL CAMBIA AL INICIO DE LA COLA DE REPRODUCCION
                        break;
                        case 7:
                                corre_programa=0;
                                break;
                        default:
                                break;
                }
            printf("\n\nENTER PARA CONTINUAR...");
            clear_buffer();
    }
    printf("\n FIN DE PROGRAMA \n\r");
    return 0;
}