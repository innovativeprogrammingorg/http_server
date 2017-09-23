#include "header.h"
Map parse_HTTP_body(char * body){
	Vector data = explode("&",body);
	size_t length = vector_length(data);
	Map out = NULL;
	uint64_t i;
	Vector pair;
	for(i = 0;i<length;i++){
		pair = split('=',(char*)vector_get(data,i));
		map_add(&out,vector_get(pair,0),vector_get(pair,1),STRING_TYPE);
		vector_clean(pair);
	}
	return out;
}

Map parse_HTTP_message(char * message){
	if(strpos(message,"GET")==0){
		return parse_HTTP_header(message);
	}	
	Map out = NULL;
	Vector v = ssplit("\r\n\r\n",message);
	if(vector_length(v)!=2){
		printf("Unexpected vector length of  %lu\n",vector_length(v));
		//exit(0);
	}
	Map header = parse_HTTP_header(vector_get(v,0));
	
	
	map_add(&out,"HEADER",header,MAP_TYPE);
	map_add(&out,"BODY",parse_HTTP_body(vector_get(v,1)),MAP_TYPE);
	return out;
}
Map parse_HTTP_header(char * header){
	/*puts("Parsing the HTTP header");*/
	//printf("\n\n\n\n%s\n\n\n\n",header);
	Vector parameters = explode("\r\n",header);
	size_t length = vector_length(parameters);
	int i = 0;
	Map out = NULL;
	char* key;
	void* value;
	void* tmp;
	Vector key_value_pair = NULL;
	Vector values = NULL;
	//puts("Parsing the parameters...");
	for(i = 0;i<length;i++){
		//printf("Parsing parameter %d/%lu\n",i,length);
		if(i == 0){
			key = "REQUEST";
			tmp = (char*)vector_get(parameters,i);
			value = malloc(sizeof(char*)*(strlen(tmp)));
			strcpy((char*)value,tmp);
			map_add(&out,key,value,STRING_TYPE);
		}else{
			key_value_pair = split(':',vector_get(parameters,i));
			key = trim((char*)vector_get(key_value_pair,0));
			value = (char*)malloc(sizeof(char*)*(strlen((char*)vector_get(key_value_pair,1)) + 1));
		
			strcpy(value,(char*)vector_get(key_value_pair,1));
			value = trim(value);
			//printf("Value is %s\n",(char*)value);
			if(strpos(key,"User-Agent")!=-1){
				map_add(&out,key,value,STRING_TYPE);
				//printf("Added %s:%s\n",key,(char*)value);
			}else if(indexOfChar((char*)value,';')!=-1){
				/*puts("Value is an array delim = ;");
				printf("Key is %s\n",key);*/
				value =  vector_to_array(explode(";",(char*)value));
				map_add(&out,key,value,ARRAY_TYPE);
			}else if(indexOfChar((char*)value,',')!=-1){
				/*puts("Value is an array delim = ,");
				printf("Key is %s\n",key);*/
				value =  vector_to_array(explode(",",(char*)value));
				map_add(&out,key,value,ARRAY_TYPE);
			}else{
				map_add(&out,key,value,STRING_TYPE);
				//printf("Added %s:%s\n",key,(char*)value);
			}
		}
	}
	return out;
}


char * get_status_line(int status){
	switch(status){
		case 200:
			return "HTTP/1.1 200 OK\r\n";
		case 404:
			return "HTTP/1.1 404 Not Found\r\n";
		case 400:
			return "HTTP/1.1 400 Bad Request\r\n";
		case 415:
			return "HTTP/1.1 415 Unsupported Media Type\r\n";
		case 403:
			return "HTTP/1.1 403 Forbidden\r\n";
		case 500:
			return "HTTP/1.1 500 Internal Server Error\r\n";
		case 503:
			return "HTTP/1.1 503 Service Unavailable\r\n";
	}
}

char * get_date_line(){
    time_t timer;
    char* buffer = (char*)malloc(sizeof(char)*38);
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 37, "Date: %Y-%m-%d %H:%M:%S GMT\r\n", tm_info);
    return buffer;
}

char * get_server_line(){
	return "Server: Nate's C Server\r\n";
}

char* get_content_encoding_line(char * encode){
	return concat("Content-Encoding: ",concat(encode,"\r\n",FALSE),SECOND);
}

char* get_vary_line(){
	return "Vary: Accept-Encoding,User-Agent\r\n";
}

char* get_content_length_line(uint64_t content_length){
	char* out = (char*)calloc(sizeof(char),100);
	sprintf(out,"Content-Length: %lu\r\n",content_length);
	return out;
}

char* get_content_type_line(char* content){
	return concat("Content-Type: ",concat(content,"\r\n",FALSE),SECOND);
}

char* get_accept_ranges_line(){
	return "Accept-Ranges: bytes\r\n";
}

char* get_connection_line(Map header){
	char* conn = (char*)map_value_at(header,"Connection");
	return concat("Connection: ",concat(conn,"\r\n",FALSE),SECOND);
}

uint16_t check_valid_params(Map m){
	if(!map_has_key(m,"Host")){
		return 400;
	}
	if(!map_has_key(m,"Connection")){
		return 400;
	}
	if(!map_has_key(m,"REQUEST")){
		return 500;
	}

	return TRUE;
}

uint8_t get_request_type(Map m){
	char* status_line = (char*)map_value_at(m,"REQUEST");
	if(!status_line){
		status_line = (char*)map_value_at((Map)map_value_at(m,"HEADER"),"REQUEST");
	}
	if(strpos(status_line,"GET")==0){
		return GET;
	}
	if(strpos(status_line,"POST")==0){
		return POST;
	}
	if(strpos(status_line,"PUT")==0){
		return PUT;
	}
	if(strpos(status_line,"DELETE")==0){
		return DELETE;
	}
	if(strpos(status_line,"HEAD")==0){
		return HEAD;
	}
	if(strpos(status_line,"OPTIONS")==0){
		return OPTIONS;
	}
	if(strpos(status_line,"CONNECT")==0){
		return CONNECT;
	}
	return UNKNOWN;
}

char* build_response(Map m){
	if(1 != check_valid_params(m)){
		//handle the error here
	}
	uint8_t request = get_request_type(m);
	switch(request){
		case GET:
			return GET_response(m);
		case POST:
			puts("Received a POST request!");
			return POST_response(m);
			break;
		default:
			break;
	}
	return NULL;
}	