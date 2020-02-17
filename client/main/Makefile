ROOT_PATH=$(shell pwd)


INCLUDE += -I../singleTerminalLoginSystem/client/processCommandLine


client:main.cpp commandLine.cpp 
	g++ -o $@ $^ -lpthread



.PHONY:clean
clean:
	rm -rf client