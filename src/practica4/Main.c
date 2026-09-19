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
	liberarLaberinto(lab);
	eliminarPila(pila,free);
	free(pila);
    printf("\n\n FIN DE PROGRAMA\n\n");
    return 0;
}


Pila* resolverLab(Laberinto lab, Coordenada origen,Coordenada destino)
{
		
	Pila *pila = crearPila();
	*pila = inicializarPila(-1);	
	Coordenada *punto;
	//PUSH INICIAL DEL ORIGEN
	punto = crearCoordenada(origen.x,origen.y);
	push(pila,punto);	
	//CICLO
	
	while( !pilaVacia(*pila))
	{
		printf("\n Presione enter para continuar....");
		clear_buffer();
		imprimirLab(lab);		
		//punto = (Coordenada*)peek(*pila);
		//mov = movimientosPosibles(lab,*punto);		
		
	}
	return pila;
}


