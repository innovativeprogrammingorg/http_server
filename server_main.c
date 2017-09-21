#include "./include/server.h"
int * intmemcopy(int * ptr){
	int* out = (int *)malloc(sizeof(int));
	*out = *ptr;
	return out;
}
char * last_input = NULL;

void segfault_catch(int signum){
	printf("The last received is : \n %s \n",last_input);
	exit(EXIT_FAILURE);
}

int main(){
	int opt = TRUE;
	int master_socket , addrlen , new_socket , activity, i , valread , sd;
	int max_sd;
	int port = 8989;
	int j;
	Vector clients = NULL;
	struct sockaddr_in address;
	char buffer[BUFFER_SIZE];  
	fd_set readfds;
	int cons = 0;
	char * out;
	Map headers = NULL;
	signal(SIGSEGV,segfault_catch);
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
  
	
	if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
  
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( port );
	  
	
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	/*printf("Listener on port %d \n", port);*/
	 
	
	if (listen(master_socket, 10) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	/*puts("Listener created successfully");*/
	
	addrlen = sizeof(address);	 
	while(TRUE) {
		FD_ZERO(&readfds);
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;
		 
		printf("The number of connections is %i\n",cons);
		
		for (i = 0 ; i < cons ; i++) {
			sd = *((int *)vector_get(clients,i));
			 
			
			if(sd > 0)
				FD_SET(sd,&readfds);
			 
			if(sd > max_sd)
				max_sd = sd;
		}
  		/*printf("The max process id is %i\n",max_sd);*/
		activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR)) {
			printf("select error");
		}
		  
		if (FD_ISSET(master_socket, &readfds)){
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
				perror("accept");
				exit(EXIT_FAILURE);
			}
			vector_push(&clients,intmemcopy(&new_socket));	
			cons++;
			continue;
		}
		for (i = 0; i < cons; i++){
			/*printf("Checking on user %i\n",i);*/
			sd = *((int *)vector_get(clients,i));
			/*printf("Retrieved the socket of that user\n");*/
			if (FD_ISSET( sd , &readfds)){
				/*printf("Checking to see what action occured...\n");*/
				if ((valread = read( sd , buffer, 2048)) == 0){
					/*printf("Removing the User from the list\n");*/
					vector_pop(&clients,i);
					/*printf("Removed Successfully!\n");*/
					cons--;
					close(sd);
					i = 0;
				}else{
					for (j = 0; j < cons; j++){
						sd = *((int *)vector_get(clients,j));
						/*printf("The socket number for this message is %i\n",sd);*/
						buffer[valread] = '\0';
						last_input = buffer;
						respond(sd,build_response(parse_HTTP_Header(buffer)));
						//print_map_contents(headers, 's');
						//write(0, buffer, strlen(buffer));
					}
					
				}
			}
		}
		
		memset(&buffer,0,2048);
	}
	  
	return 0;
} 