#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
int i =0;
int run();
void waiting();
void handleINT(int signum){
	printf("\nOdebrano sygnal SIGINT\n");
	kill(getpid(),SIGKILL); //kill yourself :)
}
void handleTSTP(int signum){
	if(i%2==0){	
		puts("Oczekuję na CTRL+Z - kontynuacja albo CTR+C - zakonczenie programu");
		i++;
	}else{
		puts("Wznawiam");
		i--;
	}
}
void waiting(){
	while(1){};
}

int run(){
	time_t rawtime;
	struct tm * local;
	char buffer[80];
	while (1){
		if(i%2==0){
			time (&rawtime);
			local = localtime(&rawtime);
			strftime(buffer,80,"%d-%m-%Y %I:%M:%S",local);
			printf("%s\n",buffer);
		}
		sleep(1);
	};
	return 1;	
}
int main(int arg, char*args[]) {
	struct sigaction act;
	act.sa_handler = handleTSTP; 
	sigemptyset(&act.sa_mask); 
	act.sa_flags = 0; 
	sigaction(SIGTSTP, &act, NULL); 
	signal(SIGINT,handleINT);	
	return run();
}
