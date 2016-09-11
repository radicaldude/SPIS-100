all:
	g++ -oSPIS-100 arrow.cpp node.cpp main.cpp system.cpp runtime.cpp file.cpp -g -lncursesw -lpthread
