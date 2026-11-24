#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <iostring.h>
#include <HashQ.h>
#include <arbol.h>
#include "Libro.h"

#define MAX_ENTRADA 100
#define TAM_INICIAL 83

typedef struct
{
	const char *nombre;
	HashQTable hq;
	size_t remapeos;
} TablaMotor;

typedef struct
{
	TablaMotor isbn;
	TablaMotor titulo;
	TablaMotor autor;
	TablaMotor fecha;
	TablaMotor inicialTitulo;
	TablaMotor inicialAutor;
} Motor;

typedef struct
{
	Arbol abb;
	fn_comparar orden;
} ArbolLibros;

#define NUM_TABLAS 6
#define TABLAS(m) { &(m)->isbn, &(m)->titulo, &(m)->autor, &(m)->fecha, &(m)->inicialTitulo, &(m)->inicialAutor }

int foldingISBN(void *data);
int foldingTitulo(void *data);
int foldingAutor(void *data);
int claveFecha(void *data);
int claveInicialTitulo(void *data);
int claveInicialAutor(void *data);

int mismoISBN(void *a, void *b);
int mismoTitulo(void *a, void *b);
int mismoAutor(void *a, void *b);
int mismaFecha(void *a, void *b);
int mismaInicialTitulo(void *a, void *b);
int mismaInicialAutor(void *a, void *b);

int matchTitulo(Libro *a, Libro *b);
int matchAutor(Libro *a, Libro *b);
int matchFecha(Libro *a, Libro *b);
int matchInicialTitulo(Libro *a, Libro *b);
int matchInicialAutor(Libro *a, Libro *b);

int ordenTitulo(void *a, void *b);
int ordenFechaTitulo(void *a, void *b);
int ordenAutorFechaTitulo(void *a, void *b);

void crearTabla(TablaMotor *t, const char *nombre, fn_folding folding, fn_comparar comparar);
void verificarRemapeo(TablaMotor *t);
void insertarTabla(TablaMotor *t, Libro *libro);
Libro* buscarTabla(TablaMotor *t, Libro *clave);
void buscarTodos(TablaMotor *t, Libro *clave, int (*coincide)(Libro*, Libro*), ArbolLibros *res);
void eliminarDeTabla(TablaMotor *t, Libro *libro);
void liberarTabla(TablaMotor *t);
double factorCarga(TablaMotor *t);

ArbolLibros crearArbolLibros(fn_comparar orden);
void insertarArbolLibros(ArbolLibros *ar, Libro *libro);
void imprimirArbolLibros(ArbolLibros *ar);
void liberarArbolLibros(ArbolLibros *ar);

void crearMotor(Motor *m, Acervo acervo);
void liberarMotor(Motor *m);
void menuTitulo(Motor *m);
void menuAutor(Motor *m);
void menuFecha(Motor *m);
void menuEliminar(Motor *m);
void buscarISBN(Motor *m);
void mostrarAcervo(Motor *m);
void mostrarTablas(Motor *m);

int leerTexto(const char *mensaje, char *buffer, size_t tam);
int leerInicial(const char *mensaje, char *inicial);
int icmp(const char *a, const char *b);
void imprimirLibroSalto(void *vlibro);


int main(void)
{
	Motor motor;
	Acervo acervo = obtener_libros();
	crearMotor(&motor, acervo);

	int opcion, corre = 1;
	while(corre)
	{
		printf("\n\n ------- MOTOR DE BUSQUEDA - ACERVO BIBLIOGRAFICO -------");
		printf("\n Libros en el acervo: %llu", (unsigned long long)motor.isbn.hq.cant);
		printf("\n [1] Buscar por ISBN");
		printf("\n [2] Buscar por titulo");
		printf("\n [3] Buscar por autor");
		printf("\n [4] Buscar por fecha de publicacion");
		printf("\n [5] Mostrar todos los libros");
		printf("\n [6] Eliminar libro / estado de las tablas hash");
		printf("\n [0] Terminar programa");
		printf("\n Selecciona una opcion: ");
		fflush(stdout);
		if(!inputEntero("", &opcion))
			opcion = -1;

		switch(opcion)
		{
			case 1: buscarISBN(&motor); break;
			case 2: menuTitulo(&motor); break;
			case 3: menuAutor(&motor); break;
			case 4: menuFecha(&motor); break;
			case 5: mostrarAcervo(&motor); break;
			case 6: menuEliminar(&motor); break;
			case 0: corre = 0; break;
			default: printf("\n Opcion invalida");
		}
	}

	liberarMotor(&motor);
	printf("\n\n FIN DE PROGRAMA\n");
	return 0;
}

