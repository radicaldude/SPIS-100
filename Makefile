all:
	g++ arrow.cpp node.cpp file.cpp main.cpp runtime.cpp  -lncurses -lpthread -g -Wall -o SPIS-100 	
