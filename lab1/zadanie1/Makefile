all-shared: dynamic.so static.so
	make dynamic.so
	make static.so
dynamic.so: dynamic.c dynamic.h
	gcc -c -fpic dynamic.c
	gcc -shared -o dynamic.so dynamic.o
static.so: static.c static.h
	gcc -c -fpic static.c
	gcc -shared -o static.so static.o

all-static: dynamic.o static.o
	make dynamic.o
	make static.o
dynamic.o: dynamic.c dynamic.h
	gcc dynamic.c -c -o dynamic.o
static.o: static.c static.h
	gcc static.c -c -o static.o


clean:
	rm *o
