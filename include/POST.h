#ifndef _POST_H_
#define _POST_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

char* process_through_php(Map _POST,char* dir);


#endif