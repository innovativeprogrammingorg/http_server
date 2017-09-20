#include "map.h"

static Map createMap(){
	Map out = NULL;
	out->head = NULL;
	return out;
}
static Meta createMeta(){
	Meta out = (Meta)malloc(sizeof(struct meta));
	out->key = NULL;
	out->value = NULL;
	out->next = NULL;
	out->prev = NULL;
	is_array = FALSE;
	return out;
}

uint8_t map_has_key(Map m, char* key){
	Meta tmp = m->head;
	while(tmp != NULL){
		if(strcompare(key,tmp->key)){
			return TRUE;
		}
		tmp = tmp->next;
	}
	return FALSE;
}

void map_add(Map* m, char* key, void* value){
	Meta add = createMeta();
	Meta tmp;
	add->data = data;
	add->key = (char*)calloc(sizeof(char),strlen(key)+1);
	memcpy(add->key,key,strlen(key)+1);
	if(m==NULL||*m==NULL){
		*m = createMap();
		m->head = add;
		return;
	}

	tmp = (*m)->head;
	while(tmp->next != NULL){
		tmp = tmp->next;
	}
	tmp->next = add;
	add->prev = tmp;
}

void map_add_array(Map* m, char* key, void* value){
	Meta add = createMeta();
	Meta tmp;
	add->data = data;
	add->key = (char*)calloc(sizeof(char),strlen(key)+1);
	add->is_array = true;
	memcpy(add->key,key,strlen(key)+1);
	if(m==NULL||*m==NULL){
		*m = createMap();
		m->head = add;
		return;
	}

	tmp = (*m)->head;
	while(tmp->next != NULL){
		tmp = tmp->next;
	}
	tmp->next = add;
	add->prev = tmp;
}

void map_clean(Map m){
	Meta tmp = m->head;
	Meta clear = NULL;
	if(v==NULL){
		return;
	}
	
	while(tmp->next != NULL){
		tmp = tmp->next;
		clear = tmp->prev;
		clear->next = NULL;
		clear->prev = NULL;
		free(clear->data);
		free(clear->key);
		clear->data = NULL;
		clear->key = NULL;
		free(clear);
	}
	free(tmp);
	free(m);
}

void map_remove(Map* m, char* key){
	if(m==NULL || *m == NULL){
		return;
	}
	Meta tmp = (*m)->head;
	Meta p;
	int i = 0;
	while(!strcompare(tmp->key,key) && tmp != NULL){
		tmp = tmp->next;
		i++;
	}
	if(tmp==NULL){
		puts("Error, key not found");
		return;
	}
	p = tmp->prev;
	p->next = tmp->next;
	tmp->next->prev = p;
	free(tmp);
}

void* map_value_at(Map *m, char* key){
	Vector tmp = v;
	while(!strcompare(key,tmp->key) && tmp != NULL){
		tmp = tmp->next;
	}
	return tmp->data;
}

size_t map_size(Map m){
	Meta tmp = m->head;
	size_t size = 0;
	while(tmp != NULL){
		size++;
		tmp = tmp->next;
	}
	return size;
}
char** map_get_keys(Map m){
	size_t size = map_size(m);
	char** keys = (char **)calloc(sizeof(char*),size+1);
	keys[size] = NULL;
	int i = 0;
	char* key;
	Meta tmp = m->head;
	for(i = 0;i<size;i++){
		key = tmp->key;
		keys[i] = (char*)calloc(sizeof(char*),strlen(key)+1);
		strcpy(keys[i],key);
		tmp = tmp->next;
	}
	return keys;
}