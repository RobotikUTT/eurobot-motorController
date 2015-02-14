SRCS=$(wildcard ./test/*.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

all: tests motorController

$(OBJS): %.o : %.cpp

deploy:
	cd src && make upload

motorController: 
	cd src && make

tests: $(OBJS)
	echo $(OBJS)
	g++ $(OBJS) -o ./bin/test/testRunner && ./bin/test/testRunner

clean:
	rm -f $(OBJS) && make -C ./src clean

dist-clean: clean
	rm -rf ./bin/test/*