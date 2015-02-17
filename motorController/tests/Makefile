CXX = clang++
GTEST_DIR = ./lib/gmock-1.7.0/gtest
GMOCK_DIR = ./lib/gmock-1.7.0

SRCS = $(wildcard *.cpp)
SRCS += $(wildcard ../src/*.cpp)
OBJS = $(subst .cpp,.o,$(SRCS))

CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include -I../src -I./lib/arduino
LDFLAGS = -L./lib/arduino -lArduino
CXXFLAGS += -g -Wall -Wextra -pthread -DTEST_VIRTUAL="virtual"



all: gmock.a tests

clean:
	rm -f $(TESTS) gmock.a gmock_main.a *.o


%.o: %.cpp $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $^ -o $@

tests: gmock_main.a $(OBJS)
	$(CXX) $^ $(CPPFLAGS) $(CXXFLAGS) -lpthread -o ../bin/tests/tests $(LDFLAGS)


######################### DO NOT TOUCH BELOW ########################

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
				$(GTEST_DIR)/include/gtest/internal/*.h
GMOCK_HEADERS = $(GMOCK_DIR)/include/gmock/*.h \
				$(GMOCK_DIR)/include/gmock/internal/*.h \
				$(GTEST_HEADERS)

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)

gtest-all.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
			-c $(GTEST_DIR)/src/gtest-all.cc

gmock-all.o:  $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
			-c $(GMOCK_DIR)/src/gmock-all.cc

gmock_main.o: $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
			-c $(GMOCK_DIR)/src/gmock_main.cc

gmock.a: gmock-all.o gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gmock_main.a: gmock-all.o gtest-all.o gmock_main.o
	$(AR) $(ARFLAGS) $@ $^