void crearTabla(TablaMotor *t, const char *nombre, fn_folding folding, fn_comparar comparar)
{
	t->nombre = nombre;
	t->remapeos = 0;
	t->hq.table = calloc(TAM_INICIAL, sizeof(void*));
	t->hq.len = TAM_INICIAL;
	t->hq.cant = 0;
	t->hq.imprimir = imprimirLibro;
	t->hq.folding = folding;
	t->hq.comparar = comparar;
	t->hq.verbose = 0;
	t->hq.min = 30;
	t->hq.max = 60;
	t->hq.fixed = 60;
}

void verificarRemapeo(TablaMotor *t)
{
	if(t->hq.cant < 5)
		return;
	unsigned int lenAntes = t->hq.len;
	remap(&t->hq);
	if(t->hq.len != lenAntes)
		t->remapeos++;
}

void insertarTabla(TablaMotor *t, Libro *libro)
{
	fn_insertKey(&t->hq, libro);
}

Libro* buscarTabla(TablaMotor *t, Libro *clave)
{
	return (Libro*)fn_searchKey(&t->hq, clave);
}

void buscarTodos(TablaMotor *t, Libro *clave, int (*coincide)(Libro*, Libro*), ArbolLibros *res)
{
	int key = t->hq.folding(clave);
	int index = fn_hashQ(t->hq, key);

	for(unsigned int k = 0; k < t->hq.len; k++)
	{
		int pos = (k == 0) ? index : fn_rehashQ(t->hq, index, (int)k);
		void *celda = t->hq.table[pos];
		if(celda == NULL)
			break;
		if(celda != &t->hq && coincide((Libro*)celda, clave))
			insertarArbolLibros(res, (Libro*)celda);
	}
}

void eliminarDeTabla(TablaMotor *t, Libro *libro)
{
	if(fn_deleteKey(&t->hq, libro) != NULL)
		verificarRemapeo(t);
}

void liberarTabla(TablaMotor *t)
{
	free(t->hq.table);
	t->hq.table = NULL;
	t->hq.cant = 0;
	t->hq.len = 0;
}

double factorCarga(TablaMotor *t)
{
	if(t->hq.len == 0) return 0.0;
	return (double)t->hq.cant * 100.0 / (double)t->hq.len;
}

ArbolLibros crearArbolLibros(fn_comparar orden)
{
	ArbolLibros ar;
	ar.abb.raiz = NULL;
	ar.abb.cant = 0;
	ar.abb.orden = ORDEN;
	ar.orden = orden;
	return ar;
}

Nodoa* insertarNodoRec(Nodoa *raiz, void *dato, fn_comparar orden)
{
	if(!raiz)
		return crearNodoa(dato);
	if(orden(dato, raiz->dato) < 0)
		raiz->izq = insertarNodoRec(raiz->izq, dato, orden);
	else
		raiz->dch = insertarNodoRec(raiz->dch, dato, orden);
	return raiz;
}

void insertarArbolLibros(ArbolLibros *ar, Libro *libro)
{
	ar->abb.raiz = insertarNodoRec(ar->abb.raiz, libro, ar->orden);
	ar->abb.cant++;
}

void imprimirLibroSalto(void *vlibro)
{
	printf("\n ");
	imprimirLibro(vlibro);
}

void imprimirArbolLibros(ArbolLibros *ar)
{
	if(ar->abb.cant == 0)
	{
		printf("\n No se encontraron resultados.");
		return;
	}
	printf("\n Total encontrados: %zu", ar->abb.cant);
	imprimirOrden(ar->abb, imprimirLibroSalto, ORDEN);
}

