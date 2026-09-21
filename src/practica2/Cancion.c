#include "Cancion.h"


Canciones cargarCanciones(void)
{
	static Cancion biblioteca[] = 
	{
		(Cancion){"Eres un Sueño", "Los Temerarios", 1993},
		(Cancion){"Como Te Recuerdo", "Los Temerarios", 1998},
		(Cancion){"Supermercado", "Mon Laferte", 2021},
		(Cancion){"Melancolia", "Mon Laferte", 2011},
		(Cancion){"Desvelado", "Bobby Pulido", 1995},
		(Cancion){"Ropa de Bazar", "Ed Maverick", 2019},
		(Cancion){"Fuentes de Ortiz", "Ed Maverick", 2018},
		(Cancion){"En un Solo Dia", "Morat", 2016},
		(Cancion){"Besos en Guerra", "Morat", 2017},
		(Cancion){"Amor Prohibido", "Selena", 1994},
		(Cancion){"Como la Flor", "Selena", 1992},
		(Cancion){"Mientras Duermes", "Junior H", 2023},
		(Cancion){"Paris", "Junior H", 2023},
		(Cancion){"Toda Esta Ciudad", "Kevin Kaarl", 2020},
		(Cancion){"Quedate", "Kevin Kaarl", 2019},
		(Cancion){"Si Supieras", "Kevin Kaarl", 2019},
		(Cancion){"Contenta", "Ed Maverick", 2021},
		(Cancion){"Piensa en Mi", "Grupo Mojado", 1996},
		(Cancion){"Diciembre", "Eslabon Armado", 2020},
		(Cancion){"Invitame un Cigarro", "Joan Sebastian", 1999},
		(Cancion){"Tatuajes", "Joan Sebastian", 1996},
		(Cancion){"Creep", "Radiohead", 1992},
		(Cancion){"Karma Police", "Radiohead", 1997},
		(Cancion){"Preso", "Jose Jose", 1981},
		(Cancion){"Almohada", "Jose Jose", 1978},
		(Cancion){"Propuesta Indecente", "Romeo Santos", 2013},
		(Cancion){"Mi Complice", "Cardenales de NL", 1997}
	};	
	static Canciones canciones;
	canciones.cancion = biblioteca;
	canciones.cant = sizeof(biblioteca)/sizeof(Cancion);
	return canciones;
}

void imprimirCanciones(Canciones canciones)
{
	printf("\n Canciones TOTAL:%zu",canciones.cant);
	for(int i = 0 ; i< canciones.cant; i++)
	{
		printf("\n[%d]",i);
		imprimirCancion(&canciones.cancion[i]);
	}
}

void imprimirCancion(void *dato)
{
	Cancion *cancion = dato;
	printf(" %-20s ",cancion->nombre);
	printf("| %-20s ",cancion->autor);
	printf("| %hu ",cancion->release_date);
}
void imprimirNombre(void *dato)
{
	Cancion *cancion = dato;
	printf(" %-20s ",cancion->nombre);	
}
Cancion *obtenerCancion(Canciones canciones, size_t pos)
{
	if (pos >= canciones.cant) {
		return  NULL;
	}
	return &canciones.cancion[pos];
}