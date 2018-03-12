all: main.o ../zadanie1/dynamic.o ../zadanie1/static.o
	gcc main.o ../zadanie1/dynamic.o ../zadanie1/static.o -o zad2
main.o: main.c ../zadanie1/dynamic.h ../zadanie1/static.h 
	gcc main.c -c -o main.o
dynamic.o: ../zadanie1/dynamic.c ../zadanie1/dynamic.h
	gcc ../zadanie1/dynamic.c -c -o dynamic.o
static.o: ../zadanie1/static.c ../zadanie1/static.h
	gcc ../zadanie1/static.c -c -o static.o



clean:
	rm ../zadanie1/*o *o zad2
