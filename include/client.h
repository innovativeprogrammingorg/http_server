#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <unistd.h>
#include <stdlib.h>

typedef struct client * Client;

struct client{
	char* ip;
	int port;
	int fd;
};


Client new_client(int fd,char* ip,int port);


#endif