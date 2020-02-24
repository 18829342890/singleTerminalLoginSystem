work_path = $(shell pwd)
c_src = $(wildcard $(work_path)/../../mylib/mylibLog/*.c)
cpp_src = $(wildcard $(work_path)/../../mylib/mylibSql/*.cpp ./*.cpp ../regist/*.cpp ../login/*.cpp ../syncClientInfo/*.cpp)
cc_src = $(wildcard $(work_path)/../../protos/protoCode/*.cc)
c_objs = $(patsubst %.c,%.o,$(c_src))
cpp_objs += $(patsubst %.cpp,%.o,$(cpp_src))
cc_objs = $(patsubst %.cc,%.o,$(cc_src))

objs += $(c_objs) $(cpp_objs) $(cc_objs) 
CC = gcc
CXX = g++
target=messageReceiverService	
##mysql 用 mysql_config --include 命令给出
INCLUDE += -I/usr/local/mysql/include \
		   -I${work_path}/../../mylib/mylibLog \
		   -I${work_path}/../../mylib/mylibSql \
		   -I${work_path}/../../mylib/enum \
		   -I${work_path}/../../protos/protoCode \
		   -I${work_path}/../regist \
		   -I${work_path}/../login \
		   -I${work_path}/../syncClientInfo


CFLAGS = $(INCLUDE)
CXXFLAGS += -std=c++11 $(INCLUDE)
CPPFLAGS += `pkg-config --cflags protobuf grpc` $(INCLUDE)
LDFLAGS =  -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`\
           -lgrpc++_reflection -ldl\
           -L/usr/local/mysql/lib -lmysqlclient \
           -L/usr/local/opt/openssl/lib -lssl -lcrypto

$(warning  $(c_src))
$(warning  $(cpp_src))
$(warning  $(c_objs))
$(warning  $(cpp_objs))
$(warning  $(objs))

ALL:$(target)

$(target):$(objs) 
	$(CXX) $(objs) -o $(target) $(LDFLAGS)

%.o:%.c	
	$(CXX) -c $< -o $@ $(CFLAGS)
	
clean:
	-rm -rf $(target) $(objs)

.PHONY:clean