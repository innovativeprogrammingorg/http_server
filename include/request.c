#include "request.h"
char* e404_response(Map header){
	char* directory = "/404.shtml";
	char* body;
	FILE* fd;
	char* head;
	uint64_t content_length;
	content_length = sread_file(directory,&body);
	head = get_status_line(404);
	head = concat(head,get_date_line(),SECOND);
	head = concat(head,get_server_line(),FIRST);
	head = concat(head,get_content_length_line(content_length),FIRST|SECOND);
	head = concat(head,get_connection_line(header),FIRST);
	head = concat(head,get_content_type_line("text/html"),FIRST);
	head = concat(head,get_vary_line(),FIRST);
	//head = concat(head,get_content_encoding_line(),FIRST);
	return concat(head,concat("\r\n",body,SECOND),FIRST|SECOND);
}

char* GET_response(Map header){
	char* directory = get_requested_directory(header);
	char* body;
	int fd;
	char* head;
	uint64_t content_length;
	char* src = concat("./www",directory,FALSE);
	if(getLast(directory)=='/'){
		directory = concat(directory,"index.html",FIRST);
	}

	if(access(src,F_OK)==-1){
		return e404_response(header);
	}
	free(src);
	
	char* content_type = get_content_type(concat("./www",directory,FALSE));

	/**
	 * Build the HTTP Message
	 */
	head = get_status_line(200);
	if(strpos(content_type,"text")==-1){
		head = concat(head,get_accept_ranges_line(),FALSE);
		content_length = prepare_media(directory,&body);
		//head = concat(head,get_content_encoding_line("gzip"),FIRST);
		head = concat(head,get_content_type_line(content_type),FIRST);
	}else{
		content_length = sread_file(directory,&body);
	}
	head = concat(head,get_date_line(),SECOND);
	head = concat(head,get_content_length_line(content_length),FIRST|SECOND);
	head = concat(head,get_connection_line(header),FIRST);
	head = concat(head,get_server_line(),FIRST);
	
	//head = concat(head,get_vary_line(),FIRST);
	return concat(head,concat("\r\n",body,FALSE),FIRST|SECOND);
}

char* POST_response(Map message){
	char* directory = get_requested_directory((Map)map_value_at(message,"HEADER"));
	directory = concat("./www",directory,SECOND);
	
	return NULL;
}

char* get_requested_directory(Map m){
	char* status_line = (char*)map_value_at(m,"REQUEST");
	int start = indexOfChar(status_line,'/');
	int end = 0;
	while(status_line[end+start]!=' ' && status_line[end+start]!='\0'){
		end++;
	}
	return substring(status_line,start,end);
}