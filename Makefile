CC = g++

all: client server

client: client.o
	$(CC) client.o   -o client -lvsomeip3

client.o: client.cc client.h
	$(CC) -c -o client.o client.cc

server: server.o
	$(CC) server.o   -o server -lvsomeip3

server.o: server.cc server.h
	$(CC) -c -o server.o server.cc

clean:
	rm -f server client server.o client.o 
	rm -f server.log client.log
