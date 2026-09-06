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
	do{
		printf("\n    ---MENU---");
		printf("\n1. Registrar Alumno");
		printf("\n2. Desplegar Alumnos");
		printf("\n3. Reordenar");
		printf("\n4. Buscar Alumno");
		printf("\n5. Borrar alumno");
		printf("\n6. Salir del programa");
		printf("\nSeleccione una opcion: ");
		scanf("%d", &opc);

		switch (opc) {
			case 1:
			{
				size_t mat = 0;
				printf("\nIngresa la matricula: ");
				scanf("%u", (unsigned int*)&mat);
				insertarFinal(&lista, crearAlumno(mat));
				break;
			}

			case 2:
				imprimirLista(lista,imprimirAlumno);
				break;
			case 3:
				printf("\nOrdenacion de alumno");
				printf("\n1. Nombre");
				printf("\n2. Matricula");
				printf("\n3. Semestre");
				printf("\n4. Promedio");
				printf("\nSeleccione una opcion: ");
				scanf("%d", &opc);
				ordenarAlumno(&lista, &opc);
				break;
			case 4:
				printf("\nBuscar alumno por?");
				printf("\n1. Nombre");
				printf("\n2. Matricula");
				printf("\n3. Semestre");
				printf("\n4. Promedio");
				printf("\nSeleccione una opcion: ");
				scanf("%d", &opc);
				buscarAlumno(&lista, &opc);
				break;

			case 5:
				printf("\nBorrar alumno por?");
				printf("\n1. Nombre");
				printf("\n2. Matricula");
				printf("\n3. Semestre");
				printf("\n4. Promedio");
				printf("\nSeleccione una opcion: ");
				scanf("%d", &opc);
				borrarAlumno(&lista, &opc);
				break;
			case 6:
				printf("\nSaliendo del programa...");
				break;
			default:
				printf("\nOpcion invalida. intentelo de nuevo.");
				break;
		}
	}while (opc!=6);
	return 0;
}