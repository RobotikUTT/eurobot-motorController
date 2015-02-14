SRCS = $(wildcard ./*.cpp)
OBJS = $(subst .cpp,.o,$(SRCS))
LIBS = $(subst .cpp,.a,$(SRCS))

all: arduino

arduino: $(LIBS)

$(OBJS) : %.o : %.cpp 

%.a : $(OBJS)
	ar rc $@ $^ && ranlib $@