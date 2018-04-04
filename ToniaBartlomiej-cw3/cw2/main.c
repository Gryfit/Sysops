#include <unistd.h>
#include <sys/resource.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include  <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE 1000
char *strsep(char **stringp, const char *delim);

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

int interpreter(char* path){ 
	FILE* file;
	char* line;
	char** args;
	int arg;
	int state;
	pid_t wpid, pid;
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
			execvp(args[0], args);
		}
		while ((wpid = wait(&state)) > 0);
		if(state!=0){
			printf("Error\n");
			return -1;
		}
		printf("\n");
	}
	return 0;
}

int main(int arg, char*args[]) {
    if (arg == 1){
        printf("./main [path]\n");
        return 0;
	}
    return interpreter(args[1]);
}

