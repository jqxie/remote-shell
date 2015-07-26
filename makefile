BASE_SOURCES   = tcp-client.c tcp-server.c
BASE_OBJECTS   = tcp-client.o tcp-server.o
#HEADERS        = tcp-client.h tcp-server.h
COMPILE        = gcc -c -std=c99 -Wall
LINK           = gcc -o
REMOVE         = rm -f
MEMCHECK       = valgrind --leak-check=full

tcp-client : tcp-client.o 
	$(LINK) tcp-client tcp-client.o 

tcp-server : tcp-server.o 
	$(LINK) tcp-server tcp-server.o 

$(BASE_OBJECTS) : $(BASE_SOURCES)
	$(COMPILE) $(BASE_SOURCES)

clean :
	$(REMOVE) tcp-client tcp-server tcp-client.o tcp-server.o $(BASE_OBJECTS)
