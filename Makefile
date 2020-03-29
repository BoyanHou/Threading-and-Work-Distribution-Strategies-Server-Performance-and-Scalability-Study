CC=g++
CFLAGS=-std=c++17 -pthread -O0 -ggdb -g -Werror -Wall -pedantic -fPIC

TOOL = ./Tools/
SERV = ./Server/
SOCK = ./Socket/
GLB = ./

all: server

server: server_main.o server.o string_tools.o socket.o
	$(CC) $(CFLAGS) -o $@ $< server.o string_tools.o socket.o

server_main.o: $(GLB)server_main.cpp $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

server.o: $(SERV)Server.cpp $(SERV)Server.h $(SERV)Server_Exceptions.h $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

string_tools.o: $(TOOL)String_Tools.cpp $(TOOL)String_Tools.h $(TOOL)String_Tools_Exceptions.h $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

socket.o: $(SOCK)Socket.cpp $(SOCK)Socket.h $(SOCK)Socket_Exceptions.h $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<