void liberarNodosRec(Nodoa *raiz)
{
	if(!raiz) return;
	liberarNodosRec(raiz->izq);
	liberarNodosRec(raiz->dch);
	free(raiz);
}

void liberarArbolLibros(ArbolLibros *ar)
{
	liberarNodosRec(ar->abb.raiz);
	ar->abb.raiz = NULL;
	ar->abb.cant = 0;
}

void crearMotor(Motor *m, Acervo acervo)
{
	crearTabla(&m->isbn,          "ISBN",             foldingISBN,        mismoISBN);
	crearTabla(&m->titulo,        "Titulo",           foldingTitulo,      mismoTitulo);
	crearTabla(&m->autor,         "Autor",            foldingAutor,       mismoAutor);
	crearTabla(&m->fecha,         "Fecha",            claveFecha,         mismaFecha);
	crearTabla(&m->inicialTitulo, "Inicial titulo",   claveInicialTitulo, mismaInicialTitulo);
	crearTabla(&m->inicialAutor,  "Inicial autor",    claveInicialAutor,  mismaInicialAutor);

	TablaMotor *tablas[NUM_TABLAS] = TABLAS(m);
	for(size_t i = 0; i < acervo.cantidad; i++)
		for(int t = 0; t < NUM_TABLAS; t++)
			insertarTabla(tablas[t], &acervo.libros[i]);

	for(int t = 0; t < NUM_TABLAS; t++)
		verificarRemapeo(tablas[t]);
}

void liberarMotor(Motor *m)
{
	TablaMotor *tablas[NUM_TABLAS] = TABLAS(m);
	for(int t = 0; t < NUM_TABLAS; t++)
		liberarTabla(tablas[t]);
}

void buscarISBN(Motor *m)
{
	Libro clave = {0};
	if(!leerTexto("\n ISBN: ", clave.isbn, sizeof(clave.isbn)))
		return;
	Libro *libro = buscarTabla(&m->isbn, &clave);
	if(libro)
	{
		printf("\n LIBRO ENCONTRADO:\n ");
		imprimirLibro(libro);
	}
	else
		printf("\n No existe un libro con ISBN %s", clave.isbn);
}

void menuTitulo(Motor *m)
{
	int opcion;
	printf("\n --- Buscar por titulo ---");
	printf("\n [1] Por titulo");
	printf("\n [2] Por rango de iniciales");
	printf("\n Escoge opcion: ");
	fflush(stdout);
	if(!inputEntero("", &opcion))
		return;

	ArbolLibros resultados = crearArbolLibros(ordenTitulo);
	Libro clave = {0};

	if(opcion == 1)
	{
		if(!leerTexto("\n Titulo: ", clave.titulo, sizeof(clave.titulo)))
			return;
		buscarTodos(&m->titulo, &clave, matchTitulo, &resultados);
	}
	else if(opcion == 2)
	{
		char inf, sup;
		if(!leerInicial("\n Inicial inferior: ", &inf) || !leerInicial(" Inicial superior: ", &sup))
			return;
		if(inf > sup) { char aux = inf; inf = sup; sup = aux; }
		printf("\n Titulos de la '%c' a la '%c', ordenados por titulo:", inf, sup);
		for(int c = inf; c <= sup; c++)
		{
			clave.titulo[0] = (char)c;
			buscarTodos(&m->inicialTitulo, &clave, matchInicialTitulo, &resultados);
		}
	}
	else
	{
		printf("\n Opcion invalida");
		return;
	}
	imprimirArbolLibros(&resultados);
	liberarArbolLibros(&resultados);
}

