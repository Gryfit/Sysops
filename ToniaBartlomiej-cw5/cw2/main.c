#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#define FAILURE_EXIT(format, ...) { printf(format, ##__VA_ARGS__); exit(-1); }

#define DECLARE_HANDLE                                                      	\
    struct sigaction sig_act;                                                  	\
    memset(&sig_act, 0, sizeof(sig_act));                                      	\
    sigemptyset(&sig_act.sa_mask);                                             	\
    sig_act.sa_flags = SA_SIGINFO;                                             	\

#define HANDLE_SIGNAL(signal, function) {                                      	\
    sig_act.sa_sigaction = &(function);                                        	\
    sigaction((signal), &sig_act, 0);                                          	\
}
volatile sig_atomic_t begin = 0;
void handle_master(int _signum, siginfo_t *_siginfo, void *_context){
    puts("main:\tMaster is ready.");
    begin =1;
}
int main(int argc, char** argv){
    pid_t pid;
    int i;
    if(argc != 4) FAILURE_EXIT("Expected: ./main [path] [N] [number of slaves]\n")
    DECLARE_HANDLE
    HANDLE_SIGNAL(SIGUSR1,handle_master)
    pid = fork();
    if(pid<0) FAILURE_EXIT("Can't fork new process\n")
    if(!pid){
        execlp("./master","./master",argv[1],NULL);
        return 0;
    }
    while(!begin){
        printf("main:\tAwating master\n");
        sleep(1);
    }
    pid_t slave;
    for(i=0;i<atoi(argv[3]);i++){
        slave = fork();
        if(slave<0) FAILURE_EXIT("Can't fork new process\n")
        if(!slave){
            execlp("./slave","./slave",argv[1],argv[2],NULL);
            return 0;
        }
    }

    while(wait(NULL)>0){}
    return EXIT_SUCCESS;
}
