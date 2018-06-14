#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include "signal.h"
#include "cluster.h"
#include "errno.h"

int extern errno;
int SOCKET;
char* name;
connect_t connect_type;


void send_message(type_t type){
    msg_t msg = {
        .type = type,
        .client.name = name,
        .client.name_len = strlen(name),
        .client.conection_type = connect_type
    };
    serialize_msg(msg,SOCKET);
}

void handle_exit(){
    send_message(UNREGISTER);
    shutdown(SOCKET, SHUT_RDWR);
    close(SOCKET);
}

void _register(){
    msg_t msg;
    send_message(REGISTER);
    msg = deserialize_msg(SOCKET);
    switch(msg.type){
        case SUCCESS:
            printf("Logged with SUCCESS\n");
            return;
        case FAILNAME:
            FAILURE_EXIT("Name already in use\n")
        case FAILSIZE:
            FAILURE_EXIT("Too many clients logged to the server\n");
        default:
            return;
    }
}

float calculate (float a, char op, float b){
    switch (op) {
        case '+':
            return a+b;
        case '-':
            return a-b;
        case '*':
            return a*b;
        case '/':
            return a/b;
        default:
            return 0;
    }
}

void handle_request(msg_t msg){
    operation_t operation = msg.operation;
    printf("Calculatiing: %f%c%f",operation.a,operation.op,operation.b);
    fflush(stdout);
    msg_t msg2 = {
        .type = RESULT,
        .client.name = name,
        .client.name_len = strlen(name),
        .result.num = operation.num,
        .result.result = calculate(operation.a,operation.op,operation.b)
    };
    serialize_msg(msg2, SOCKET);
}

int main(int argc, char *argv[]) {
    if (argc != 4)
        FAILURE_EXIT("Usage: %s <Client Name> <WEB / LOCAL> <ADDRESS:PORT / Unix Path>\n", argv[0])

    atexit(handle_exit);
    signal(SIGINT, handle_int);

    name = argv[1];

    if(strcmp(argv[2],"WEB")==0){
        uint32_t ip;
        uint16_t port;
        connect_type = WEB;
        ip = inet_addr(strtok(argv[3],":"));
        port = atoi(strtok(NULL,":"));

        struct sockaddr_in web_address = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = ip,
            .sin_port = htons(port)
        };

        SOCKET = socket(AF_INET, SOCK_DGRAM, 0);
        connect(SOCKET,
            (const struct sockaddr *) &web_address,
            sizeof(web_address));
    }else{
        if(strcmp(argv[2],"LOCAL")==0){
            connect_type = LOCAL;
            char * path = argv[3];
            if (strlen(argv[3]) > PATH_MAX)
                FAILURE_EXIT("Wrong Unix Path\n")

            SOCKET = socket(AF_UNIX, SOCK_DGRAM, 0);
            if (SOCKET < 0)
                FAILURE_EXIT("\nError : Could not create local socket\n");

            struct sockaddr_un addr = {.sun_family = AF_UNIX};
            strcpy(addr.sun_path, path);

            if(bind(SOCKET, (const struct sockaddr *) &addr, sizeof(sa_family_t)) <0){
                printf(strerror(errno));
                FAILURE_EXIT("\nError : Could not bind local socket\n")
            }

            if (connect(SOCKET, (const struct sockaddr *) &addr, sizeof(addr)) == -1)
                FAILURE_EXIT("\nError : Could not connect to local socket\n");


        }else{
            FAILURE_EXIT("Use WEB or LOCAL\n")
        }
    }
    _register();

    msg_t msg;
    while(1){
        msg = deserialize_msg(SOCKET);
        switch(msg.type){
            case REQUEST:
                handle_request(msg);
                break;
            case PING:
                send_message(PONG);
                break;
            default:
                printf("Unknown message type\n");
                break;
        }
    }
    if(!errno)
        fprintf(stderr, "\n%s\n",strerror(errno));
    return errno;
}