void menuAutor(Motor *m)
{
	int opcion;
	printf("\n --- Buscar por autor ---");
	printf("\n [1] Por nombre del autor");
	printf("\n [2] Por rango de iniciales");
	printf("\n Escoge opcion: ");
	fflush(stdout);
	if(!inputEntero("", &opcion))
		return;

	ArbolLibros resultados;
	Libro clave = {0};

	if(opcion == 1)
	{
		if(!leerTexto("\n Autor: ", clave.autor, sizeof(clave.autor)))
			return;
		resultados = crearArbolLibros(ordenFechaTitulo);
		printf("\n Libros de %s, ordenados por fecha:", clave.autor);
		buscarTodos(&m->autor, &clave, matchAutor, &resultados);
	}
	else if(opcion == 2)
	{
		char inf, sup;
		if(!leerInicial("\n Inicial inferior: ", &inf) || !leerInicial(" Inicial superior: ", &sup))
			return;
		if(inf > sup) { char aux = inf; inf = sup; sup = aux; }
		resultados = crearArbolLibros(ordenAutorFechaTitulo);
		printf("\n Autores de la '%c' a la '%c', ordenados por autor:", inf, sup);
		for(int c = inf; c <= sup; c++)
		{
			clave.autor[0] = (char)c;
			buscarTodos(&m->inicialAutor, &clave, matchInicialAutor, &resultados);
		}
	}
	else
	{
		printf("\n Opcion invalida");
		return;
	}
	imprimirArbolLibros(&resultados);
	liberarArbolLibros(&resultados);
}

void menuFecha(Motor *m)
{
	int opcion;
	printf("\n --- Buscar por fecha de publicacion ---");
	printf("\n [1] Por anio");
	printf("\n [2] Por rango de anios");
	printf("\n Escoge opcion: ");
	fflush(stdout);
	if(!inputEntero("", &opcion))
		return;

	ArbolLibros resultados;
	Libro clave = {0};

	if(opcion == 1)
	{
		printf("\n Anio: ");
		fflush(stdout);
		if(!inputEntero("", &clave.fecha))
		{
			printf("\n Anio invalido");
			return;
		}
		resultados = crearArbolLibros(ordenTitulo);
		printf("\n Libros publicados en %d, ordenados por titulo:", clave.fecha);
		buscarTodos(&m->fecha, &clave, matchFecha, &resultados);
	}
	else if(opcion == 2)
	{
		int inf, sup;
		printf("\n Anio inferior: ");
		fflush(stdout);
		if(!inputEntero("", &inf))
		{
			printf("\n Anio invalido");
			return;
		}
		printf(" Anio superior: ");
		fflush(stdout);
		if(!inputEntero("", &sup))
		{
			printf("\n Anio invalido");
			return;
		}
		if(inf > sup) { int aux = inf; inf = sup; sup = aux; }
		resultados = crearArbolLibros(ordenFechaTitulo);
		printf("\n Libros de %d a %d, ordenados por fecha y titulo:", inf, sup);
		for(int anio = inf; anio <= sup; anio++)
		{
			clave.fecha = anio;
			buscarTodos(&m->fecha, &clave, matchFecha, &resultados);
		}
	}
	else
	{
		printf("\n Opcion invalida");
		return;
	}
	imprimirArbolLibros(&resultados);
	liberarArbolLibros(&resultados);
}

void mostrarAcervo(Motor *m)
{
	ArbolLibros todos = crearArbolLibros(ordenTitulo);
	for(unsigned int i = 0; i < m->isbn.hq.len; i++)
	{
		void *celda = m->isbn.hq.table[i];
		if(celda != NULL && celda != &m->isbn.hq)
			insertarArbolLibros(&todos, (Libro*)celda);
	}
	printf("\n Acervo completo, ordenado por titulo:");
	imprimirArbolLibros(&todos);
	liberarArbolLibros(&todos);
}

void menuEliminar(Motor *m)
{
	int opcion = -1;
	while(opcion != 0)
	{
		printf("\n\n --- Administracion del acervo ---");
		printf("\n [1] Eliminar libro por ISBN");
		printf("\n [2] Mostrar estado de las tablas hash");
		printf("\n [0] Regresar");
		printf("\n Escoge opcion: ");
		fflush(stdout);
		if(!inputEntero("", &opcion))
			opcion = -1;

		if(opcion == 1)
		{
			Libro clave = {0};
			if(!leerTexto("\n ISBN del libro a eliminar: ", clave.isbn, sizeof(clave.isbn)))
				continue;
			Libro *libro = buscarTabla(&m->isbn, &clave);
			if(!libro)
			{
				printf("\n No existe un libro con ISBN %s", clave.isbn);
				continue;
			}
			printf("\n ");
			imprimirLibro(libro);
			char resp[MAX_ENTRADA];
			if(!leerTexto("\n Eliminar este libro? (s/n): ", resp, sizeof(resp)) || tolower((unsigned char)resp[0]) != 's')
			{
				printf("\n Cancelado");
				continue;
			}
			TablaMotor *tablas[NUM_TABLAS] = TABLAS(m);
			for(int t = 0; t < NUM_TABLAS; t++)
				eliminarDeTabla(tablas[t], libro);
			printf("\n Libro eliminado de las %d tablas hash", NUM_TABLAS);
			mostrarTablas(m);
		}
		else if(opcion == 2)
			mostrarTablas(m);
		else if(opcion != 0)
			printf("\n Opcion invalida");
	}
}

