#include "alumno.h"
#include <stdlib.h>

#include "../datastructures/lista/Lista.h"
#include "../iostring/iostring.h"


Alumno *crearAlumno(size_t matricula)
{
	Alumno *nuevo = calloc(1,sizeof(Alumno));
	nuevo->matricula = matricula;
	clear_buffer();
	inputCadena("Captura cadena: ",nuevo->nombre,MAX);
	inputEntero("Captura semestre: ",&nuevo->semestre);
	inputFloat("Captura promedio: ",&nuevo->promedio);
	return nuevo;
}

void imprimirAlumno(void *a)
{	
	Alumno alumno = *(Alumno*)a;
	printf("%-8zu | ",alumno.matricula);
	printf("%-20s | ",alumno.nombre);
	printf("%3d | ",alumno.semestre);
	printf("%.2f | ",alumno.promedio);
}

int compararMatricula(void *a, void *b) {

	const Alumno *alumnoA = (const Alumno *)a;
	const Alumno *alumnoB = (const Alumno *)b;
	if (alumnoA->matricula > alumnoB->matricula) {
		return 1;
	}
	if (alumnoA->matricula < alumnoB->matricula) {
		return -1;
	}
	return 0; // si es son iguales retorna 0
}
int compararNombre(void *a, void *b) {
	Alumno alumnoA = *(Alumno*)a;
	Alumno alumnoB = *(Alumno*)b;

	return strcmpIC(alumnoA.nombre,alumnoB.nombre);
}
int compararSemestre(void *a, void *b) {
	Alumno alumnoA = *(Alumno*)a;
	Alumno alumnoB = *(Alumno*)b;

	if (alumnoA.semestre > alumnoB.semestre) {
		return 1;
	}
	if (alumnoA.semestre < alumnoB.semestre) {
		return -1;
	}
	return 0; //retorna 0 en caso de ser iguales
}
int compararCompararPromedio(void *a, void *b) {
	Alumno alumnoA = *(Alumno*)a;
	Alumno alumnoB = *(Alumno*)b;

	if (alumnoA.promedio > alumnoB.promedio) {
		return 1;
	}
	if (alumnoA.promedio < alumnoB.promedio) {
		return -1;
	}
	return 0; //retorna en caso de ser iguales
}
void ordenarAlumno(void *a, void *b)
{
	Lista lista = *(Lista*)a;
	int opcion = *(int*)b;

	if (!lista.inicio)
	{
		return;
	}

	if (opcion == 1)
	{ //Ordenar por nombre
		ordenarLista(&lista, compararNombre, ASCENDENTE);
	}
	if (opcion ==2 )
	{ //ordenar por matricula
		ordenarLista(&lista, compararMatricula, ASCENDENTE);
	}
	if (opcion == 3)
	{
		//ordenar por semestre
		ordenarLista(&lista, compararSemestre, ASCENDENTE);
	}
	if (opcion == 4)
	{ // Ordenar por promedio
			ordenarLista(&lista, compararCompararPromedio, ASCENDENTE);
	}
}
