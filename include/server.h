#ifndef _SERVER_H_
#define _SERVER_H_
#include <stdio.h>
#include <string.h>   
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   
#include <arpa/inet.h>    
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <netdb.h> /* struct hostent, gethostbyname */
#include "str.h"
#include "vector.h"

#ifndef TRUE
#define TRUE   1
#endif
#ifndef FALSE
#define FALSE  0
#endif
#define BUFFER_SIZE 20000 

int expo(int n,int i);

#endif