void mostrarTablas(Motor *m)
{
	TablaMotor *tablas[NUM_TABLAS] = TABLAS(m);
	printf("\n\n %-16s %8s %8s %10s %9s", "TABLA", "CLAVES", "TAMANIO", "F. CARGA", "REMAPEOS");
	for(int t = 0; t < NUM_TABLAS; t++)
		printf("\n %-16s %8llu %8llu %9.2f%% %9llu", tablas[t]->nombre,
			(unsigned long long)tablas[t]->hq.cant, (unsigned long long)tablas[t]->hq.len,
			factorCarga(tablas[t]), (unsigned long long)tablas[t]->remapeos);
}

int foldingISBN(void *data)
{
	Libro *l = (Libro*)data;
	unsigned int suma = 0, bloque = 0;
	int digitos = 0;
	for(const char *c = l->isbn; *c; c++)
	{
		int d;
		if(isdigit((unsigned char)*c))
			d = *c - '0';
		else if(toupper((unsigned char)*c) == 'X')
			d = 10;
		else
			continue;
		bloque = bloque * 10 + d;
		if(++digitos == 3)
		{
			suma += bloque;
			bloque = 0;
			digitos = 0;
		}
	}
	return (int)((suma + bloque) & 0x7FFFFFFF);
}

int foldingTitulo(void *data)
{
	Libro *l = (Libro*)data;
	unsigned int clave = 0, bloque = 0, sector = 1;
	int n = 0;
	for(const char *c = l->titulo; *c; c++)
	{
		bloque = bloque * 256 + (unsigned char)tolower((unsigned char)*c);
		if(++n == 3)
		{
			clave += bloque * sector++;
			bloque = 0;
			n = 0;
		}
	}
	return (int)((clave + bloque * sector) & 0x7FFFFFFF);
}

int foldingAutor(void *data)
{
	Libro *l = (Libro*)data;
	unsigned int clave = 0;
	const char *s = l->autor;
	size_t len = strlen(s);
	for(size_t i = 0, sector = 0; i < len; i += 3, sector++)
	{
		unsigned int bloque = 0;
		size_t fin = (i + 3 < len) ? i + 3 : len;
		if(sector % 2 == 0)
			for(size_t j = i; j < fin; j++)
				bloque = bloque * 256 + (unsigned char)tolower((unsigned char)s[j]);
		else
			for(size_t j = fin; j-- > i; )
				bloque = bloque * 256 + (unsigned char)tolower((unsigned char)s[j]);
		clave += bloque;
	}
	return (int)(clave & 0x7FFFFFFF);
}

int claveFecha(void *data)
{
	return ((Libro*)data)->fecha;
}

int claveInicialTitulo(void *data)
{
	return (unsigned char)toupper((unsigned char)((Libro*)data)->titulo[0]);
}

int claveInicialAutor(void *data)
{
	return (unsigned char)toupper((unsigned char)((Libro*)data)->autor[0]);
}

int mismoISBN(void *a, void *b)
{
	const char *p = ((Libro*)a)->isbn, *q = ((Libro*)b)->isbn;
	while(1)
	{
		while(*p && !isalnum((unsigned char)*p)) p++;
		while(*q && !isalnum((unsigned char)*q)) q++;
		if(!*p || !*q)
			return (*p == '\0' && *q == '\0') ? 0 : 1;
		if(toupper((unsigned char)*p) != toupper((unsigned char)*q))
			return 1;
		p++;
		q++;
	}
}

