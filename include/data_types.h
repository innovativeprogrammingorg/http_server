#ifndef _DATA_TYPES_H_
#define _DATA_TYPES_H_
#include "map.h"
#include <pthread.h>


typedef struct client * Client;

struct client{
	char* ip;
	int port;
	int fd;
	Map cookies;
};


typedef struct request* Request;

struct request{
	Client client;
	char* data;
};

typedef struct thread_wrapper * WThread; 

struct thread_wrapper{
	Request req;
	pthread_t* thread;
};


#endif