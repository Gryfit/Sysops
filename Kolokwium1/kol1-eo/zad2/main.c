#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

volatile int x =0;

void f(){
    x =0;
}


int main()
{
  pid_t child;
  int status, retval;
  if((child = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if(child == 0) {
    sleep(1);
    exit(EXIT_SUCCESS);
  }
  else {
/* Proces macierzysty pobiera status  zakończenie potomka child,
 * nie zawieszając swojej pracy. Jeśli proces się nie zakończył, wysyła do dziecka sygnał SIGKILL.
 * Jeśli wysłanie sygnału się nie powiodło, ponownie oczekuje na zakończenie procesu child,
 * tym razem zawieszając pracę do czasu zakończenia sygnału
 * jeśli się powiodło, wypisuje komunikat sukcesu zakończenia procesu potomka z numerem jego PID i statusem zakończenia. */

        signal(SIGCHLD,f);

        int status;
        while(x){}
        if(waitpid(-1, &status, WNOHANG)==-1){
            printf("%s\n", "FAKAP");
        }
        if(status==0){
            if(kill(child, SIGKILL)==-1){
                wait(&status);
            }
        }
        printf("Udalo sie zakonczyc proces, pid: %d, exit code: %d\n", child, WEXITSTATUS(status));


/* koniec*/
 } //else
  exit(EXIT_SUCCESS);
}
