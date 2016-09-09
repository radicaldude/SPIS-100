all:
	g++ -D_ascii_only arrow.cpp node.cpp file.cpp main.cpp runtime.cpp system.cpp -lncursesw  -lpthread -g  -o SPIS-100
