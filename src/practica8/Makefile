cflags = -std=c99 -O2
dflags = -std=c99 -g
includes = -I ../iostring -I ../datastructures/tablashash/cuadratica -I ../datastructures/arbol -I ../datastructures/nodo -I ../datastructures/lista
libs = ../../lib -lArbol -lLista -liostring
source = $(wildcard *.c) ../datastructures/tablashash/cuadratica/HashQ.c
output = ../../bin/practica8

build :
	gcc $(cflags) $(source) -o $(output) $(includes) -L $(libs)
	@echo "Compilo Exitosamente!!!"
run :
	$(output)