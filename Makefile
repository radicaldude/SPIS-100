all:
	g++ -oSPIS-100 arrow.cpp node.cpp main.cpp listInput.cpp listOutput.cpp consoleInput.cpp consoleOutput.cpp system.cpp runtime.cpp file.cpp io.cpp -g -lncursesw -lpthread
