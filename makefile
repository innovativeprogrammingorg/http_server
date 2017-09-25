
CC= gcc
LIB = -I /usr/local/lib/
CFLAGS = -Wall -Werror --pedantic -lm -lssl -lcrypto -pthread $(LIB) $(INCLUDE)
FILES = ./*.c ./include/*.c ./include/CGI/*.c
OBJECTS= $(foreach x, $(basename $(wildcard *.c)), $(x).o)
.PHONY: clean

all: server

server: 
	$(CC)  $(FILES) -o server $(CFLAGS)

clean: 
	rm -f server
fclean:
	rm -f server
	
re: fclean all
