#include <unistd.h>
#include <sys/resource.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include  <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE 1000
char *strsep(char **stringp, const char *delim);
int setrlimit(int resource, const struct rlimit *rlim); 

int parseargs(char* line,char** newargs){
	char * token;
	int i=0;
	while ((token = strsep(&line, " \n"))){
		newargs[i]=token;
		newargs[i] = strsep(&newargs[i], "\n");
		i++;
	}
	newargs[i-1]= NULL;
	return i-1;
}

int interpreter(char* path, int timelimit, int sizelimit){ 
	FILE* file;
	char* line;
	char** args;
	int arg;
	int state;
	double sys, user;
	pid_t wpid, pid;
 	struct rlimit tlim, slim;
    struct rusage usage_start, usage_end;
	tlim.rlim_cur = timelimit;
	tlim.rlim_max = timelimit;
	slim.rlim_cur = sizelimit*1000000;
	slim.rlim_max = sizelimit*1000000;
	if(!(args = calloc(MAX_LINE, sizeof(char*))))
		return -1;
	for(int i=0;i<MAX_LINE;i++){
		if(!(args[i] = calloc(MAX_LINE, sizeof(char))))
			return -1;
	}
	if(!(line = calloc(MAX_LINE, sizeof(char))))
		return -1;
    if(!(file = fopen(path, "r")))
	    return -1;
	while((fgets(line, MAX_LINE, file))){
        getrusage(RUSAGE_CHILDREN, &usage_start);
		arg = parseargs(line, args);
		for(int i=0;i<arg;i++){
			printf("%s", args[i]);	
		}
		printf("\n");
       	pid = fork(); 
        if(pid<0){
        	return -1;
		}
		if(pid==0){
			if(setrlimit(RLIMIT_AS ,&slim)==-1 && setrlimit(RLIMIT_CPU,&tlim)==-1){
				perror("Limits not set.\n");
				return -1;
			}
			execvp(args[0], args);
		}
		while ((wpid = wait(&state)) > 0);
		if(state!=0){
			printf("Process ended with code: %d\n",state );
			return -1;
		}
        getrusage(RUSAGE_CHILDREN, &usage_end);
    	user =(usage_end.ru_utime.tv_sec-usage_start.ru_utime.tv_sec)+(double)(usage_end.ru_utime.tv_usec-usage_start.ru_utime.tv_usec)/1000000;
		sys = (usage_end.ru_stime.tv_sec-usage_start.ru_stime.tv_sec)+(double)(usage_end.ru_stime.tv_usec-usage_start.ru_stime.tv_usec)/1000000;
		printf("Real time: %f\t User time: %f\n\n", sys, user);
	}
	return 0;
}

int main(int arg, char*args[]) {
    if (arg == 1){
        printf("./main [path] [time limit] [size limmit]\n");
        return 0;
	}
    return interpreter(args[1],atoi(args[2]),atoi(args[3]));
}

