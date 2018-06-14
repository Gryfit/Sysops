#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include "cluster.h"
#include "signal.h"
#include "errno.h"


int extern errno;

int web_socket;
int local_socket;
int epoll;
char * path;

pthread_t ping;
pthread_t command;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

client_t clients[CLIENT_MAX];

int client_num = 0;
int operation_num = 0;


void handle_exit() {
    pthread_cancel(ping);
    pthread_cancel(command);

    close(web_socket);

    close(local_socket);
    unlink(path);

    close(epoll);
}

void handle_connection(int socket) {
    int client;
    client = accept(socket, NULL, NULL);
    struct epoll_event event = {
        .events = EPOLLIN | EPOLLPRI,
        .data.fd = client
    };
    epoll_ctl(epoll, EPOLL_CTL_ADD, client, &event);
}

int client_index(char * name){
    int i;
    for (i=0;i<client_num;i++)
        if (strcmp(clients[i].name,name)==0)
            return i;
    return -1;
}

void remove_socket(int socket){
    epoll_ctl(epoll, EPOLL_CTL_DEL, socket, NULL);
    shutdown(socket, SHUT_RDWR);
    close(socket);
}

void register_client(msg_t msg, int socket){
    pthread_mutex_lock(&clients_mutex);
    if(client_num == CLIENT_MAX){
        msg.type = FAILSIZE;
        serialize_msg(msg, socket);
        remove_socket(socket);
    } else{
        int exists = client_index(msg.client.name);
        if(exists >= 0){
            msg.type = FAILNAME;
            serialize_msg(msg,socket);
            remove_socket(socket);
        } else{
            clients[client_num].fd = socket;
            clients[client_num].name = malloc(msg.client.name_len+1);
            clients[client_num].inactive = 0;
            strcpy(clients[client_num++].name, msg.client.name);
            msg.type = SUCCESS;
            printf("Registered Client %s \n",msg.client.name);
            serialize_msg(msg,socket);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int i) {
    remove_socket(clients[i].fd);
    free(clients[i].name);
    client_num--;
    while(i < client_num)
        clients[i] = clients[++i];
}

void unregister_client(msg_t msg){
    pthread_mutex_lock(&clients_mutex);
    int i = client_index(msg.client.name);
    if(i >= 0){
        remove_client(i);
        printf("Client \"%s\" unregistered\n", msg.client.name);
    }
    pthread_mutex_unlock(&clients_mutex);
}

void handle_message(int socket){
    msg_t msg;
    msg = deserialize_msg(socket);
    switch(msg.type){
        case REGISTER:
            register_client(msg, socket);
            break;
        case UNREGISTER:
            unregister_client(msg);
            break;
        case RESULT:
            printf("Client \"%s\" calculated operation [%d]. Result: %lf\n",   \
                msg.client.name, msg.result.num, msg.result.result);
            break;
        case PONG:
            pthread_mutex_lock(&clients_mutex);
            int i = client_index(msg.client.name);
            if (i >= 0)
                clients[i].inactive--;
            pthread_mutex_unlock(&clients_mutex);
            break;
        default:
            printf("Unknown message type\n");
            break;
    }
}

void *ping_routine(void *arg) {
    int i;
    while (1) {
        pthread_mutex_lock(&clients_mutex);
        for (i = 0; i < client_num; i++) {
            if (clients[i].inactive != 0) {
                printf("Client \"%s\" do not respond. Removing from registered clients\n",
                    clients[i].name);
                remove_client(i--);
            } else {
                msg_t msg = {.type = PING};
                serialize_msg(msg, clients[i].fd);
                clients[i].inactive++;
            }
        }
        pthread_mutex_unlock(&clients_mutex);
        sleep(5);
    }
}

void *command_routine(void *arg) {
    operation_t operation;
    msg_t msg;
    char buffer[256];
    int i;
    srand((unsigned int) time(NULL));

    while (1) {
        printf("Enter command: \n");
        fgets(buffer, 256, stdin);
        if (sscanf(buffer, "%f%c%f\n", &operation.a, &operation.op, &operation.b) != 3)
            printf("Wrong command format\n");
        if (operation.op != '+' && operation.op != '-' && operation.op != '*' \
            && operation.op != '/')
            printf("Wrong operator (%c)\n", operation.op);
        operation.num = ++operation_num;

        pthread_mutex_lock(&clients_mutex);
        if (client_num == 0)
            FAILURE_EXIT("No Clients connected to server\n");
        i = rand() % client_num;
        msg_t msg = {
            .type = REQUEST,
            .operation = operation
        };
        serialize_msg(msg,clients[i].fd);
        printf("Command %d: %lf %c %lf Has been sent to client \"%s\"\n",
                   operation.num , operation.a, operation.op, operation.b,
                   clients[i].name);
        pthread_mutex_unlock(&clients_mutex);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3)
        FAILURE_EXIT("Usage: %s <port number> <unix path>\n", argv[0]);

    atexit(handle_exit);
    signal(SIGINT, handle_int);

    path = argv[2];
    uint16_t port = atoi(argv[1]);
    if (strlen(path) > PATH_MAX)
        FAILURE_EXIT("Wrong Unix Path\n");

    struct sockaddr_in web_address = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(port)
    };
    web_socket = socket(AF_INET, SOCK_STREAM, 0);
    bind(web_socket,
        (const struct sockaddr *) &web_address,
        sizeof(web_address));
    listen(web_socket, 64);

    struct sockaddr_un local_address = {.sun_family = AF_UNIX};
    snprintf(local_address.sun_path, PATH_MAX, "%s", argv[2]);
    local_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    bind(local_socket,
        (const struct sockaddr *) &local_address,
        sizeof(local_address));
    listen(local_socket, 64);

    struct epoll_event event;
    epoll = epoll_create1(0);
    event.data.fd = -web_socket;
    epoll_ctl(epoll, EPOLL_CTL_ADD, web_socket, &event);
    event.data.fd = -local_socket;
    epoll_ctl(epoll, EPOLL_CTL_ADD, local_socket, &event);

    pthread_create(&ping, NULL, ping_routine, NULL);
    pthread_create(&command, NULL, command_routine, NULL);
    while (1) {
        epoll_wait(epoll, &event, 1, -1);
        if (event.data.fd < 0)
            handle_connection(-event.data.fd);
        else
            handle_message(event.data.fd);
    }

    if(!errno)
        fprintf(stderr, "\n%s\n",strerror(errno));
    return errno;
}
