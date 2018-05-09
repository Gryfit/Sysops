#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#define FAILURE_EXIT(format, ...) { printf(format, ##__VA_ARGS__); exit(-1); }
#define BUFFER 128
pid_t pid;

int main(int argc, char** argv){
    FILE *date_file;
    int fifo;
    char read_buffer[BUFFER],write_buffer[BUFFER];
    int i;
    pid = getpid();
    srand((unsigned int)time(0)*getpid());
    if(argc!=3) FAILURE_EXIT("Expected: ./slave [path] [N]\n")
    int n;
    n = atoi(argv[2]);
    fifo = open(argv[1], O_WRONLY);
    if(fifo<0) FAILURE_EXIT("slave (%d):\tCan't open FIFO\n",pid)

    printf("slave:\tpid %d\n",pid);

    for(i=0;i<n;i++){
        date_file = popen("date","r");
        fgets(read_buffer, BUFFER, date_file);
        sprintf(write_buffer,"slave (%d):\tpid %d date %s",i,pid,read_buffer);
        write(fifo, write_buffer, strlen(write_buffer));
        fclose(date_file);
        sleep((unsigned int) (rand()%4 + 2));
    }
    close(fifo);
    return EXIT_SUCCESS;
}
