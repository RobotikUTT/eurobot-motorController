CXX = clang++
SRCS = $(wildcard *.cpp)
OBJS = $(subst .cpp,.o,$(SRCS))

LIBS_ = $(subst .cpp,.a,$(SRCS))
LIBS = $(addprefix lib,$(LIBS_))

all: arduino

arduino: $(LIBS)

$(OBJS) : %.o : %.cpp 

%.a : $(OBJS)
	ar rcs $@ $^