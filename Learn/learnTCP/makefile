all:server client

server.o:server.cpp
	g++ -c server.cpp
client.o:client.cpp
	g++ -c client.cpp 
server:server.o
	g++ -o server server.o
client:client.o
	g++ -o client client.o

clean:
	rm -f server client *.o
