#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <memory.h>
#include <errno.h>
#include <wait.h>

#define MAX_LINE 1000
#define FAILURE_EXIT(format, ...) { printf(format, ##__VA_ARGS__); free(line); free(commands); free(args); fclose(file); exit(-1); }
#define _INIT_                                                                  \
    if (argc != 2) FAILURE_EXIT("Expected: ./main [path]\n")                    \
    file = fopen(argv[1], "r");                                                 \
    if (file == 0) FAILURE_EXIT("Couldn't open file\n")                         \
    if(!(args = calloc(MAX_LINE, sizeof(char*))))                               \
        FAILURE_EXIT("Failed to allocate arguments\n")                          \
    if(!(commands = calloc(MAX_LINE, sizeof(char*))))                           \
        FAILURE_EXIT("Failed to allocate arguments\n")                          \

char *strsep(char **stringp, const char *delim);

int parseargs(char* line,char** newargs, char *delimeter){
    int last = strlen(line)-1;
	int i=0;
    if (line[last] == '\n') line[last] = '\0';
	for(i=0;(newargs[i] = strsep(&line, delimeter)); i++){ }
    return i;
}


int main(int argc, char** argv) {
    char **args, **commands, *line;
    int i, count, pipes[2][2];
    size_t length;
    FILE *file;

    file=0; args=0; commands=0; line =0;

    _INIT_

    while ((getline(&line, &length, file)) != -1) {
        count = parseargs(line, commands,"|");
        for (i = 0; i < count; i++) {
            if (i > 1){
                close(pipes[i % 2][0]);
                close(pipes[i % 2][1]);
            }
            pipe(pipes[i % 2]);

            pid_t pid = fork();
            if (pid == 0) {
                parseargs(commands[i],args," ");
                if (i < count - 1){
                    close(pipes[i % 2][0]);
                    dup2(pipes[i % 2][1], 1);
                }
                if (i > 0){
                    close(pipes[(i + 1) % 2][1]);
                    dup2(pipes[(i + 1) % 2][0], 0);
                }
                execvp(args[0], args);
            } else if (pid == -1) {
                FAILURE_EXIT("fork() failed\n")
            }
        }
        close(pipes[i % 2][0]);
        close(pipes[i % 2][1]);
        for(i=0;i<count;i++){
            wait(0);
        }
    }
    free(line);
    fclose(file);
    return 0;
}
