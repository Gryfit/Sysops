//
// Created by gryfit on 10.03.18.
//

//static loading of library
#include <stdio.h>
#include <stdlib.h>
#include "../zadanie1/dynamic.h"
#include <time.h>
#include <memory.h>
#include "../zadanie1/static.h"
#include <sys/resource.h>

int main(int argc, char* argv[]){

    if(argc ==1){
        printf("zad1 [Liczba elementów tablicy] [rozmiar bloku] [sposób alokacji pamięci] [operacja] [static/dynamic library]\n"
                       "sposób alokacji:\n"
                       "1 -> calloc\n"
                       "2 -> static\n"
                       "Operacje:\n"
                       "Create\n"
                       "Find [index]\n"
                       "DelAdd [lenght]\n"
                       "DelAddAlternating [length]\n"
        );
        return 0;
    }


    char ** T;
    struct rusage usage_start, usage_end;
    struct timeval start_sys, end_sys,start_u,end_u;
    clock_t start_real,end_real;
    if(strcmp(argv[3],"1")==0){
        if(strcmp(argv[4],"Create")==0){
            start_real = clock();
            getrusage(RUSAGE_SELF, &usage_start);

            T = newTableCalloc(atoi(argv[1]), (unsigned)atoi(argv[2])); //allocate memory


            getrusage(RUSAGE_SELF, &usage_end);
            end_real = clock();
            printf("dynamic.h Create: \n");
        }
        if(strcmp(argv[4],"Find")==0){
            T = newTableCalloc(atoi(argv[1]), (unsigned)atoi(argv[2]));
            start_real = clock();
            getrusage(RUSAGE_SELF, &usage_start);



            findCalloc(T,atoi(argv[5]),atoi(argv[1]),(unsigned)atoi(argv[2]));


            getrusage(RUSAGE_SELF, &usage_end);
            end_real = clock();
            printf("dynamic.h Find: \n");
        }
        if(strcmp(argv[4],"DelAdd")==0){
            T = newTableCalloc(atoi(argv[1]), (unsigned)atoi(argv[2]));
            start_real = clock();
            getrusage(RUSAGE_SELF, &usage_start);

            for(int i=0;i<atoi(argv[5]);i++){
                delBlockCalloc(T,i);
            }
            for(int i=0;i<atoi(argv[5]);i++){
                addBlockCalloc(T,i,(unsigned)atoi(argv[2]));
            }


            getrusage(RUSAGE_SELF, &usage_end);
            end_real = clock();
            printf("dynamic.h DelAdd: \n");
        }
        if(strcmp(argv[4],"DelAddAlternating")==0){
            T = newTableCalloc(atoi(argv[1]), (unsigned)atoi(argv[2]));
            start_real = clock();
            getrusage(RUSAGE_SELF, &usage_start);


            for(int i=0;i<atoi(argv[5]);i++){
                delBlockCalloc(T,i);
                addBlockCalloc(T,i,(unsigned)atoi(argv[2]));
            }


            getrusage(RUSAGE_SELF, &usage_end);
            end_real = clock();
            printf("dynamic.h DelAddAlternating: \n");
        }
    }
    if(strcmp(argv[3],"2")==0){
        if(strcmp(argv[4],"Create")==0){
            start_real = clock();
            getrusage(RUSAGE_SELF, &usage_start);


            newTableStatic(atoi(argv[1]), (unsigned)atoi(argv[2]));

            getrusage(RUSAGE_SELF, &usage_end);
            end_real = clock();
            printf("static.h Create: \n");
        }
        if(strcmp(argv[4],"Find")==0){
            start_real = clock();
            getrusage(RUSAGE_SELF, &usage_start);


            findStatic(atoi(argv[5]),atoi(argv[1]),(unsigned)atoi(argv[2]));

            getrusage(RUSAGE_SELF, &usage_end);
            end_real = clock();
            printf("static.h Find: \n");
        }
        if(strcmp(argv[4],"DelAdd")==0){
            newTableStatic(atoi(argv[1]), (unsigned)atoi(argv[2]));
            start_real = clock();
            getrusage(RUSAGE_SELF, &usage_start);


            for(int i=0;i<atoi(argv[5]);i++){
                delBlockStatic(i, (unsigned)atoi(argv[2]));
            }
            for(int i=0;i<atoi(argv[5]);i++){
                addBlockStatic(i,(unsigned)atoi(argv[2]));
            }


            getrusage(RUSAGE_SELF, &usage_end);
            end_real = clock();
            printf("static.h DelAdd: \n");
        }
        if(strcmp(argv[4],"DelAddAlternating")==0){
            newTableStatic(atoi(argv[1]), (unsigned)atoi(argv[2]));
            start_real = clock();
            getrusage(RUSAGE_SELF, &usage_start);


            for(int i=0;i<atoi(argv[5]);i++){
                delBlockStatic(i, (unsigned)atoi(argv[2]));
                addBlockStatic(i,(unsigned)atoi(argv[2]));
            }

            getrusage(RUSAGE_SELF, &usage_end);
            end_real = clock();
            printf("static.h DelAddAlternating: \n");
        }
    }

    double real = (double)(end_real-start_real)/CLOCKS_PER_SEC;
    double user = (usage_end.ru_utime.tv_sec-usage_start.ru_utime.tv_sec)+(double)(usage_end.ru_utime.tv_usec-usage_start.ru_utime.tv_usec)/1000000;

    double sys = (usage_end.ru_stime.tv_sec-usage_start.ru_stime.tv_sec)+(double)(usage_end.ru_stime.tv_usec-usage_start.ru_stime.tv_usec)/1000000;
    printf("real time: %f s\n"
                   "user time: %f s\n"
                   "system time: %f s\n",real,user,sys);
}


