all:
	g++ -oSPIS-100 arrow.cpp node.cpp main.cpp system.cpp runtime.cpp file.cpp io.cpp -g -lncursesw -lpthread
