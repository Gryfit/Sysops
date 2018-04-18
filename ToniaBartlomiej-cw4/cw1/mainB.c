#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>



pid_t pid = -1;
int i =0;

void handleINT(int signum){
	printf("\nOdebrano sygnal SIGINT\n");
    if(kill(pid,SIGKILL)==-1){
    	printf("Error");
    }
	kill(getpid(),SIGKILL); //kill yourself :)
}
void handleTSTP(int signum){
	char * args [2];
	args[0] =  "./inf.sh";
	args[1] = NULL; 
	if(i%2==0){	
		puts("Oczekuję na CTRL+Z - kontynuacja albo CTR+C - zakonczenie programu");
		//kill(pid,SIGTSTP);
		kill(pid,SIGKILL);
		i++;
	}else{
		puts("Wznawiam");
		//kill(pid,SIGCONT);
		pid = fork(); 
   		if(pid<0){
    		printf("ERROR\n");
		}
		if(pid==0){
			execvp(args[0], args);
		}
		i++;
	}
}
int run(){
	char * args [2];
	struct sigaction act;
	
	args[0] =  "./inf.sh";
	args[1] = NULL;  
	act.sa_handler = handleTSTP; 
	sigemptyset(&act.sa_mask); 
	act.sa_flags = 0; 

	pid = fork(); 
    if(pid<0){
    	return -1;
	}
	if(pid==0){
		execvp(args[0], args);
	}else{
		sigaction(SIGTSTP, &act, NULL); 
		signal(SIGINT,handleINT);
		while (1){};
	}
	return 1;
}
int main(int arg, char*args[]) {
	return run();
}
