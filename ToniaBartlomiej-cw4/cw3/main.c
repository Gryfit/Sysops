#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define DECLARE_HANDLE                                                      	\
    struct sigaction sig_act;                                                  	\
    memset(&sig_act, 0, sizeof(sig_act));                                      	\
    sigemptyset(&sig_act.sa_mask);                                             	\
    sig_act.sa_flags = SA_SIGINFO;                                             	\

#define HANDLE_SIGNAL(signal, function) {                                      	\
    sig_act.sa_sigaction = &(function);                                        	\
    sigaction((signal), &sig_act, 0);                                          	\
}
int L, Type;
int PARENT_COUNTER =0;
int SEND=0;
int CHILD_COUNTER =0;
pid_t child=0;
///////////////////PARENT SIGNALS////////////////////////////////////
void parent_sigint(int _signum, siginfo_t *siginfo, void *_context){
    if(kill(child,0)!=-1){
        kill(child,SIGTERM);
    }
    kill(getpid(),SIGTERM);
}
void parent_sigusr1_1(int _signum, siginfo_t *siginfo, void *_context){
    PARENT_COUNTER++;
}
void parent_sigusr1_2(int _signum, siginfo_t *siginfo, void *_context){
    PARENT_COUNTER++;
    if(SEND<L){
        kill(siginfo->si_pid,SIGUSR1);
        SEND++;
    }else{
        kill(siginfo->si_pid,SIGUSR2);
    }
}
void parent_sigusr1_3(int _signum, siginfo_t *siginfo, void *_context){
    PARENT_COUNTER++;
}
////////////////CHILD SIGNALS////////////////////////////////////////////
void child_sigusr1_123(int _signum, siginfo_t *siginfo, void *_context){
    CHILD_COUNTER++;
    kill(siginfo->si_pid,siginfo->si_signo);
}
void child_sigusr2_123(int _signum, siginfo_t *siginfo, void *_context){
    printf("Received form parent:\t%d\n",CHILD_COUNTER);
    kill(getpid(),SIGTERM);
}
///////////////////////////////////////////////////////////////////////
void run(){
    DECLARE_HANDLE;
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask,SIGRTMIN+8);
    sigdelset(&mask,SIGRTMIN+5);
    sigdelset(&mask,SIGUSR1);
    sigdelset(&mask,SIGUSR2);
    sig_act.sa_mask = mask;
    HANDLE_SIGNAL((Type!=3)?SIGUSR1:SIGRTMIN+8 ,child_sigusr1_123);
    HANDLE_SIGNAL((Type!=3)?SIGUSR2:SIGRTMIN+5 ,child_sigusr2_123);
    while(1){}
}
void child_is_dead(int _signum, siginfo_t *siginfo, void *_context){
    printf("Send form parent:\t%d\n",SEND);
    printf("Received form child:\t%d\n", PARENT_COUNTER);
    kill(getpid(),SIGKILL);
}
void f(){
    DECLARE_HANDLE;
    HANDLE_SIGNAL(SIGINT,parent_sigint);
    HANDLE_SIGNAL(SIGCHLD,child_is_dead);
    if(Type==1){
        HANDLE_SIGNAL(SIGUSR1,parent_sigusr1_1);
    }
    if(Type==2){
        HANDLE_SIGNAL(SIGUSR1,parent_sigusr1_2);
    }
    if(Type==3){
        HANDLE_SIGNAL(SIGRTMIN+8,parent_sigusr1_3);
    }
    ///////////////////////////
    child = fork();
	if(child==0){
		return run();
	}
    /////////////////////////
    if(Type==1){
        puts(" ");
        for(;SEND<L;SEND++){
            kill(child,SIGUSR1);
            sleep(1);
        }
        kill(child,SIGUSR2);
    }
    if(Type==2){
        puts(" ");
        SEND++;
        kill(child,SIGUSR1);
    }
    if(Type==3){
        puts(" ");
        for(;SEND<L;SEND++){
            kill(child,SIGRTMIN+8);
            sleep(1);
        }
        kill(child,SIGRTMIN+5);
    }
    while(1){}
}

int main(int arg, char*args[]) {
	if(arg != 3){
	printf("Expected: ./main [L] [Type]\n");
	return -1;
	}
	L=atoi(args[1]);
	Type=atoi(args[2]);
	f();
	return 0;
}
