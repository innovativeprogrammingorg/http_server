#include "io.h"

char * get_content_type(char* directory){
	char* command = concat("/usr/bin/file -i ",directory,FALSE);
	FILE* fd = popen(command,"r");
	char* file_type;
	if(!fread_file(fd,&file_type)){
		puts("Error getting file type.");
		puts("Aborting");
		exit(EXIT_FAILURE);
	}
	Vector v = split(':',file_type);
	free(file_type);
	file_type = vector_get(v,1);
	if(indexOfChar(file_type,';')!=-1){
		file_type = substring(file_type,0,indexOfChar(file_type,';'));
	}
	vector_clean(v);
	printf("Requested item is of type: %s\n",file_type);
	return file_type;
}

void create_tmp_file(char* directory){
	puts("Creating the tmp file");
	char* src = concat("./www",directory,FALSE);
	char* dest = concat("./tmp",concat(directory,".gz",FALSE),SECOND);
	char * command = concat("gzip -c ",concat(src,concat(" > ",dest,SECOND),FIRST|SECOND),SECOND);
	system(command);
	free(command);
	puts("done");
}


uint64_t sread_file(char* directory,char** data){
	char* buffer = (char*)calloc(sizeof(char),200);
	char* dat = NULL;
	size_t n;
	uint64_t out = 0;
	char* src = concat("./www",directory,FALSE);
	/*if(access(src,F_OK)==-1){
		create_tmp_file(directory);
	}*/
	FILE* fd =  fopen(src,"r");
	printf("reading the contents of the file...");
	while(!feof(fd)){
		n = fread(buffer,sizeof(char),199,fd);
		if(dat != NULL){
			dat = concat(dat,buffer,FIRST);
		}else{
			dat = concat(dat,buffer,FALSE);
		}
		out += n;
		memset(buffer,'\0',200);
	}
	*data = dat;
	printf("done\n");
	fclose(fd);
	return out;
}

uint64_t prepare_media(char* directory,char** data){
	//char* src = concat("./tmp",concat(directory,".gz",FALSE),SECOND);
	/*if(access(src,F_OK)==-1){
		create_tmp_file(directory);
	}*/
	char* src = concat("./www",directory,FALSE);
	FILE* fd =  fopen(src,"r");
	char * raw_data;
	size_t n;
	if(!(n = fread_file(fd,&raw_data))){
		perror("Error reading file");
		exit(EXIT_FAILURE);
	}
	char * out = convertToBase64(raw_data);
	free(raw_data);
	*data = out;
	//printf("RAW: %s\n",out);
	return strlen(out);
}

uint64_t fread_file(FILE* fd,char** data){
	char* buffer = (char*)calloc(sizeof(char),200);
	char* dat = NULL;
	size_t n;
	uint64_t out = 0;
	while(!feof(fd)){
		n = fread(buffer,sizeof(char),199,fd);
		if(dat != NULL){
			dat = concat(dat,buffer,FIRST);
		}else{
			dat = concat(dat,buffer,FALSE);
		}
		out += n;
		memset(buffer,'\0',200);
	}
	*data = dat;
	fclose(fd);
	return out;
}

uint64_t read_file(int fd,char** data){
	char* buffer = (char*)calloc(sizeof(char),200);
	char* dat = NULL;
	ssize_t n;
	uint64_t out = 0;
	while((n = read(fd,buffer,199))){
		if(dat != NULL){
			dat = concat(dat,buffer,FIRST);
		}else{
			dat = concat(dat,buffer,FALSE);
		}
		out += n;
		memset(buffer,'\0',200);
	}
	*data = dat;
	close(fd);
	return out;
}

void respond(int fd, char * data){
	//printf("HTTP MESSAGE\n%s\n\n\n",data);
	int chunk_size = 1000;
	int64_t size = strlen(data); 
	int chunks = size/chunk_size;
	int chunk = 0;
	char* buffer;
	printf("Sending %ld bytes of data to the client...",size);
	while(size>0){
		if(size>=chunk_size){
			buffer = substring(data,chunk*chunk_size,size);
			write(fd,buffer,chunk_size);
			free(buffer);
		}else{
			
			buffer = substring(data,chunk*chunk_size,chunk_size);
			write(fd,buffer,size);
			free(buffer);
		}
		size = size - chunk_size;
		chunk++;
	}
	puts("done");
}