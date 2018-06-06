#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <memory.h>
#include <signal.h>

#define PRINT_SHORTER 1
#define PRINT_EQUALS 2
#define PRINT_LONGER 3

#define FAILURE_EXIT(format, ...) {                                            \
    printf(format, ##__VA_ARGS__);                                             \
    exit(-1);}

int P;
int K;
int N;
FILE *text;
int L = 50;
int compare_type;
int print_type;
int nk;

char **BUFFER;
int size = 20;
int c_poz = 0;
int p_poz = 0;
int p_finished = 0;

pthread_t *producers;
pthread_t *consumers;

pthread_mutex_t c_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t p_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t *buffer_mutexes;

pthread_cond_t slots = PTHREAD_COND_INITIALIZER;
pthread_mutex_t slots_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t slot_consumed = PTHREAD_COND_INITIALIZER;
pthread_mutex_t slot_consumed_mutex = PTHREAD_MUTEX_INITIALIZER;

void exit_handler(int signo){
    printf("\n\nEXIT PROCEDURE\n\n");
    int i=0;
    for (i; i < P; ++i)
        pthread_cancel(producers[i]);
    for (i = 0; i < K; ++i)
        pthread_cancel(consumers[i]);

    fclose(text);
    free(producers);
    free(consumers);
    exit(EXIT_SUCCESS);
}

void initialise(){
    BUFFER = malloc(N * sizeof(char*));
    int j=0;
    int i=0;
    for (;j<N;j++)
        BUFFER[j] = NULL;
    buffer_mutexes = malloc(N * sizeof(pthread_mutex_t));

    for (; i < N;i++)
        if(pthread_mutex_init(&buffer_mutexes[i], NULL)!=0)
            FAILURE_EXIT("Error mutex initialisation")

    signal(SIGINT, exit_handler);
    signal(SIGALRM, exit_handler);
}

void compare_and_print(int array_index, char *string){
    int string_length = strlen(string);
    switch (compare_type) {
        case PRINT_SHORTER:
            if (string_length < L)
                printf("At index: %d, founded string with length %d < %d\n%s\n",
                       array_index, string_length, L, string);
            break;
        case PRINT_EQUALS:
            if (string_length == L)
                printf("At index: %d, founded string with length %d == %d\n%s\n",
                       array_index, string_length, L, string);
            break;
        case PRINT_LONGER:
            if (string_length > L)
                printf("At index: %d, founded string with length %d > %d\n%s\n",
                       array_index, string_length, L, string);
            break;
        default:
            break;
    }
/*  FULL OUTPUT :P
    printf("At index: %d, founded string with length %d\n%s\n",
           array_index, string_length, string);
*/
    fflush(stdout);
}

void mutex_lock(pthread_mutex_t *mutex){
    if(pthread_mutex_lock(mutex)!=0)
        FAILURE_EXIT("error locking mutex")
}

void mutex_unlock(pthread_mutex_t *mutex){
    if(pthread_mutex_unlock(mutex)!=0)
        FAILURE_EXIT("error unlocking mutex")
}

void *producer(void *args){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    char buffer[512];
    int str_length;

    while (fgets(buffer, 512, text) != NULL){
        str_length = strlen(buffer);

        mutex_lock(&p_mutex);
        while (BUFFER[p_poz] != NULL)
            pthread_cond_wait(&slot_consumed, &slot_consumed_mutex);

        mutex_lock(&buffer_mutexes[p_poz]);
        BUFFER[p_poz] = malloc((str_length + 1) * sizeof(char));
        strcpy(BUFFER[p_poz], buffer);
        pthread_cond_broadcast(&slots);
        mutex_unlock(&buffer_mutexes[p_poz]);

        p_poz = (p_poz + 1) % size;
        mutex_unlock(&p_mutex);
    }
    p_finished++;
}

void *consumer(void *args){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    while (p_finished == 0) {
        mutex_lock(&c_mutex);
        while (BUFFER[c_poz] == NULL)
            pthread_cond_wait(&slots, &slots_mutex);

        mutex_lock(&buffer_mutexes[c_poz]);
        compare_and_print(c_poz, BUFFER[c_poz]);
        free(BUFFER[c_poz]);
        BUFFER[c_poz] = NULL;
        pthread_cond_broadcast(&slot_consumed);
        mutex_unlock(&buffer_mutexes[c_poz]);

        c_poz = (c_poz + 1) % size;
        mutex_unlock(&c_mutex);

        usleep(1000);
    }

}

void threads(){
    int i=0;
    producers = malloc(P * sizeof(pthread_t));
    consumers = malloc(K * sizeof(pthread_t));

    for (i = 0; i < P; ++i)
        if(pthread_create(&producers[i], NULL, &producer, NULL)!=0)
            FAILURE_EXIT("error creating threads of producer")

    for (i = 0; i < K; ++i)
        if(pthread_create(&consumers[i], NULL, &consumer,  NULL)!=0)
            FAILURE_EXIT("error creating threads of consumer")
}


void wait_for_threads(){
    int i=0;
    for (; i < P; ++i)
        if(pthread_join(producers[i], NULL)!=0)
            FAILURE_EXIT("error joining threads of producer")

    for (i = 0; i < K; ++i)
        if(pthread_join(consumers[i], NULL)!=0)
            FAILURE_EXIT("error joining threads of consumer")

    fclose(text);
    free(producers);
    free(consumers);
}

void configure(){
    FILE *file = fopen("config.txt", "r");
    fscanf(file, "%d", &P);
    fscanf(file, "%d", &K);
    fscanf(file, "%d", &N);

    char buffer[256];
    fscanf(file, "%s", buffer);
    if((text = fopen(buffer, "r"))==NULL)
        FAILURE_EXIT("Cant open text file")

    fscanf(file, "%d", &L);
    fscanf(file, "%d", &compare_type);
    fscanf(file, "%d", &print_type);
    fscanf(file, "%d", &nk);

    fclose(file);
    alarm((unsigned) nk);
}

int main() {
    configure();
    initialise();
    threads();
    wait_for_threads();

    return 0;
}
