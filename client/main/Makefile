work_path = $(shell pwd)
c_src = $(wildcard $(work_path)/../../mylib/*/*.c)
cpp_src = $(wildcard ./*.cpp ../service/*/*.cpp ../commanLine/*.cpp)
cc_src = $(wildcard $(work_path)/../../protos/*/*.cc $(work_path)/../../mylib/mylibLog/*.cc $(work_path)/../../mylib/myLibEncrypt/*.cc)
c_objs = $(patsubst %.c,%.o,$(c_src))
cpp_objs += $(patsubst %.cpp,%.o,$(cpp_src))
cc_objs = $(patsubst %.cc,%.o,$(cc_src))

objs += $(c_objs) $(cpp_objs) $(cc_objs)
CC = gcc
CXX = g++
target=client	
INCLUDE += -I/usr/local/opt/openssl/include \
		   -I${work_path}/../../            \
		   -I${work_path}/../../mylib/mylibLog \
		   -I${work_path}/../../mylib/enum \
		   -I${work_path}/../../mylib/myLibEncrypt \
		   -I${work_path}/../../protos/protoCode \
		   -I${work_path}/../service/receiveLogoutNotice \
		   -I${work_path}/../myToolLib  \
		   -I${work_path}/../commanLine  \
		    -I${work_path}/../commanLine/processCommandLine  

CFLAGS = $(INCLUDE)
CXXFLAGS += -std=c++11 $(INCLUDE)
CPPFLAGS += `pkg-config --cflags protobuf grpc` $(INCLUDE)
LDFLAGS = -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`\
           -lgrpc++_reflection\
           -ldl -lpthread\
           -L/usr/local/opt/openssl/lib -lssl -lcrypto

$(warning  $(c_src))
$(warning  $(cpp_src))
$(warning  $(c_objs))
$(warning  $(cpp_objs))
$(warning  $(cc_src))
$(warning  $(objs))

ALL:$(target)

$(target):$(objs) 
	$(CXX) $(objs) -o $(target) $(LDFLAGS)

%.o:%.c	
	$(CXX) -c $< -o $@ $(CFLAGS)
	
clean:
	-rm -rf $(target) $(objs)

.PHONY:clean