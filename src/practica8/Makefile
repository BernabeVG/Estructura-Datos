cflags = -std=c99 -O2 
dflags = -std=c99 -g
includes = -I ../iostring -I ../datastructures/tablashash/cuadratica
libs = ../../lib -liostring -lHashQ
source = $(wildcard *.c) 
output = ../../bin/practica8

build :
	gcc $(cflags) $(source) -o $(output) $(includes) -L $(libs)
	@echo "Compilo Exitosamente!!!"
run :
	$(output).exe