int mismoTitulo(void *a, void *b)
{
	int r = icmp(((Libro*)a)->titulo, ((Libro*)b)->titulo);
	return r ? r : strcmp(((Libro*)a)->isbn, ((Libro*)b)->isbn);
}

int mismoAutor(void *a, void *b)
{
	int r = icmp(((Libro*)a)->autor, ((Libro*)b)->autor);
	return r ? r : strcmp(((Libro*)a)->isbn, ((Libro*)b)->isbn);
}

int mismaFecha(void *a, void *b)
{
	int fa = ((Libro*)a)->fecha, fb = ((Libro*)b)->fecha;
	if(fa != fb) return (fa < fb) ? -1 : 1;
	return strcmp(((Libro*)a)->isbn, ((Libro*)b)->isbn);
}

int mismaInicialTitulo(void *a, void *b)
{
	int ia = claveInicialTitulo(a), ib = claveInicialTitulo(b);
	if(ia != ib) return (ia < ib) ? -1 : 1;
	return strcmp(((Libro*)a)->isbn, ((Libro*)b)->isbn);
}

int mismaInicialAutor(void *a, void *b)
{
	int ia = claveInicialAutor(a), ib = claveInicialAutor(b);
	if(ia != ib) return (ia < ib) ? -1 : 1;
	return strcmp(((Libro*)a)->isbn, ((Libro*)b)->isbn);
}

int matchTitulo(Libro *a, Libro *b)        { return icmp(a->titulo, b->titulo) == 0; }
int matchAutor(Libro *a, Libro *b)         { return icmp(a->autor, b->autor) == 0; }
int matchFecha(Libro *a, Libro *b)         { return a->fecha == b->fecha; }
int matchInicialTitulo(Libro *a, Libro *b) { return claveInicialTitulo(a) == claveInicialTitulo(b); }
int matchInicialAutor(Libro *a, Libro *b)  { return claveInicialAutor(a) == claveInicialAutor(b); }

int ordenTitulo(void *a, void *b)
{
	int r = icmp(((Libro*)a)->titulo, ((Libro*)b)->titulo);
	return r ? r : strcmp(((Libro*)a)->isbn, ((Libro*)b)->isbn);
}

int ordenFechaTitulo(void *a, void *b)
{
	int fa = ((Libro*)a)->fecha, fb = ((Libro*)b)->fecha;
	if(fa != fb)
		return fa < fb ? -1 : 1;
	return ordenTitulo(a, b);
}

int ordenAutorFechaTitulo(void *a, void *b)
{
	int r = icmp(((Libro*)a)->autor, ((Libro*)b)->autor);
	return r ? r : ordenFechaTitulo(a, b);
}

int icmp(const char *a, const char *b)
{
	for(; *a && *b; a++, b++)
	{
		int x = tolower((unsigned char)*a), y = tolower((unsigned char)*b);
		if(x != y)
			return x < y ? -1 : 1;
	}
	return (unsigned char)*a - (unsigned char)*b;
}

int leerTexto(const char *mensaje, char *buffer, size_t tam)
{
	char entrada[MAX_ENTRADA];
	printf("%s", mensaje);
	fflush(stdout);
	if(!inputCadena("", entrada, MAX_ENTRADA))
		return 0;
	char *ini = entrada;
	while(isspace((unsigned char)*ini)) ini++;
	size_t len = strlen(ini);
	while(len && isspace((unsigned char)ini[len - 1])) ini[--len] = '\0';
	if(!len)
	{
		printf("\n No escribiste nada");
		return 0;
	}
	strncpy(buffer, ini, tam - 1);
	buffer[tam - 1] = '\0';
	return 1;
}

int leerInicial(const char *mensaje, char *inicial)
{
	char entrada[MAX_ENTRADA];
	if(!leerTexto(mensaje, entrada, sizeof(entrada)))
		return 0;
	if(!isalnum((unsigned char)entrada[0]))
	{
		printf("\n Escribe una letra o un numero");
		return 0;
	}
	*inicial = (char)toupper((unsigned char)entrada[0]);
	return 1;
}