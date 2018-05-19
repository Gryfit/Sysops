#ifndef __COMMON_H__
#define __COMMON_H__

#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <zconf.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
#include <semaphore.h>
// PRINTING UTILITIES //////////////////////////////////////////////////////////

#define FAILURE_EXIT(format, ...) {                                            \
    printf(format, ##__VA_ARGS__);                                             \
    exit(-1);}

// PROJECT CONSTS //////////////////////////////////////////////////////////////

#define PROJECT_PATH "/bSHOP"

#define MAX_QUEUE_SIZE 64

// DATA ////////////////////////////////////////////////////////////////////////

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
