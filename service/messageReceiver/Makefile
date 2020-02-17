ROOT_PATH=$(shell pwd)


INCLUDE += -I../singleTerminalLoginSystem/client/processCommandLine

g++ main.cpp messageReceiver.cpp logrecord.c register.cpp sqlApi.cpp -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient -lz -lcrypt -lnsl -lm -lssl -lcrypto -o messageReceiverService

messageReceiverService:main.cpp messageReceiver.cpp logrecord.c register.cpp loginer.cpp sqlApi.cpp -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient -lz -lcrypt -lnsl -lm -lssl -lcrypto
	g++ $^ -o $@



.PHONY:clean
clean:
	rm -rf messageReceiverService