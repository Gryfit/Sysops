#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>


int main(int argc, char* argv[])
{

 if(argc !=2){
    printf("Not a suitable number of program parameters\n");
    exit(1);
 }

 int toChildFD[2];
 int toParentFD[2];

 pipe(toChildFD);
 pipe(toParentFD);

 int val1,val2,val3 = 0;

 pid_t pid;

    if((pid = fork()) == 0) {
        close(toChildFD[1]);
        char buff [20];
        read(toChildFD[0], &buff, 20);
        val2 = atoi(buff);
    //odczytaj z potoku nienazwanego wartosc przekazana przez proces macierzysty i zapisz w zmiennej val2

     val2 = val2 * val2;
        sprintf(buff, "%d", val2);
        close(toParentFD[0]);
        write(toParentFD[1], buff, strlen(buff));
    //wyslij potokiem nienazwanym val2 do procesu macierzysego

 } else {
     char buff [20];
     val1 = atoi(argv[1]);
     sprintf(buff, "%d", val1);
     close(toChildFD[0]);
     write(toChildFD[1], buff, strlen(buff));
    //wyslij val1 potokiem nienazwanym do priocesu potomnego

     sleep(1);
     close(toParentFD[1]);
     read(toParentFD[0], &buff, 20);
     val3 = atoi(buff);
    //odczytaj z potoku nienazwanego wartosc przekazana przez proces potomny i zapisz w zmiennej val3

     printf("%d square is: %d\n",val1, val3);
 }
 return 0;
}
