all-static: main2.o dynamic.o static.o
	gcc -std=c99 main2.o dynamic.o static.o -o zad -O$(optim)

all-dynamic: dynamic.so static.so
	gcc -std=c99 -L . -o zad main3.c -lstatic -ldynamic -ldl -O$(optim)
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`

all-shared: main3.o static.so dynamic.so
	gcc -std=c99 -L . -o zad main2.c -lstatic -ldynamic -O$(optim)
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`

dynamic.so: dynamic.c dynamic.h
	gcc -std=c99 -c -fpic dynamic.c -O$(optim)
	gcc -std=c99 -shared -o libdynamic.so dynamic.o -O$(optim)
static.so: static.c static.h
	gcc -std=c99 -c -fpic static.c -O$(optim)
	gcc -std=c99 -shared -o libstatic.so static.o -O$(optim)

dynamic.o: dynamic.c dynamic.h
	gcc -std=c99 dynamic.c -c -o dynamic.o -O$(optim)
static.o: static.c static.h
	gcc -std=c99 static.c -c -o static.o -O$(optim)

main2.o: main2.c dynamic.h static.h 
	gcc -std=c99 main2.c -c -o main2.o
main3.o: main3.c dynamic.h static.h 
	gcc -std=c99 main3.c -c -o main3.o
clean:
	rm *o zad
