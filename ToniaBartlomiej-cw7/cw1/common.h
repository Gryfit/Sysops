#ifndef __COMMON_H__
#define __COMMON_H__

#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <zconf.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ipc.h>
#include <fcntl.h>

#define FAILURE_EXIT(format, ...) {                                            \
    printf(format, ##__VA_ARGS__);                                             \
    exit(-1);}


#define PROJECT_PATH getenv("HOME")
#define PROJECT_ID 123
#define MAX_QUEUE_SIZE 64


enum Barber_status {
    SLEEPING,
    AWOKEN, //to comunicate when its waking
    READY,
    IDLE, //to decide between beeing ready and faling asleep
    BUSY
};

enum Client_status {
    NEWCOMER,
    INVITED,
    SHAVED
};

struct Barbershop {
    enum Barber_status status;
    int count;
    int qsize;
    pid_t client;
    pid_t queue[MAX_QUEUE_SIZE];
} *barbershop;


long get_time() {
    struct timespec buffer;
    clock_gettime(CLOCK_MONOTONIC, &buffer);
    return buffer.tv_nsec / 1000;
}


void get_semaphore(int semaphore) {
    struct sembuf sr;
    sr.sem_num = 0;
    sr.sem_op = -1;
    sr.sem_flg = 0;

    semop(semaphore, &sr, 1);
}

void release_semaphore(int semaphore) {
    struct sembuf sr;
    sr.sem_num = 0;
    sr.sem_op = 1;
    sr.sem_flg = 0;

    semop(semaphore, &sr, 1);
}
// QUEUE

int qfull() {
    return barbershop->count < barbershop->qsize? 0:1;
}

int qempty() {
    return barbershop->count == 0? 1:0;
}

void qpush(pid_t pid) {
    barbershop->queue[barbershop->count ++] = pid;
}

void qpop() {
    int i =0;
    for (; i < barbershop->count - 1; i++) {
        barbershop->queue[i] = barbershop->queue[i + 1];
    }
    barbershop->queue[barbershop->count - 1] = 0;
    barbershop->count -= 1;
}

#endif
