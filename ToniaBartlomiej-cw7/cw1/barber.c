#include "common.h"

int shared_memory;
int semaphore;

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
    if (semaphore != 0) semctl(semaphore, 0, IPC_RMID);
    if (shared_memory != 0) shmctl(shared_memory, IPC_RMID, NULL);
}

void init(int argc, const char** argv){
    int qsize;

    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    atexit(clean);

    if (argc != 2)
        FAILURE_EXIT("Usage:\n %s queue_size\n", argv[0])

    if((qsize = atoi(argv[1]))>MAX_QUEUE_SIZE)
        FAILURE_EXIT("Too big queue size\n")

    key_t key = ftok(PROJECT_PATH,PROJECT_ID);
    if (key == -1){
        FAILURE_EXIT("Couldn't obtain a project key %d\n", errno)
    }

    shared_memory = shmget(key,sizeof(struct Barbershop), S_IRWXU | IPC_CREAT);
    if (shared_memory == -1)
        FAILURE_EXIT("Couldn't create shared memory\n")

    barbershop = shmat(shared_memory, 0, 0);
    if (barbershop == (void*) -1)
        FAILURE_EXIT("Couldn't access shared memory\n")

    barbershop->status = SLEEPING;
    barbershop->qsize = qsize;
    barbershop->count = 0;
    barbershop->client = 0;
    for (int i = 0; i < MAX_QUEUE_SIZE; ++i)
        barbershop->queue[i] = 0;


    if((semaphore = semget(key,1,IPC_CREAT | S_IRWXU)))
        FAILURE_EXIT("Couldn't create semaphore\n")
    semctl(semaphore, 0, SETVAL, 0);


}

int main(int argc, char const *argv[]) {
    init(argc,argv);
    release_semaphore(semaphore);
    while (1) {
        get_semaphore(semaphore);

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
        release_semaphore(semaphore);
    }
    return 0;
}
