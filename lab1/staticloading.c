//
// Created by gryfit on 10.03.18.
//

//static loading of library
#include <stdio.h>
#include <stdlib.h>
#include "dynamic.h"
#include <time.h>
#include <memory.h>
#include <sys/times.h>
#include "static.h"

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
    struct timespec startR, finishR;
    double real, user, sys;
    struct tms startCPU, finishCPU;

    if(strcmp(argv[3],"1")==0){
        if(strcmp(argv[4],"Create")==0){
            clock_gettime(CLOCK_MONOTONIC, &startR);
            times(&startCPU);

            T = newTableCalloc(atoi(argv[1]), (unsigned)atoi(argv[2])); //allocate memory

            times(&finishCPU);
            clock_gettime(CLOCK_MONOTONIC, &finishR);
            printf("dynamic.h Create: \n");
        }
        if(strcmp(argv[4],"Find")==0){
            T = newTableCalloc(atoi(argv[1]), (unsigned)atoi(argv[2]));

            clock_gettime(CLOCK_MONOTONIC, &startR);
            times(&startCPU);

            findCalloc(T,atoi(argv[5]),atoi(argv[1]),(unsigned)atoi(argv[2]));

            times(&finishCPU);
            clock_gettime(CLOCK_MONOTONIC, &finishR);
            printf("dynamic.h Find: \n");
        }
        if(strcmp(argv[4],"DelAdd")==0){
            T = newTableCalloc(atoi(argv[1]), (unsigned)atoi(argv[2]));
            clock_gettime(CLOCK_MONOTONIC, &startR);
            times(&startCPU);

            for(int i=0;i<atoi(argv[5]);i++){
                delBlockCalloc(T,i);
            }
            for(int i=0;i<atoi(argv[5]);i++){
                addBlockCalloc(T,i,(unsigned)atoi(argv[2]));
            }

            times(&finishCPU);
            clock_gettime(CLOCK_MONOTONIC, &finishR);
            printf("dynamic.h DelAdd: \n");
        }
        if(strcmp(argv[4],"DelAddAlternating")==0){
            T = newTableCalloc(atoi(argv[1]), (unsigned)atoi(argv[2]));
            clock_gettime(CLOCK_MONOTONIC, &startR);
            times(&startCPU);

            for(int i=0;i<atoi(argv[5]);i++){
                delBlockCalloc(T,i);
                addBlockCalloc(T,i,(unsigned)atoi(argv[2]));
            }

            times(&finishCPU);
            clock_gettime(CLOCK_MONOTONIC, &finishR);
            printf("dynamic.h DelAddAlternating: \n");
        }
    }
    if(argv[1]=="2"){
        if(strcmp(argv[4],"Create")==0){
            clock_gettime(CLOCK_MONOTONIC, &startR);
            times(&startCPU);

            newTableStatic(atoi(argv[1]), (unsigned)atoi(argv[2]));

            times(&finishCPU);
            clock_gettime(CLOCK_MONOTONIC, &finishR);
            printf("static.h Create: \n");
        }
        if(strcmp(argv[4],"Find")==0){
            newTableStatic(atoi(argv[1]), (unsigned)atoi(argv[2]));

            clock_gettime(CLOCK_MONOTONIC, &startR);
            times(&startCPU);

            findStatic(atoi(argv[5]),atoi(argv[1]),(unsigned)atoi(argv[2]));

            times(&finishCPU);
            clock_gettime(CLOCK_MONOTONIC, &finishR);
            printf("static.h Find: \n");
        }
        if(strcmp(argv[4],"DelAdd")==0){
            newTableCalloc(atoi(argv[1]), (unsigned)atoi(argv[2]));
            clock_gettime(CLOCK_MONOTONIC, &startR);
            times(&startCPU);

            for(int i=0;i<atoi(argv[5]);i++){
                delBlockStatic(i, (unsigned)atoi(argv[2]));
            }
            for(int i=0;i<atoi(argv[5]);i++){
                addBlockStatic(i,(unsigned)atoi(argv[2]));
            }


            times(&finishCPU);
            clock_gettime(CLOCK_MONOTONIC, &finishR);
            printf("static.h DelAdd: \n");
        }
        if(strcmp(argv[4],"DelAddAlternating")==0){
            newTableStatic(atoi(argv[1]), (unsigned)atoi(argv[2]));
            clock_gettime(CLOCK_MONOTONIC, &startR);
            times(&startCPU);

            for(int i=0;i<atoi(argv[5]);i++){
                delBlockStatic(i, (unsigned)atoi(argv[2]));
                addBlockStatic(i,(unsigned)atoi(argv[2]));
            }

            times(&finishCPU);
            clock_gettime(CLOCK_MONOTONIC, &finishR);
            printf("static.h DelAddAlternating: \n");
        }
    }

    real = (finishR.tv_sec - startR.tv_sec);                   //sec
    real += (finishR.tv_nsec - startR.tv_nsec) / 1000000000.0; //nanosec
    user = (finishCPU.tms_utime - startCPU.tms_utime);
    sys = (finishCPU.tms_stime - startCPU.tms_stime);
    printf("real time: %f s\n"
                   "user time: %f s\n"
                   "system time: %f s\n",real,user/CLOCKS_PER_SEC,sys/CLOCKS_PER_SEC);

}


