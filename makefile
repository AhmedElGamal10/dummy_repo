server: 
	g++ -o server server.cpp functions.o
	
functions: 
	 g++ -c functions.cpp -o functions.o
	 
client: 
	g++ -o client client.cpp functions.o


all:
	g++ -std=c++11 -c functions.cpp -o functions.o 
	g++ -std=c++11 -o client client.cpp functions.o
	g++ -std=c++11 -o server server.cpp functions.o


clear:
	rm client
	rm server
	rm functions.o