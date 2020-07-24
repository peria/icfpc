CXX=clang++
CXXFLAGS=-O2 -std=c++2a
LDFLAGS=

LIB_SRCS=server.cc galaxy.cc expr.cc

TEST_SRCS=$(wildcard tests/*_test.cc)

GTEST_DIR=../third_party/googletest/googletest
GTEST_SRCS=$(GTEST_DIR)/src/gtest-all.cc $(GTEST_DIR)/src/gtest_main.cc
GTEST_OBJS=$(GTEST_SRCS:.cc=.o)

CXXFLAGS+=-I. -I../third_party/cpp-httplib
LDFLAGS+=-lglog -lpthread

LIB_OBJS=$(LIB_SRCS:.cc=.o)

.PHONY: all clean

all: server test

server: server_main.o $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_SRCS) $(LIB_OBJS) $(LIB_OBJS:.o=.h)
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR)/include -I$(GTEST_DIR) -o $@ $(TEST_SRCS) $(LIB_OBJS) $(GTEST_SRCS) $(LDFLAGS)

clean:
	rm -rf *.o *~ galaxy
