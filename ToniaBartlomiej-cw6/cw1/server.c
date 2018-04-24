#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "utils.h"

#define CLIENTS_MAX 100
#define FAILURE_EXIT(format, ...) { printf(format, ##__VA_ARGS__); exit(-1); }

int busy = 1;
int public_id = -1;
int clients_count = 0;
int clients_data [CLIENTS_MAX][2];
//////////////////HANDLERS///////////////////////////////
void handleSIGINT(int a) {
    FAILURE_EXIT("client: killed by INT\n");
}
void handleEXIT() {
    if(public_id <= -1) return;
    if (msgctl(public_id, IPC_RMID, 0) == -1) {
        printf("server: there was some error deleting queue\n");
    }else{
        printf("server: queue deleted successfully\n");
    }
}
/////////////////////////UTILITIES/////////////////////////
int findclient(Message* m){
    printf("%s\n", "LOOOOL");
    int i;
    for(i = 0; i < CLIENTS_MAX; i++){//mogą być nie zainicjowane pola
        printf("%d  ->   %d\n", clients_data[i][0], m->sender_pid);
        if (clients_data[i][0] == m->sender_pid){
            return clients_data[i][1];
        }
    }
    return -1;
}
void swap(char* a, char* b){
    char buffer;
    buffer = *a;
    *a = *b;
    *b = buffer;
}

char* convert_time(const time_t* time) {
    char* buffer = malloc(sizeof(char) * 30);
    struct tm* time_info;
    time_info = localtime(time);
    strftime(buffer, 20, "%b %d %H:%M", time_info);
    return buffer;
}
////////////////////////MESAGE CONTROL///////////////////////////
void Mtime(Message* m){
    time_t timer;
    int client_queue_id = findclient(m);
    printf("%s\n", "OOOo");
    if(client_queue_id == -1) {
        printf("server: client not found\n");
        return; // to że klient nam znikł to jeszcze nie powód żeby zabić server
    }
    m->type = m->sender_pid; //będziemy potem szukać komunikatów dla siebie
    m->sender_pid = getpid();
    time(&timer);
    sprintf(m->content, "%s", convert_time(&timer));
    if(msgsnd(client_queue_id, m, MESSAGE_SIZE, 0) == -1)
        FAILURE_EXIT("server: TIME response failed\n");
}
void Mcalc(Message* m){
    FILE* calc;
    char command[4096];
    int client_queue_id = findclient(m);
    if(client_queue_id == -1) {
        printf("server: client not found\n");
        return; // to że klient nam znikł to jeszcze nie powód żeby zabić server
    }
    sprintf(command, "echo '%s' | bc", m->content); // #EMEJZING *o*
    calc = popen(command, "r");
    fgets(m->content, CONTENT_SIZE, calc);
    pclose(calc);
    if(msgsnd(client_queue_id, m, MESSAGE_SIZE, 0) == -1)
        FAILURE_EXIT("server: CALC response failed\n");
}
void Mmirror(Message* m){
    int i;
    int client_queue_id = findclient(m);
    if(client_queue_id == -1) {
        printf("server: client not found\n");
        return; // to że klient nam znikł to jeszcze nie powód żeby zabić server
    }
    int len = (int) strlen(m->content);
    if(m->content[len-1] == '\n') len--;
    for(i = 0; i<len/2; i++)
        swap(&m->content[i],&m->content[len-i-1]);
    if(msgsnd(client_queue_id, m, MESSAGE_SIZE, 0) == -1)
        FAILURE_EXIT("server: MIRROR response failed\n");
}
void Mlogin(Message* m){
    key_t clientkey;
    int client_queue_id;
    if(sscanf(m->content, "%d", &clientkey) < 0)
        FAILURE_EXIT("server: reading client_key failed\n");
    if((client_queue_id =  msgget(clientkey, 0)) == -1)
        FAILURE_EXIT("server: reading client_queue_id failed\n");
    if(clients_count > CLIENTS_MAX - 1){ //dlaczego -1
        printf("Maximum amount of clients reached!\n");
        sprintf(m->content, "%d", -1);
    }else{
        clients_data[clients_count][0] = m->sender_pid;
        clients_data[clients_count++][1] = client_queue_id;
        sprintf(m->content, "%d", clients_count - 1); //dlaczego -1
    }
    m->type = INIT;
    m->sender_pid = getpid();
    printf("CLENT Q ID: %d\n", client_queue_id);
    if(msgsnd(client_queue_id, m, MESSAGE_SIZE, 0) == -1)
        FAILURE_EXIT("server: LOGIN response failed\n");
}
void executeMessage(Message* m){
    switch (m->type) {
        case LOGIN:
            Mlogin(m);
            break;
        case MIRROR:
            Mmirror(m);
            break;
        case CALC:
            Mcalc(m);
           break;
        case TIME:
            Mtime(m);
            break;
        case END:
            busy = 0; //stops server
            break;
    }
}
int main(int argc, char const *argv[]) {
    char * path;
    key_t key;
    Message message;
    struct msqid_ds currentState;
    if (atexit(handleEXIT) == -1)
        FAILURE_EXIT("server: failed to register atexit()\n")
    if(signal(SIGINT, handleSIGINT) == SIG_ERR)
        FAILURE_EXIT("server: failed to register INT handler\n")
    if((path = getenv("HOME")) == 0)
        FAILURE_EXIT("server: failed to obtain value of $HOME\n");
    if((key= ftok(path, PROJECT_ID)) == -1)
        FAILURE_EXIT("server: failed to generate public_key\n");
    if((public_id = msgget(key, IPC_CREAT | IPC_EXCL | 0666))== -1)
        FAILURE_EXIT("server: failed to create public queue\n");
    while(1) {
        if(busy == 0) {
            if(msgctl(public_id, IPC_STAT, &currentState) == -1)
                FAILURE_EXIT("server: getting current state of failed\n");
            if(currentState.msg_qnum == 0) break;
        }
        if(msgrcv(public_id, &message, MESSAGE_SIZE, 0, 0) < 0)
            FAILURE_EXIT("server: failed to receive message\n");
        executeMessage(&message);
    }
    return 0;
}
