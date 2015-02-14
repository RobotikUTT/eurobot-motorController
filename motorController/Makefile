SRCS = $(wildcard ./test/*.cpp)
OBJS = $(subst .cpp,.o,$(SRCS))

CXXFLAGS = -I./test/lib/Catch -I./test/lib/arduino
LDFLAGS = -L./test/lib/arduino/

all: tests motorController

$(OBJS): %.o : %.cpp

motorController: 
	cd src && make

tests: $(OBJS)
	make -C ./test/lib/arduino
	$(CXX) $^ -o ./bin/test/testRunner && ./bin/test/testRunner

deploy:
	make upload -C src

clean:
	$(RM) $(OBJS) && make -C ./src clean

dist-clean: clean
	$(RM) ./bin/test/*