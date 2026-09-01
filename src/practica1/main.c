#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostring.h>
#include <Lista.h>
#include "alumno.h"
#include "../datastructures/lista/Lista.h"



int main(void)
{

	Lista lista  = inicializarLista();
	/*int i =3;
	while(i--)
	{
		Alumno = temp;
		size_t mat = 0;
		printf("\n Captura matricula: ");
		scanf("%u",&mat);
		clear_buffer();
		Alumno *nuevo = crearAlumno(mat);
		insertarFinal(&lista,nuevo);
		imprimirLista(lista,imprimirAlumno);
		compararMatricula(&lista,mat);
		compararNombre(&lista,temp->nombre);
	}
	eliminarLista(&lista,free);*/

	int opc;

	printf("    ---MENU---");
	printf("\n1. Registrar Alumno");
	printf("\n2. Desplegar Alumnos");
	printf("\n3. Reordenar");
	printf("\n4. Buscar Alumno");
	printf("\n5. Salir del programa");
	printf("\nSeleccioe una opcion: ");
	scanf("%d", &opc);

	switch (opc) {
		case 1: {
			size_t mat = 0;
			printf("\nIngresa la matricula: ");
			scanf("%u", (unsigned int*)&mat);
			insertarFinal(&lista, crearAlumno(mat));
			break;
		}

		case 2:
			break;

		case 3:
			break;

		case 4:
			break;

		case 5:
			break;
	}
	return 0;
}
