#include "cookies.h"

Cookie new_cookie(char* value,time_t created,time_t exp){
	Cookie out = (Cookie)malloc(sizeof(struct cookie));
	out->value = value;
	out->created = (created) ? created : time(NULL);
	out->expires = (exp) ? exp : time(NULL) + COOKIES_EXP;
	return out;
}

uint8_t store_cookie(char* ip, char* key, char* value){
	char* loc = concat(COOKIES_DIR,ip,FALSE);
	return 0;
}

Map load_cookies(char* ip){
	return NULL;
}

uint8_t parse_cookies(char* ip,Map header){
	return 0;
}

char* get_cookie(char* ip, char* key){
	return NULL;
}




