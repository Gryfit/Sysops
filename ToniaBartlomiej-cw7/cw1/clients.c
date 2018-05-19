#include "common.h"

enum Client_status status;
int shared_memory;
int semaphore;

void init(){
    key_t key = ftok(PROJECT_PATH,PROJECT_ID);
    if (key == -1)
        FAILURE_EXIT("Couldn't obtain a project key\n")

    shared_memory = shmget(key, sizeof(struct Barbershop), 0);
    if (shared_memory == -1)
        FAILURE_EXIT("Couldn't create shared memory\n")
    barbershop = shmat(shared_memory, 0, 0);
    if (barbershop == (void*) -1)
        FAILURE_EXIT("Couldn't access shared memory\n")
    semaphore = semget(key, 0, 0);
    if (semaphore == -1)
        FAILURE_EXIT("Couldn't create semaphore\n")
}

void sit(){
    pid_t pid = getpid();
    switch (status) {
        case INVITED:
            qpop();
            break;
        case NEWCOMER:
            while (1) {
                release_semaphore(semaphore);
                get_semaphore(semaphore);
                if(barbershop->status == READY)
                    break;
            }
            status = INVITED;
            break;
        default:
            break;
    }
    barbershop->client = pid;
    printf("%lo\t#%i:\ttook place in the chair\n", get_time(), pid);
}
void run(int S){
    pid_t pid = getpid();
    int cuts = 0;
    while (cuts < S) {
        status = NEWCOMER;
        get_semaphore(semaphore);
        if(barbershop->status == SLEEPING){
            printf("%lo\t#%i:\twoke up the barber\n", get_time(), pid);
            barbershop->status = AWOKEN;
            sit();
            barbershop->status = BUSY;
        }else if(!qfull()){
            qpush(pid);
            printf("%lo\t#%i:\tenter the queue\n", get_time(), pid);
        }else{
            printf("%lo\t#%i:\tcant find place in the queue\n", get_time(), pid);
            release_semaphore(semaphore);
            return;
        }
        release_semaphore(semaphore);
        while (status==NEWCOMER) {
            get_semaphore(semaphore);
            if(barbershop->client == pid){
                status = INVITED;
                sit();
                barbershop->status = BUSY;
            }
            release_semaphore(semaphore);
        }
        while(status!=SHAVED){
            get_semaphore(semaphore);
            if(barbershop->client != pid){
                status = SHAVED;
                printf("%lo\t#%i:\tshaved\n", get_time(), pid);
                barbershop->status = IDLE;
                cuts++;
            }
            release_semaphore(semaphore);
        }
    }
    printf("%lo\t#%i:\tleft barbershop after %i cuts\n", get_time(), pid, cuts);
}

int main(int argc, char const *argv[]) {
    int clients;
    int cuts;
    int i = 0;
    if(argc != 3)
        FAILURE_EXIT("Usage: %s number_of_clients number_of_cuts\n", argv[0])
    clients = atoi(argv[1]);
    cuts = atoi(argv[2]);
    init();
    for(;i<clients;i++){
        if(!fork()){
            run(cuts);
            exit(0);
        }
    }
    while (wait(0)) {
        if (errno!=ECHILD) break;
    }
    return 0;
}
