CC=gcc
CFLAGS=-Wall -pthread

all: server

server: main.o http_server.o static_handler.o calc_handler.o utils.o
	$(CC) $(CFLAGS) -o server main.o http_server.o static_handler.o calc_handler.o utils.o

main.o: main.c http_server.h
	$(CC) $(CFLAGS) -c main.c

http_server.o: http_server.c http_server.h static_handler.h calc_handler.h
	$(CC) $(CFLAGS) -c http_server.c

static_handler.o: static_handler.c static_handler.h utils.h
	$(CC) $(CFLAGS) -c static_handler.c

calc_handler.o: calc_handler.c calc_handler.h
	$(CC) $(CFLAGS) -c calc_handler.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f *.o server
