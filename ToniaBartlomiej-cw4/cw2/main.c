#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define SIGDIFF 32

#define DECLARE_HANDLE                                                      	\
    struct sigaction sig_act;                                                  	\
    memset(&sig_act, 0, sizeof(sig_act));                                      	\
    sigemptyset(&sig_act.sa_mask);                                             	\
    sig_act.sa_flags = SA_SIGINFO;                                             	\

#define HANDLE_SIGNAL(signal, function) {                                      	\
    sig_act.sa_sigaction = &(function);                                        	\
    sigaction((signal), &sig_act, 0);                                          	\
}


pid_t *children, *requests, parent;
int N, K, alive_children;
volatile sig_atomic_t signals_received;

void Clear(){
	int i=0;
	puts("\nClear.\n");
	for(;i<N;i++){
		kill(children[i], SIGKILL);
	}
	kill(parent,SIGKILL);
}
void handle_in_parent_realtime_print(int signum, siginfo_t *siginfo, void *_context) {
    printf("Parent:\t\tSIGMIN+%i, from child %i\n", signum - SIGRTMIN, siginfo->si_pid);
	fflush(stdout);
}
void handle_in_child(int _signum, siginfo_t *siginfo, void *_context) {
    int d = rand() % SIGDIFF;
    printf("Child %d:\trecieved SIGUSR2\n",getpid());
    kill(getppid(), SIGRTMIN + d);
    waitpid(getppid(),NULL,0);
}

void handle_in_parent_SIGCHLD(int _signum, siginfo_t *siginfo, void *_context){
 	int i;
	int exit_status;
	for (i = 0; i < N; ++i) {
		if (children[i] == siginfo->si_pid)
			children[i] = 0;
 	}
 	wait(&exit_status);
 	alive_children--;
	printf("Child %i:\texited with status %i\n", siginfo->si_pid, WEXITSTATUS(exit_status));
	fflush(stdout);
}

void handle_in_parent_usr1(int _signum, siginfo_t *siginfo, void *_context){
	printf("Parent:\t\trecieved SIGUSR1 from child %d\n",siginfo->si_pid);
	if (getpid() != parent) return;
	if (signals_received < K) {
		requests[signals_received++] = siginfo->si_pid;
	}else{
        if (signals_received == K) {
            int i;
		    kill(siginfo->si_pid,SIGUSR2);
            waitpid(siginfo->si_pid,NULL,0);
		    for (i = 0; i <signals_received; ++i){
                kill(requests[i], SIGUSR2);
                waitpid(requests[i],NULL,0);
		    }
            signals_received++;
	   }else{
           if (signals_received > K) {
               kill(siginfo->si_pid, SIGUSR2);
               waitpid(siginfo->si_pid,NULL,0);
           }
       }
    }
}

int f(){
	int i;
	DECLARE_HANDLE;
	for(i=0;i<SIGDIFF;i++){
		HANDLE_SIGNAL(SIGRTMIN+i, handle_in_parent_realtime_print);
	}
	HANDLE_SIGNAL(SIGINT,Clear);
	HANDLE_SIGNAL(SIGUSR1,handle_in_parent_usr1);
	HANDLE_SIGNAL(SIGCHLD,handle_in_parent_SIGCHLD);
	signals_received =0;
	alive_children =0;
	parent = getpid();
	children = calloc(N,sizeof(pid_t));
	requests = calloc(N,sizeof(pid_t));

	for(i=0;i<N;i++){
		children[i]=0;
	}
	for(i=0;i<N;i++){
		children[i] = fork();
		alive_children++;
		if(children[i]==0){
            srand(time(0)*getpid());
            int t;
            DECLARE_HANDLE;
            HANDLE_SIGNAL(SIGUSR2, handle_in_child);
            t=(rand()%10)+1;
            printf("Child %d:\tcreated! Sleep time: %d\n",getpid(),t);
            sleep(t);
            printf("Child %d:\tsending SIGUSR1\n",getpid());
            kill(parent,SIGUSR1);
            waitpid(parent,NULL,0);
            pause();
            _exit(t);
		}
	}
	while(alive_children>0){sleep(1);}
	return 0;
}
int main(int arg, char*args[]) {
	if(arg!=3){
		printf("Expected: ./main [N] [K]\n");
		return -1;
	}
	N=atoi(args[1]);
	K=atoi(args[2]);
	f();
	return 0;
}
