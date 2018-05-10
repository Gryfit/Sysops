
//2. Dostajesz pliki sender.c i receiver.c. Sender powinien utworzyć nazwany potok (jest już podany #define z nazwą), wpisać do niej pierwszy argumtn programu jako int "i posprzątać" (zrozumialem to jako zamknięcie i usunięcie pipe'y).
//Receiver powinien otworzyć ten sam potok, odczytać przekazaną liczbę i wypisać (printf był już w kodzie, wystarczyło wczytać wartość do zadanej zmiennej).
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>

#define  FIFONAME "./YOLO"


int main(int argc, char const *argv[]) {
    FILE * fifo;
    fifo = fopen(FIFONAME, "r");
    int a;
    fread(&a, sizeof(int), 1,fifo);
    printf("REVICED %d\n", a);
    fclose(fifo);
    remove(FIFONAME);
    return 0;
}
