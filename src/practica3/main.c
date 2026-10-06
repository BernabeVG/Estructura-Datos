#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostring.h>
#include <Pila.h>
#include <Cola.h>

void imprimir(void *);
char convertirMinuscula(char c);
void quitarEspacios(const char *origen, char *destino);
int esPalindromo(const char *cadena);
int validacionParentesis(Pila pila, char *cadena);

int main(void)
{
	int opc=0;
	Pila pila = inicializarPila(-1);
	//Cola cola = inicializarCola();
    char cadena[30];

	do {
		printf("\n---Menu---");
		printf("\n1. Palindromo.");
		printf("\n2. Verificacion de parentesis.");
		printf("\n3. Terminar ejeucion.");
		printf(("\nSeleccione una opcion: "));
		scanf("%d",&opc);

		while (getchar() != '\n');

		switch(opc) {
			case 1:
				inputCadena("\n Captura cadena: ",cadena,30);
				if (esPalindromo(cadena)) {
					printf("La cadena es un palindromo");

				}
				else {
					printf("La cadena no es un palindromo");
				}
				/*for(int i=0; cadena[i]!='\0' ; i++)
				{
					push(&pila, &cadena[i]);
					enqueue(&cola, &cadena[i]);
				}
				imprimirPila(pila,imprimir);
				eliminarPila(&pila,NULL);
				imprimirCola(cola,imprimir);
				imprimirCola(cola,imprimir);
				eliminarCola(&cola,NULL);*/
				break;
			case 2:
				inputCadena("\n Captura cadena: ",cadena,30);
				if (validacionParentesis(pila, cadena)) {
					printf("La cadena es valida");
				}
				else {
					printf("La cadena no es valida");
				}
				break;
			case 3:
				printf("\n FIN DE PROGRAMA \n\r");
				break;
			default:
				printf("Opcion invalida.intentelo de nuevo.");
				break;
		}
	}while (opc!=3);
	return 0;
}

void imprimir(void *dato)
{
	printf("%c",*(char*)dato);
}

char convertirMinuscula(char c)
{
	if(c>='A' && c<='Z')
	{
		return c + 32;
	}
	return c;
}

void quitarEspacios(const char *origen, char *destino) {
	int j=0;
	for (int i=0; origen[i] != '\0';i++) {
		if (origen[i] != ' ' && origen[i] != '\n') {
			destino[j] = convertirMinuscula(origen[i]);
			j++;
		}
	}
	destino[j] = '\0';
}

int esPalindromo(const char *cadena) {

	char auxCadena[100];
	quitarEspacios(cadena,auxCadena);

	if (!auxCadena[0]) return 0;

	Pila pila = inicializarPila(-1);
	int esPal= 1;

	for (int i=0; auxCadena[i] != '\0'; i++) { //rellenarPila
		push(&pila, &auxCadena[i]);
	}

	for (int i=0; auxCadena[i] != '\0'; i++) { //voltear cadena
		char *auxCmp = pop(&pila);

		if (*auxCmp != auxCadena[i]) {
			esPal= 0;
			break;
		}
	}
	eliminarPila(&pila,NULL);

	return esPal;
}
//validacion de caracteres que contienen parentesis,llaves y corchetes
int validacionParentesis(Pila pila, char *cadena) {

	if (cadena == NULL)
		return 0;

	char caracter;

	for (int i=0; cadena[i] != '\0'; i++)
	{
		if (cadena[i] == '('|| cadena[i] == '['|| cadena[i] ==  '{') {
			push(&pila, &cadena[i]);
		}
		if (cadena[i] == ')'|| cadena[i] == ']'|| cadena[i] == '}') {
			if (pilaVacia(pila)) {
				eliminarPila(&pila, NULL);
				return 0;
			}
			caracter = *(char *) pop(&pila);
			// Se verifica si NO coinciden como pareja opuesta
			if ((cadena[i] == ')' && caracter != '(') ||
				(cadena[i] == ']' && caracter != '[') ||
				(cadena[i] == '}' && caracter != '{')) {
				eliminarPila(&pila, NULL);
				return 0;
				}
		}
	}
	eliminarPila(&pila,NULL);
	return 1;
}