#ifndef _MAP_H_
#define _MAP_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"


typedef struct meta * Meta;

struct meta{
	char* key;
	void* data;
	Meta next;
	Meta prev;
	uint8_t is_array;
};
/**
 * Map is used to connect keys to values, keys must be char*, values are of type void*
 */
typedef struct map * Map;

struct map{
	Meta head;
};

uint8_t map_has_key(Map m, char* key);
void map_add(Map* m, char* key, void* value);
void map_add_array(Map* m, char* key, void* value);
void map_remove(Map* m, char* key);
void* map_value_at(Map *m, char* key);
char** map_get_keys(Map m);
size_t map_size(Map m);
void map_clean(Map m);

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif