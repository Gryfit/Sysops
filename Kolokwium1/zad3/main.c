#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main ( int argc, char *argv[] )
{
    int i, pid;

    if(argc != 2){
        printf("Not a suitable number of program arguments");
        exit(2);
    }else {
       for (i = 0; i < atoi(argv[1]); i++) {
           if((pid = fork())== 0 ){
               printf("I'm %d child with pid %d and my parent has pid %d\n", i, getpid(), getppid());
               fflush(stdout);
               exit(0);
           }else{
               printf("Child created %d\n", getpid());
           }
           if(i == atoi(argv[1])-1)
            sleep(1);
        //*********************************************************
        //Uzupelnij petle w taki sposob aby stworzyc dokladnie argv[1] procesow potomnych, bedacych dziecmi
        //   tego samego procesu macierzystego.
           // Kazdy proces potomny powinien:
               // - "powiedziec ktorym jest dzieckiem",
                //-  jaki ma pid,
                //- kto jest jego rodzicem
           //******************************************************
        }
    }
    return 0;
}
