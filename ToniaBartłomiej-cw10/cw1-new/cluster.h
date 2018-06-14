#ifndef CLUSTER_H
#define CLUSTER_H

#define PATH_MAX   4096   //from <linux/limits.h>
#define CLIENT_MAX       64

#define FAILURE_EXIT(format, ...) { 										\
	fprintf(stderr, format, ##__VA_ARGS__); 								\
	exit(1);																\
}

#define READ(socket,data,size) {                                            \
    if(read(socket, data, size) != size)                                    \
        FAILURE_EXIT("Could not read message\n");                           \
}

#define WRITE(socket,data,size){                                            \
    if (write(socket, data, size) != size)                                  \
        FAILURE_EXIT("Could not write message\n");                          \
}

void handle_int(int signo) {
    printf("\nClosed with INTERUPT SIGNAL\n");
    exit(0);
}

typedef enum {
    REGISTER,
    UNREGISTER,
    SUCCESS,
	FAILSIZE,
    FAILNAME,
    REQUEST,
    RESULT,
    PING,
    PONG
} type_t;

typedef struct{
    int fd;
    char* name;
	int name_len;
    uint8_t inactive;
} client_t;

typedef struct{
    uint32_t num;
    float result;
} result_t;

typedef struct{
    uint32_t num;
    char op;
    float a;
    float b;
} operation_t;

typedef struct {
    type_t type;
    operation_t operation;
    result_t result;
    client_t client;
} msg_t;
void serialize_msg(msg_t msg, int socket){
	WRITE(socket, &msg.type, sizeof(type_t));
	if(msg.type != PING){
		WRITE(socket, &msg.operation.num, sizeof(uint32_t));
		WRITE(socket, &msg.operation.op, sizeof(char));
		WRITE(socket, &msg.operation.a, sizeof(float));
		WRITE(socket, &msg.operation.b, sizeof(float));

		WRITE(socket, &msg.result.num, sizeof(uint32_t));
		WRITE(socket, &msg.result.result, sizeof(float));

		WRITE(socket, &msg.client.fd, sizeof(int));
		WRITE(socket, &msg.client.name_len, sizeof(int));
		if(msg.client.name_len != 0){
			int s = msg.client.name_len+1;
			WRITE(socket, msg.client.name, s);
		}
	}
}

msg_t deserialize_msg(int socket){
	msg_t msg;
	READ(socket, &msg.type, sizeof(type_t));
	if (msg.type != PING){
		READ(socket, &msg.operation.num, sizeof(uint32_t));
		READ(socket, &msg.operation.op, sizeof(char));
		READ(socket, &msg.operation.a, sizeof(float));
		READ(socket, &msg.operation.b, sizeof(float));

		READ(socket, &msg.result.num, sizeof(uint32_t));
		READ(socket, &msg.result.result, sizeof(float));
		READ(socket, &msg.client.fd, sizeof(int));
		READ(socket, &msg.client.name_len, sizeof(int));
		if(msg.client.name_len != 0){
			int s = msg.client.name_len +1;
			msg.client.name = malloc(s);
			char * tmp = malloc(s);
			READ(socket, tmp, s);
			strcpy(msg.client.name, tmp);
		}
	}
	return msg;
}
#endif
