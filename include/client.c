#include "client.h"


Client new_client(int fd,char* ip,int port){
	Client out = (Client)malloc(sizeof(struct client));
	out->fd = fd;
	out->ip = ip;
	out->port = port;
	return out;
}