#ifndef CLUSTER_H
#define CLUSTER_H

#define PATH_MAX   108   //from <linux/limits.h>
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
#define SENDTO(socket, msg, size, addr, socklen){							\
	if (sendto(socket, msg, size, 0, addr, socklen) != size)				\
		FAILURE_EXIT("CANT SEND VIA SENDTO")								\
}
#define RECEIVEFROM(socket, msg, size, addr, socklen){						\
	if(recvfrom(socket, msg, size, 0, addr, socklen) != size)				\
		FAILURE_EXIT("Could not receive message\n")							\
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

typedef enum {
	LOCAL,
	WEB
} connect_t;

typedef struct{
    int fd;
	connect_t conection_type;
    char* name;
	int name_len;
    uint8_t inactive;			//nieserializowane
	struct sockaddr* sockaddr;	//nieserializowane
	socklen_t socklen;			//nieserializowane
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

		WRITE(socket, &msg.client.conection_type, sizeof(connect_t));
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

		READ(socket, &msg.client.conection_type, sizeof(connect_t));
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
void serialize_msg_via_sendto(int socket, msg_t msg, struct sockaddr* addr, socklen_t socklen){
	int errno;
	if (sendto(socket, &msg.type, sizeof(type_t), 0, addr, socklen) != sizeof(type_t)){
		puts("CANT SEND VIA SENDTO");
		puts(strerror(errno));
		exit(0);
	}
	//SENDTO(socket, &msg.type, sizeof(type_t), addr, socklen);
	if(msg.type != PING){

		SENDTO(socket, &msg.operation.num, sizeof(uint32_t), addr, socklen);
		SENDTO(socket, &msg.operation.op, sizeof(char), addr, socklen);
		SENDTO(socket, &msg.operation.a, sizeof(float), addr, socklen);
		SENDTO(socket, &msg.operation.b, sizeof(float), addr, socklen);

		SENDTO(socket, &msg.result.num, sizeof(uint32_t), addr, socklen);
		SENDTO(socket, &msg.result.result, sizeof(float), addr, socklen);

		SENDTO(socket, &msg.client.conection_type, sizeof(connect_t), addr, socklen);
		SENDTO(socket, &msg.client.fd, sizeof(int), addr, socklen);
		SENDTO(socket, &msg.client.name_len, sizeof(int), addr, socklen);

		if(msg.client.name_len != 0){
			int s = msg.client.name_len+1;
			SENDTO(socket, msg.client.name, s, addr, socklen);
		}
	}
}
msg_t deserialize_msg_via_receivefrom(int socket, struct sockaddr *addr, socklen_t* socklen){
	msg_t msg;
	RECEIVEFROM(socket, &msg.type, sizeof(type_t), addr, socklen);
	if (msg.type != PING){
		RECEIVEFROM(socket, &msg.operation.num, sizeof(uint32_t), addr, socklen)
		RECEIVEFROM(socket, &msg.operation.op, sizeof(char), addr, socklen)
		RECEIVEFROM(socket, &msg.operation.a, sizeof(float), addr, socklen)
		RECEIVEFROM(socket, &msg.operation.b, sizeof(float), addr, socklen)

		RECEIVEFROM(socket, &msg.result.num, sizeof(uint32_t), addr, socklen)
		RECEIVEFROM(socket, &msg.result.result, sizeof(float), addr, socklen)

		RECEIVEFROM(socket, &msg.client.conection_type, sizeof(connect_t), addr, socklen);
		RECEIVEFROM(socket, &msg.client.fd, sizeof(int), addr, socklen)
		RECEIVEFROM(socket, &msg.client.name_len, sizeof(int), addr, socklen)
		if(msg.client.name_len != 0){
			int s = msg.client.name_len +1;
			msg.client.name = malloc(s);
			char * tmp = malloc(s);
			RECEIVEFROM(socket, tmp, s, addr, socklen)
			strcpy(msg.client.name, tmp);
		}
	}
	return msg;
}

#endif
