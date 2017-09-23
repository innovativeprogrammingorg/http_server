#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include "cookies.h"
#include "vector.h"
#include "map.h"
#include "str.h"

typedef struct client * Client;

struct client{
	char* ip;
	int port;
	int fd;
	Map cookies;
};


Client new_client(int fd,char* ip,int port);

Client find_client_by_ip(Vector clients,char* ip);


#endif