work_path = $(shell pwd)
c_src = $(wildcard $(work_path)/../../../mylib/cJSON/*.c $(work_path)/../../../mylib/mylib_log/*.c)
cpp_src = $(wildcard $(work_path)/../../../mylib/mylib_daemon/*.cpp $(work_path)/../../../mylib/mylib_sql/*.cpp ./*.cpp ../login/*.cpp ../regist/*.cpp)
c_objs = $(patsubst %.c,%.o,$(c_src))
cpp_objs += $(patsubst %.cpp,%.o,$(cpp_src))

objs += $(c_objs) $(cpp_objs)
CC = gcc
CXX = g++
target=userLoginService	
CPPFLAGS =
INCLUDE += -I/usr/include/mysql \
           -I${work_path}/../../../mylib/cJSON \
		   -I${work_path}/../../../mylib/mylib_log \
		   -I${work_path}/../../../mylib/mylib_sql \
		   -I${work_path}/../../../mylib/enum \
		   -I${work_path}/../../../mylib/mylib_daemon \
		   -I${work_path}/../login \
		   -I${work_path}/../regist

CFLAGS = $(INCLUDE)
CXXFLAGS += $(INCLUDE)
CPPFLAGS += $(INCLUDE)
LDFLAGS = -L/usr/lib64/mysql -lmysqlclient -lz -lcrypt -lnsl -lm -lssl -lcrypto

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