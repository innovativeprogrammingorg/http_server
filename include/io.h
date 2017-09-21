#ifndef _IO_H_
#define _IO_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "str.h"
#include "base64.h"
#include "vector.h"

uint64_t fread_file(FILE * fd,char** data);
uint64_t sread_file(char* directory,char** data);
uint64_t prepare_media(char* directory,char** data);
uint64_t read_file(int fd,char** data);
char * get_content_type(char* directory);
void respond(int fd, char * data);

#endif