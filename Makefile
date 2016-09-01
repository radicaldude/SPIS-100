all:
	g++ -D_ascii_only arrow.cpp node.cpp file.cpp main.cpp runtime.cpp -lncursesw  -lpthread -g  -o SPIS-100
