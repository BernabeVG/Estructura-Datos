#include <stdio.h>
#include <stdlib.h>
#include <Pila.h>
#include <iostring.h>
#include "Laberinto.h"



Pila* resolverLab(Laberinto lab, Coordenada origen,Coordenada destino);

int main(void)
{
    Laberinto lab = crear_laberinto();
	//Movimientos mov;
	Coordenada origen = {1,1}; 
	Coordenada destino = {25,24};
	unsigned int nuevasCoor = 0;	
	inputEnteroSinSigno("\n Nuevas coordenadas? (1:SI 0:NO) : ",&nuevasCoor);
	if(nuevasCoor)
	{
		inputEntero("\n Captura Origen X: ",&origen.x);
		inputEntero(" Captura Origen Y: ",&origen.y);
		inputEntero("\n Captura Destino X: ",&destino.x);
		inputEntero(" Captura Destino Y: ",&destino.y);
	}
	setOrigen(lab,origen);
	setDestino(lab,destino);	
	
	Pila *pila = resolverLab(lab,origen,destino);

	imprimirLab(lab);

	if (!pilaVacia(*pila))
	{
		printf("\n\n Caminon encontrado (A -> B):\n");
		Pila rutaOrdenada = inicializarPila(-1);

		//invertir pila para que el origen A quede en la cima
		while (!pilaVacia(*pila)) {
			push(&rutaOrdenada, pop(pila));
		}

		imprimirPila(rutaOrdenada, imprimirCoordenada);

		//devolvemos los punteros a la pila para la liberacion de memoria
		while (!pilaVacia(rutaOrdenada)) {
			push(pila,pop(&rutaOrdenada));
		}
	}
	else {
		printf("\n\n No existe solucion para llegar de A a B.\n");
	}

	liberarLaberinto(lab);
	eliminarPila(pila,free);
	free(pila);
    printf("\n\n FIN DE PROGRAMA\n\n");
    return 0;
}


Pila* resolverLab(Laberinto lab, Coordenada origen, Coordenada destino)
{
    Pila *pila = crearPila();
    *pila = inicializarPila(-1);
    Coordenada *punto;
    Movimientos mov;

    // 1. PUSH INICIAL DEL ORIGEN
    punto = crearCoordenada(origen.x, origen.y);
    push(pila, punto);

    //CICLO
    while (!pilaVacia(*pila))
    {
        // printf("\n Presione enter para continuar....");
        // clear_buffer();
        // imprimirLab(lab);

        // Obtener la coordenada actual (cima de la pila)
        punto = (Coordenada*)peek(*pila);

        //verifica si el destino b es adyacente
        if (lab[punto->x - 1][punto->y] == 'B' ||
            lab[punto->x + 1][punto->y] == 'B' ||
            lab[punto->x][punto->y + 1] == 'B' ||
            lab[punto->x][punto->y - 1] == 'B')
        {
            push(pila, crearCoordenada(destino.x, destino.y));
            break;
        }

        // evaluar movimientos disponibles('*')
        mov = movimientosPosibles(lab, *punto);

        if (bloqueado(mov))
        {
            // Callejon sin salida: marcar con 'o' (sin sobrescribir 'A') y retroceder
            if (lab[punto->x][punto->y] != 'A')
            {
                lab[punto->x][punto->y] = 'o';
            }

            Coordenada *descartada = (Coordenada*)pop(pila);
            free(descartada);
        }
        else
        {
            // Elegir la siguiente casilla libre según prioridad
            int sigX = punto->x;
            int sigY = punto->y;

            if (mov.right == 1)
            {
                sigY = sigY + 1;
            }
            else if (mov.left == 1)
            {
                sigY = sigY - 1;
            }
            else if (mov.down == 1)
            {
                sigX = sigX + 1;
            }
            else if (mov.up == 1)
            {
                sigX = sigX - 1;
            }

            // Marcar avance en el mapa con '.' y apilar nueva coordenada
            lab[sigX][sigY] = '.';
            push(pila, crearCoordenada(sigX, sigY));
        }
    }

    return pila;
}


