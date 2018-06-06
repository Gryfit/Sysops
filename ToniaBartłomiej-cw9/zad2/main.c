#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <memory.h>
#include <signal.h>
#include <semaphore.h>

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


sem_t consumer_sem;
sem_t *buffer_semaphores;
sem_t free_places;
sem_t full_places;

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
    sem_init(&consumer_sem, 0, 1);
    sem_init(&free_places, 0, N);
    sem_init(&full_places, 0, 0);
    int i;
    BUFFER = malloc(N * sizeof(char*));
    for (i = 0; i < N; ++i)
        BUFFER[i] = NULL;

    buffer_semaphores = malloc(N * sizeof(sem_t));

    for (i = 0; i < N; ++i)
        if(sem_init(&buffer_semaphores[i], 0, 1) != 0)
            FAILURE_EXIT("error: semaphore initialisation")

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
 // FULL OUTPUT :P
//    printf("At index: %d, founded string with length %d\n%s\n",
//           array_index, string_length, string);

    fflush(stdout);
}


void *producer(void *args){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    char buffer[512];
    int str_length;

    while (fgets(buffer, 512, text) != NULL){
        str_length = strlen(buffer);
        sem_wait(&free_places);
        sem_wait(&buffer_semaphores[p_poz]);

        BUFFER[p_poz] = malloc((str_length + 1) * sizeof(char));
        strcpy(BUFFER[p_poz], buffer);

        sem_post(&full_places);
        sem_post(&buffer_semaphores[p_poz]);
        p_poz = (p_poz + 1) % size;
    }
    p_finished ++;
}

void *consumer(void *args){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while (p_finished == 0) {
        sem_wait(&consumer_sem);
        sem_wait(&full_places);
        sem_wait(&buffer_semaphores[c_poz]);

        compare_and_print(c_poz, BUFFER[c_poz]);
        free(BUFFER[c_poz]);
        BUFFER[c_poz] = NULL;

        sem_post(&free_places);
        sem_post(&buffer_semaphores[c_poz]);
        c_poz = (c_poz + 1) % size;
        sem_post(&consumer_sem);
        usleep(1000);
    }
}

void threads(){
    int i;
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
