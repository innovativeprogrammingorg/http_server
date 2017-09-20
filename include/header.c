
#include "header.h"
Map parse_HTTP_Header(char * header){
	Vector parameters = explode("\r\n",header);
	size_t length = vector_length(parameters);
	int i = 0;
	Map out = NULL;
	Vector tmp = NULL;
	char* key;
	char* value;
	void* tmp;
	Vector key_value_pair = NULL;
	Vector values = NULL;
	for(i = 0;i<length;i++){
		if(i == 0){
			key = "REQUEST";
			tmp = (char*)vector_get(parameters,i);
			value = malloc(sizeof(char*)*(strlen(tmp)+1));
			strcpy((char*)value,tmp);
		}else{
			key_value_pair = explode(":",vector_get(parameters,i));
			key = (char*)vector_get(key_value_pair,0);
			value = (char*)malloc(sizeof(char*)*(strlen((char*)vector_get(key_value_pair,1)) + 1));
			strcpy(value,(char*)vector_get(key_value_pair,1));
			if(indexOfChar((char*)value,';')!=-1){
				value =  vector_to_array(e0xplode(";",(char*)value));
			}
		}
		map_add(&out,key,value);
	}
	return out;
}

