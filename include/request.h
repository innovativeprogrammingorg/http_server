#ifndef _REQUEST_H_
#define _REQUEST_H_

#include "str.h"
#include "map.h"
#include "vector.h"
#include "io.h"
#include "header.h"

char* e404_response(Map header);
char* GET_response(Map header);
char* get_requested_directory(Map m);
char* POST_response(Map header);

#endif