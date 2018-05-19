
#include "common.h"

enum Client_status status;
int shared_memory_fd;
sem_t* semaphore;

void init(){
    shared_memory_fd = shm_open(PROJECT_PATH, O_RDWR, S_IRWXU | S_IRWXG);
    if (shared_memory_fd == -1)
        FAILURE_EXIT("Couldn't create shared memory\n")
    if (ftruncate(shared_memory_fd, sizeof(*barbershop)) == -1)
        FAILURE_EXIT("Failed truncating file\n");
    barbershop = mmap(NULL, sizeof(*barbershop), PROT_READ | PROT_WRITE,
            MAP_SHARED, shared_memory_fd, 0);
    if (barbershop == MAP_FAILED)
        FAILURE_EXIT("Couldn't access shared memory\n")
    semaphore = sem_open(PROJECT_PATH, O_WRONLY, S_IRWXU | S_IRWXG, 0);
    if (semaphore == SEM_FAILED)
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
                sem_post(semaphore);
                sem_wait(semaphore);
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
        sem_wait(semaphore);
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
            sem_post(semaphore);
            return;
        }
        sem_post(semaphore);
        while (status==NEWCOMER) {
            sem_wait(semaphore);
            if(barbershop->client == pid){
                status = INVITED;
                sit();
                barbershop->status = BUSY;
            }
            sem_post(semaphore);
        }
        while(status!=SHAVED){
            sem_wait(semaphore);
            if(barbershop->client != pid){
                status = SHAVED;
                printf("%lo\t#%i:\tshaved\n", get_time(), pid);
                barbershop->status = IDLE;
                cuts++;
            }
            sem_post(semaphore);
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
