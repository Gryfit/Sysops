#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sighandler(int sig, siginfo_t *info, void *ucontext){
    printf("%d\n", info->si_value);
}


int main(int argc, char* argv[]) {

    if(argc != 3){
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
    action.sa_sigaction = &sighandler;
    sigfillset(&action.sa_mask);   
    sigdelset(&action.sa_mask, SIGUSR1);
    //sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;
    union sigval v;
    v.sival_int = atoi(argv[2]);
    //..........


    int child = fork();
    if(child == 0) {
        sigaction(SIGUSR1, &action, NULL);
        while(1){}
        //zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
        //zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny wydrukowal
        //na konsole przekazana przez rodzica wraz z sygnalem SIGUSR1 wartosc
    }
    else {
        sleep(1);
        sigqueue(child, SIGUSR1, v);
        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[1]
    }

    return 0;
}
