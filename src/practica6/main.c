#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostring.h>
#include <Pila.h>
#include <arbol.h>
#include <Nodo.h>

char* crearCaracter(char c);
int validacionParentesis(const char *cadena);
void imprimirCaracter(void *dato);
int esOperador(char c);
int esVariable(char c);
int precedencia(char op);
int validarExpresion(const char *expr);
void infixAPostfix(const char *infix, char *postfix);
Nodoa* postfixAArbol(const char *postfix);
void liberarArbol(Arbol *arbol);

int indiceVariable(char c);
void capturarVariablesInorden(Nodoa *raiz, double valores[], int asignado[]);
int evaluarArbolPostorden(Nodoa *raiz, const double valores[], double *resultado);

int main(void)
{
    int opc = 0;
    char expresion[100] = "";
    char postfix[100] = "";
    char ultimaValida[100] = "Ninguna";
    Arbol arbol = {NULL, 0, NINGUNO};

    do
    {
        printf("\nEXPRESION: %s", ultimaValida);
        printf("\n[0] CAPTURAR EXPRESION");
        printf("\n[1] IMPRIMIR ARBOL DE EXPRESION");
        printf("\n[2] EVALUAR EXPRESION");
        printf("\n[3] TERMINAR PROGRAMA");
        printf("\nSelecciona una opcion: ");
        scanf("%d", &opc);
        while (getchar() != '\n');

        switch (opc)
        {
            case 0:
                inputCadena("\nIngresa la expresion: ", expresion, 100);
                if (validarExpresion(expresion))
                {
                    liberarArbol(&arbol);
                    strcpy(ultimaValida, expresion);
                    infixAPostfix(expresion, postfix);
                    arbol.raiz = postfixAArbol(postfix);
                    printf("\nExpresion valida. Postfix: %s\n", postfix);
                }
                else
                {
                    printf("\nExpresion no valida.\n");
                }
                break;

            case 1:
                if (arbol.raiz != NULL)
                {
                    printf("\nArbol de Expresion: \n");
                    imprimirArbol(arbol, imprimirCaracter);
                }
                else
                {
                    printf("\nNo hay un arbol de expresion valido cargado.\n");
                }
                break;

            case 2:
                if (arbol.raiz != NULL)
                {
                    double valores[52] = {0.0};
                    int asignado[52] = {0};
                    double resultado = 0.0;

                    capturarVariablesInorden(arbol.raiz, valores, asignado);

                    if (evaluarArbolPostorden(arbol.raiz, valores, &resultado))
                    {
                        printf("\nResultado de la expresion: %.4lf\n", resultado);
                    }
                    else
                    {
                        printf("\nError matematico: division entre cero.\n");
                    }
                }
                else
                {
                    printf("\nNo hay un arbol de expresion valido cargado.\n");
                }
                break;

            case 3:
                liberarArbol(&arbol);
                printf("\nTerminando programa...\n");
                break;

            default:
                printf("\nOpcion no valida.\n");
                break;
        }
    } while (opc != 3);

    return 0;
}

char* crearCaracter(char c)
{
    char *dato = calloc(1, sizeof(char));
    *dato = c;
    return dato;
}

void imprimirCaracter(void *dato)
{
    printf("%c", *(char *)dato);
}

