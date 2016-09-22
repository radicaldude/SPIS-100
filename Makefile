all:
	g++ -DASCII_ONLY -std=c++11 -oSPIS-100 arrow.cpp node.cpp main.cpp listInput.cpp listOutput.cpp consoleInput.cpp consoleOutput.cpp system.cpp runtime.cpp file.cpp io.cpp -g -lncurses -lpthread
