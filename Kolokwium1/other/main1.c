//otwórz plik podany jako argument i skopiuj jego zawartość do pliku podanego jako drugi argument
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char* argv[]){
    //zaalokuj pamięć dla tablicy dwuwymiarowej i potem ją zwolnij
    char ** batat = calloc(20,sizeof(char*));
    int i =0;
    for (;i<20;i++){batat[i] = calloc(20,sizeof(char));}

    FILE * plik1;
    FILE * plik2;
    char * path;
    path = realpath(argv[1], NULL);
    plik1 = fopen(path, "r");
    realpath(argv[2], path);
    plik2 = fopen(path, "w+");
    char buffer [20];
    int got =0;
    while((got =fread(buffer,sizeof(char), 20, plik1))){
        fwrite(buffer,sizeof(char), got, plik2);
    }
    for (i=0;i<20;i++){free(batat[i]);}
    free(batat);
    return 0;
}