int esOperador(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int esVariable(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int precedencia(char op)
{
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

int validacionParentesis(const char *cadena)
{
    if (cadena == NULL || cadena[0] == '\0')
        return 0;
    Pila pila = inicializarPila(-1);
    char caracter;
    int cadenaValida = 1;

    for (int i = 0; cadena[i] != '\0'; i++)
    {
        if (cadena[i] == '(')
        {
            push(&pila, (void *)&cadena[i]);
        }
        else if (cadena[i] == ')')
        {
            if (pilaVacia(pila))
            {
                eliminarPila(&pila, NULL);
                return 0;
            }
            caracter = *(char *)pop(&pila);
            if (caracter != '(')
            {
                eliminarPila(&pila, NULL);
                return 0;
            }
        }
        else if (cadena[i] == '[' || cadena[i] == ']' || cadena[i] == '{' || cadena[i] == '}')
        {
            eliminarPila(&pila, NULL);
            return 0;
        }
    }
    if (!pilaVacia(pila))
    {
        cadenaValida = 0;
    }
    eliminarPila(&pila, NULL);
    return cadenaValida;
}

int validarExpresion(const char *expr)
{
    if (!validacionParentesis(expr))
        return 0;

    int estadoAnterior = 0;
    int tieneVariable = 0;

    for (int i = 0; expr[i] != '\0'; i++)
    {
        char c = expr[i];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') continue;

        if (esVariable(c))
        {
            if (estadoAnterior == 1) return 0;

            estadoAnterior = 1;
            tieneVariable = 1;
        }
        else if (esOperador(c))
        {
            if (estadoAnterior != 1) return 0;
            estadoAnterior = 2;
        }
        else if (c == '(')
        {
            if (estadoAnterior == 1) return 0;
            estadoAnterior = 0;
        }
        else if (c == ')')
        {
            if (estadoAnterior != 1) return 0;
            estadoAnterior = 1;
        }
        else
            return 0;
    }
    return (tieneVariable && estadoAnterior == 1);
}

void infixAPostfix(const char *infix, char *postfix)
{
    Pila pila = inicializarPila(-1);
    int j = 0;

    for (int i = 0; infix[i] != '\0'; i++)
    {
        char c = infix[i];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') continue;

        if (esVariable(c))
            postfix[j++] = c;
        else if (c == '(')
        {
            push(&pila, (void *)&infix[i]);
        }
        else if (c == ')')
        {
            while (!pilaVacia(pila) && *(char *)peek(pila) != '(')
            {
                postfix[j++] = *(char *)pop(&pila);
            }
            if (!pilaVacia(pila))
            {
                pop(&pila);
            }
        }
        else if (esOperador(c))
        {
            while (!pilaVacia(pila) && *(char *)peek(pila) != '(')
            {
                char tope = *(char *)peek(pila);
                if (precedencia(tope) > precedencia(c) ||
                    (precedencia(tope) == precedencia(c) && c != '^'))
                {
                    postfix[j++] = *(char *)pop(&pila);
                }
                else
                {
                    break;
                }
            }
            push(&pila, (void *)&infix[i]);
        }
    }
    while (!pilaVacia(pila))
    {
        postfix[j++] = *(char *)pop(&pila);
    }

    postfix[j] = '\0';
    eliminarPila(&pila, NULL);
}

Nodoa* postfixAArbol(const char *postfix)
{
    Pila pila = inicializarPila(-1);

    for (int i = 0; postfix[i] != '\0'; i++)
    {
        char c = postfix[i];
        Nodoa *nuevo = crearNodoa(crearCaracter(c));

        if (esOperador(c))
        {
            nuevo->dch = (Nodoa *)pop(&pila);
            nuevo->izq = (Nodoa *)pop(&pila);
        }
        push(&pila, nuevo);
    }
    Nodoa *raiz = NULL;
    if (!pilaVacia(pila))
    {
        raiz = (Nodoa *)pop(&pila);
    }

    eliminarPila(&pila, NULL);
    return raiz;
}

void liberarArbol(Arbol *arbol)
{
    if (!arbol || !arbol->raiz) return;

    Pila pila = inicializarPila(-1);
    push(&pila, arbol->raiz);

    while (!pilaVacia(pila))
    {
        Nodoa *actual = (Nodoa *)pop(&pila);

        if (actual->izq != NULL)
        {
            push(&pila, actual->izq);
        }
        if (actual->dch != NULL)
        {
            push(&pila, actual->dch);
        }
        free(actual->dato);
        free(actual);
    }
    arbol->raiz = NULL;
    arbol->cant = 0;
    eliminarPila(&pila, NULL);
}

int indiceVariable(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    if (c >= 'a' && c <= 'z')
        return (c - 'a') + 26;
    return -1;
}

void capturarVariablesInorden(Nodoa *raiz, double valores[], int asignado[])
{
    Pila pila = inicializarPila(-1);
    Nodoa *actual = raiz;

    while (actual != NULL || !pilaVacia(pila))
    {
        while (actual != NULL)
        {
            push(&pila, actual);
            actual = actual->izq;
        }

        actual = (Nodoa *)pop(&pila);
        char sim = *(char *)actual->dato;

        if (esVariable(sim))
        {
            int idx = indiceVariable(sim);
            if (idx != -1 && !asignado[idx])
            {
                printf("Ingresa el valor para '%c': ", sim);
                scanf("%lf", &valores[idx]);
                while (getchar() != '\n');
                asignado[idx] = 1;
            }
        }

        actual = actual->dch;
    }

    eliminarPila(&pila, NULL);
}

int evaluarArbolPostorden(Nodoa *raiz, const double valores[], double *resultado)
{
    Pila pilaNodos = inicializarPila(-1);
    Pila pilaOrden = inicializarPila(-1);
    Pila pilaEval = inicializarPila(-1);
    int valido = 1;

    push(&pilaNodos, raiz);
    while (!pilaVacia(pilaNodos))
    {
        Nodoa *nodo = (Nodoa *)pop(&pilaNodos);
        push(&pilaOrden, nodo);

        if (nodo->izq != NULL)
            push(&pilaNodos, nodo->izq);
        if (nodo->dch != NULL)
            push(&pilaNodos, nodo->dch);
    }

    while (!pilaVacia(pilaOrden) && valido)
    {
        Nodoa *nodo = (Nodoa *)pop(&pilaOrden);
        char sim = *(char *)nodo->dato;

        if (esVariable(sim))
        {
            int idx = indiceVariable(sim);
            double *val = malloc(sizeof(double));
            *val = valores[idx];
            push(&pilaEval, val);
        }
        else if (esOperador(sim))
        {
            double *pOp2 = (double *)pop(&pilaEval);
            double *pOp1 = (double *)pop(&pilaEval);
            double op2 = *pOp2;
            double op1 = *pOp1;
            free(pOp2);
            free(pOp1);

            double res = 0.0;
            switch (sim)
            {
                case '+': res = op1 + op2; break;
                case '-': res = op1 - op2; break;
                case '*': res = op1 * op2; break;
                case '/':
                    if (op2 == 0.0)
                    {
                        valido = 0;
                    }
                    else
                    {
                        res = op1 / op2;
                    }
                    break;
                case '^': res = pow(op1, op2); break;
            }

            if (valido)
            {
                double *pRes = malloc(sizeof(double));
                *pRes = res;
                push(&pilaEval, pRes);
            }
        }
    }

    if (valido && !pilaVacia(pilaEval))
    {
        double *pFinal = (double *)pop(&pilaEval);
        *resultado = *pFinal;
        free(pFinal);
    }

    eliminarPila(&pilaNodos, NULL);
    eliminarPila(&pilaOrden, NULL);
    eliminarPila(&pilaEval, free);
    return valido;
}