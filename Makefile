CC=g++
CFLAGS=-std=c++17 -pthread -O3 -ggdb -g -Werror -Wall -pedantic -fPIC

TOOL = ./Tools/
SERV = ./Server/
CLNT = ./Client/
SOCK = ./Socket/
GLB = ./

all: server client_scale client_func

server: server_main.o server.o string_tools.o socket.o
	$(CC) $(CFLAGS) -o $@ $< server.o string_tools.o socket.o

client_scale: client_main.o client_scale.o string_tools.o socket.o
	$(CC) $(CFLAGS) -o $@ $< client_scale.o string_tools.o socket.o

client_func: client_main.o client_func.o string_tools.o socket.o
	$(CC) $(CFLAGS) -o $@ $< client_func.o string_tools.o socket.o

client_main.o: $(CLNT)client_main.cpp $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

client_scale.o: $(CLNT)Client.cpp $(CLNT)Client.h $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

client_func.o: $(CLNT)Client.cpp $(CLNT)Client.h $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -DFUNC_TEST -o $@ $<

server_main.o: $(SERV)server_main.cpp $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

server.o: $(SERV)Server.cpp $(SERV)Server.h $(SERV)Server_Exceptions.h $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

string_tools.o: $(TOOL)String_Tools.cpp $(TOOL)String_Tools.h $(TOOL)String_Tools_Exceptions.h $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

socket.o: $(SOCK)Socket.cpp $(SOCK)Socket.h $(SOCK)Socket_Exceptions.h $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o server client_func client_scale
