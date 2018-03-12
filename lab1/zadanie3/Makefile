
all-static: ../zadanie2/main.o ../zadanie1/dynamic.o ../zadanie1/static.o
	gcc ../zadanie2/main.o ../zadanie1/dynamic.o ../zadanie1/static.o -o zad3 -O$(optim)

all-dynamic: main.c dynamic.so static.so
	export LD_LIBRARY_PATH=/home/gryfit/Documents/Sysopy/Sysops/lab1/zadanie3:$LD_LIBRARY_PATH
	gcc -L/home/gryfit/Documents/Sysopy/Sysops/lab1/zadanie3 -o zad3 main.c -lstatic -ldynamic -ldl -O$(optim)

all-shared: main.c ../zadanie2/main.o static.so dynamic.so
	export LD_LIBRARY_PATH=/home/gryfit/Documents/Sysopy/Sysops/lab1/zadanie3:$LD_LIBRARY_PATH
	gcc -L/home/gryfit/Documents/Sysopy/Sysops/lab1/zadanie3 -o zad3 ../zadanie2/main.c -lstatic -ldynamic -O$(optim)

dynamic.so: ../zadanie1/dynamic.c ../zadanie1/dynamic.h
	gcc -c -fpic ../zadanie1/dynamic.c -O$(optim)
	gcc -shared -o libdynamic.so dynamic.o -O$(optim)
static.so: ../zadanie1/static.c ../zadanie1/static.h
	gcc -c -fpic ../zadanie1/static.c -O$(optim)
	gcc -shared -o libstatic.so static.o -O$(optim)
clean:
	rm ../zadanie1/*o ../zadanie2/*o ../zadanie3/*o zad3
