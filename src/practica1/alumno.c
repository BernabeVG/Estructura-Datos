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
	printf("%-8u | ",alumno.matricula);
	printf("%-20s | ",alumno.nombre);
	printf("%3d | ",alumno.semestre);
	printf("%.2f | ",alumno.promedio);
}

/*int compararMatricula(void *a, void *b)
{

	Lista alumno = inicializarLista();


	if ((size_t * )a == (size_t * )b) {
		strcmp(a,b);
		return 1;
	}
}
int compararNombre(void *a, void *b)
{
	strcmpIC(a,b);

int compararSemestre(void *a, void *b)
{

}
int compararCompararPromedio(void *a, void *b)
{

}
*/