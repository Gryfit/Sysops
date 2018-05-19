#include "common.h"

int shared_memory_fd;
sem_t* semaphore;

void handler(int a){
    printf("\nClosing Barbersop\n");
    exit(0);
}

void invite(){
    pid_t client = barbershop->queue[0];
    barbershop->client = client;
    printf("%lo\tbarber:\tinvited client %i\n", get_time(), client);
}

void shave(){
    printf("%lo\tbarber:\tshaved client %i\n",get_time(),
               barbershop->client);
    barbershop->client = 0;
}

void clean(){
    if (semaphore != 0) sem_unlink(PROJECT_PATH);
    if (shared_memory_fd != 0) shm_unlink(PROJECT_PATH);
}

void init(int qsize){
    shared_memory_fd = shm_open(PROJECT_PATH, O_RDWR | O_CREAT | O_EXCL,
            S_IRWXU | S_IRWXG);
    if (shared_memory_fd == -1)
        FAILURE_EXIT("Couldn't create shared memory\n")
    if (ftruncate(shared_memory_fd, sizeof(*barbershop)) == -1)
        FAILURE_EXIT("Failed truncating file\n");
    barbershop = mmap(NULL, sizeof(*barbershop), PROT_READ | PROT_WRITE,
            MAP_SHARED, shared_memory_fd, 0);
    if (barbershop == MAP_FAILED) //(void*) -1
        FAILURE_EXIT("Couldn't access shared memory\n")
    semaphore = sem_open(PROJECT_PATH, O_WRONLY | O_CREAT | O_EXCL,
            S_IRWXU | S_IRWXG,0);
    if (semaphore == SEM_FAILED)
        FAILURE_EXIT("Couldn't create semaphore\n")
    barbershop->status = SLEEPING;
    barbershop->qsize = qsize;
    barbershop->count = 0;
    barbershop->client = 0;
    for (int i = 0; i < MAX_QUEUE_SIZE; ++i)
        barbershop->queue[i] = 0;
}

int main(int argc, char const *argv[]) {
    int qsize;
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    atexit(clean);
    if (argc != 2)
        FAILURE_EXIT("Usage:\n %s queue_size\n", argv[0])
    if((qsize = atoi(argv[1]))>MAX_QUEUE_SIZE)
        FAILURE_EXIT("Too big queue size\n")

    init(qsize);
    
    sem_post(semaphore);
    while (1) {
        sem_wait(semaphore);

        switch (barbershop->status) {
            case IDLE:
                if(qempty()){
                    printf("%lo\tbarber:\tbarber fell asleep\n", get_time());
                    barbershop->status = SLEEPING;
                }else{
                    invite();
                    barbershop->status = READY;
                }
                break;
            case AWOKEN:
                printf("%lo barber: woke up\n", get_time());
                barbershop->status = READY;
                break;
            case BUSY:
                shave();
                barbershop->status = READY;
                break;
            default:
                break;
        }
        sem_post(semaphore);
    }
    return 0;
}
