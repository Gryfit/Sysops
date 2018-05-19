#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <zconf.h>
#include <sys/times.h>

#define FAILURE_EXIT(format, ...) {                                            \
    printf(format, ##__VA_ARGS__);                                             \
    exit(-1);}

int MIN(double a,double b) {
    return a>b?b:a;
}

int MAX(double a,double b) {
    return a>b?a:b;
}
double diff(clock_t start, clock_t end){
    return (double)(end -  start) / sysconf(_SC_CLK_TCK);
}

int W,H,C,M;
int threads;
float ** K; // [C][C]
int ** I; //[W][H]
int ** J; //[W][H]


void load_picture(char const * path){
    FILE * file;
    int i,j;
    if(!(file = fopen(path,"r")))
        FAILURE_EXIT("cant open picture file\n")
    fscanf(file, "P2\n%d %d\n%d\n", &W,&H,&M);
    I = calloc(W, sizeof(int*));
    for(i=0;i<W;i++)
        I[i] = calloc(H, sizeof(int));
    for(i=0;i<W;i++)
        for(j=0;j<H;j++)
            fscanf(file, "%d%*[ \n]", &I[i][j]);
    fclose(file);
}

void load_filter(char const * path){
    FILE * file;
    int i,j;
    if(!(file = fopen(path,"r")))
        FAILURE_EXIT("cant open filter file\n");
    fscanf(file, "%d\n", &C);
    K = calloc(C, sizeof(float*));
    for(i=0;i<C;i++)
        K[i] = calloc(C, sizeof(float));
    for(i=0;i<C;i++)
        for(j=0;j<C;j++)
            fscanf(file, "%f%*[ \n]", &K[i][j]);
    fclose(file);
}

void * filter(void * arg){
    int thread = * (int *) arg;
    int start = W/threads*thread;
    int end = W/threads*(thread+1);
    printf("%d -> %d     -     %d\n",thread,start,end);
    int i,j,x,y;
    double s;
    int c = ceil(C/2);
    for(x=start;x<end;x++)
        for(y=0;y<H;y++){
            s=0;
            for(i=0;i<C;i++)
                for(j=0;j<C;j++)
                    s += I[MIN(W-1,MAX(0,x-c+i))][MIN(H-1,MAX(0,y-c+j))]*K[i][j];
            J[x][y] = round(s);
        }
    return NULL;
}

void save_output(char const * path){
    FILE * file;
    int i,j;
    if(!(file = fopen(path,"w")))
        FAILURE_EXIT("cant open/create output file\n")
    fprintf(file, "P2\n%d %d\n255\n", W, H);
    for(i=0;i<W;i++){
        for(j=0;j<H;j++)
            fprintf(file,"%d ",J[i][j]);
        fputs("\n", file);
    }
    fclose(file);
}

void save_times(clock_t * real, struct tms * time_){
    FILE * file;
    if(!(file = fopen("./Times.txt","a")))
        FAILURE_EXIT("cant open/create output file\n")
    fprintf(
        file,
        "Threads %d\nPicture size %dx%d\nFilter size %d\n\
Time\n\tReal %.2lfs\n\tUser %.2lfs\n\tSystem %.2lfs\n\n",
        threads,W,H,C,
        diff(real[0],real[1]),
        diff(time_[0].tms_utime,time_[1].tms_utime),
        diff(time_[0].tms_stime,time_[1].tms_stime)
    );
    fclose(file);
}

int main(int argc, char const *argv[]) {
    int i;
    pthread_t * T;
    clock_t real[2] = {0, 0};
    struct tms other[2];
    if(argc!=5)
        FAILURE_EXIT("usage: %s threads picture filter output\n",argv[0])
    threads = atoi(argv[1]);
    load_picture(argv[2]);
    load_filter(argv[3]);
    J = calloc(W, sizeof(int*));
    for(i=0;i<W;i++)
        J[i] = calloc(H, sizeof(int));
    T = calloc(threads, sizeof(pthread_t));

    real[0] = times(&other[0]);

    for(i=0;i<threads;i++){
        int * arg = calloc(1, sizeof(int));
        *arg = i;
        pthread_create(&T[i], NULL, filter, arg);
    }
    for(i=0;i<threads;i++)
        pthread_join(T[i], NULL);

    real[1] = times(&other[1]);

    save_output(argv[4]);
    save_times(real, other);
    return 0;
}
