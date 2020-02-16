ROOT_PATH=$(shell pwd)


INCLUDE += -I../singleTerminalLoginSystem/client/processCommandLine


messageReceiverService:main.cpp messageReceiver.cpp logrecord.c
	g++ -o $@ $^



.PHONY:clean
clean:
	rm -rf messageReceiverService