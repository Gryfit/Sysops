static-load: staticloading.o dynamic.o static.o
	gcc staticloading.o dynamic.o static.o -o lab1

share-static-load: staticloading.c dynamic.so static.so
	gcc staticloading.o dynamic.so static.so -o lab1
	gcc -L`pwd` -Wall -o lab1 staticloading.c -ldynamic -lstatic


dynamic.so: dynamic.c dynamic.h
	gcc -c -Wall -Werror -fpic dynamic.c
	gcc -shared -o dynamic.so dynamic.o
static.so: static.c static.h
	gcc -c -Wall -Werror -fpic dynamic.c
	gcc -shared -o dynamic.so dynamic.o




staticloading.o: staticloading.c dynamic.h static.h
	gcc staticloading.c -c -o staticloading.o
dynamic.o: dynamic.c dynamic.h
	gcc dynamic.c -c -o dynamic.o
static.o: static.c static.h
	gcc static.c -c -o static.o



clean:
	rm *o lab1
