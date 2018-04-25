#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include "utilities.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#define FAILURE_EXIT(format, ...) { printf(format, ##__VA_ARGS__);  mq_close(public_id); mq_close(private_id); mq_unlink(key); exit(-1);}

int session_id = -1;
int public_id = -1;
int private_id = -1;
int busy =1;
char key [64];
struct sigevent notif;

// UTILS ///////////////////////////////////////////////////////////////////////
void Mlogin(char* m){
    int i = atoi(m+1);
    if(i==-1){
        printf("CANT REGISTER CLIENT\n");
    }else{
        printf("CLIENT REGISTERED WITH CODE: %s\n", m+1);
        session_id = atoi(m+1);
    }
}

void Mmirror(char *m) {
    printf("REVERSED STRING: %s\n", m+2);
}
void Mcalc(char *m) {
    printf("CALCULATED EXPR: %s\n", m+2);
}
void Mtime(char *m) {
    printf("TIME: %s\n", m+2);
}

void executeMessage(char* m){
    printf("GOT MSG: %s\n",m );
    switch (m[0]) {
        case END:
            mq_close(public_id);
            mq_close(private_id);
            mq_unlink(key);
            kill(getpid(),SIGKILL);
            break;
        case INIT:
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
    }
}


/////////////////////////////HANDLERS////////////////////////////////////////////
void handleSIGINT(int a) {
    FAILURE_EXIT("client: killed by INT\n");
}

void handleEXIT() {
    mq_close(public_id);
    mq_close(private_id);
    mq_unlink(key);
}

void handleSIGUSR2(){
    char in_buffer0 [MSG_BUFFER_SIZE];

    if (mq_receive (private_id, in_buffer0, MSG_BUFFER_SIZE, NULL) == -1)
        FAILURE_EXIT("Server: mq_receive")
    printf ("Server: message received: %s\n", in_buffer0);

    executeMessage(in_buffer0);

    mq_notify(private_id,&notif);
}

// MAIN ////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[]) {
    sprintf (key, "/c%d", getpid());
    char register_msg[66];
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    if (atexit(handleEXIT) == -1)
        FAILURE_EXIT("client: failed to register atexit()\n")
    if(signal(SIGINT, handleSIGINT) == SIG_ERR)
        FAILURE_EXIT("client: failed to register INT handler\n")
    if(signal(SIGUSR2, handleSIGUSR2) == SIG_ERR)
        FAILURE_EXIT("client: failed to register USR1 handler\n")
    if ((private_id = mq_open (key, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1)
        FAILURE_EXIT("CLIENT: mq_open (private)")
    if ((public_id = mq_open (SERVER_QUEUE_NAME, O_WRONLY)) == -1)
        FAILURE_EXIT ("Client: mq_open (server)");
    //register client
    sprintf (register_msg, "%c%s", LOGIN, key);
    printf("%s\n", register_msg);
    if (mq_send (public_id, register_msg, strlen (register_msg)+1, 0) == -1) {
        perror ("Client: Not able to send message to server");
    }
    notif.sigev_signo = SIGUSR2;
    mq_notify(private_id,&notif);
    char command [20];
    char MSG [66];
    char content [240];
    while(busy) {
        printf("client: provide command\n");
        if(fgets(command, 20, stdin) == 0)
            printf("client: error reading your command\n");

        if(command[strlen(command)-1] == '\n')
            command[strlen(command)-1] = 0;

        if(strcmp(command, "MIRROR") == 0){
            printf("client: enter line to mirror:\n");
            if(fgets(content, 240, stdin) == 0) {
                printf("client: too long input\n");
            }
            sprintf(MSG, "%c%d%s",MIRROR,session_id,content);
            if (mq_send (public_id, MSG, strlen (MSG)+1, 0) == -1) {
                perror ("Client: Not able to send message to server");
            }
        }
        if (strcmp(command, "CALC") == 0){
            printf("Enter expression to calculate: ");
            if(fgets(content, 240, stdin) == 0) {
                printf("Too many characters!\n");
            }
            sprintf(MSG, "%c%d%s",CALC,session_id,content);
            if (mq_send (public_id, MSG, strlen (MSG)+1, 0) == -1) {
                perror ("Client: Not able to send message to server");
            }
        }
        if (strcmp(command, "TIME") == 0){
            sprintf(MSG, "%c%d",TIME,session_id);
            if (mq_send (public_id, MSG, strlen (MSG)+1, 0) == -1) {
                perror ("Client: Not able to send message to server");
            }
        }
        if (strcmp(command, "END") == 0){
            sprintf(MSG, "%c%d",END,session_id);
            if (mq_send (public_id, MSG, strlen (MSG)+1, 0) == -1) {
                perror ("Client: Not able to send message to server");
            }
            busy =0;
        }
    }
    mq_close(public_id);
    mq_close(private_id);
    mq_unlink(key);
    return 0;
}
