#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define FAILURE_EXIT(format, ...) { printf(format, ##__VA_ARGS__); exit(-1); }
#define BUFFER 128
int mkfifo(const char *pathname, mode_t mode);

int main(int argc, char** argv){
    FILE *fifo;
    char buffer[BUFFER];
    if(argc!=2) FAILURE_EXIT("Expected: ./master [path]\n")
    if(mkfifo(argv[1],S_IRUSR | S_IWUSR)== -1){
        FAILURE_EXIT("master:\tCan't create FIFO\n")
    }
    puts("master:\tI'm ready");
    kill(getppid(),SIGUSR1);
    fifo = fopen(argv[1],"r");
    if(!fifo) FAILURE_EXIT("master:\tCan't open FIFO\n")
    while(fgets(buffer, BUFFER, fifo) != 0){
        printf("master:\t%s",buffer);
    }
    fclose(fifo);
    if(remove(argv[1])) FAILURE_EXIT("master:\tCan't delete FIFO\n")
    return EXIT_SUCCESS;
}
