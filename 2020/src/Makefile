CXX=clang++
CXXFLAGS=-O2 -std=c++2a
LDFLAGS=

LIB_SRCS=galaxy.cc expr.cc
MAIN_SRCS=galaxy_main.cc
SRCS=$(LIB_SRCS) $(MAIN_SRCS)

TEST_SRCS=$(wildcard tests/*_test.cc)
TEST_OBJS=$(TEST_SRCS:%.cc=%.o)

GTEST_DIR=../third_party/googletest/googletest
GTEST_SRCS=$(GTEST_DIR)/src/gtest-all.cc $(GTEST_DIR)/src/gtest_main.cc
GTEST_OBJS=$(GTEST_SRCS:.cc=.o)

CXXFLAGS+=-I.
CXXFLAGS+=-I$(GTEST_DIR)/include -I$(GTEST_DIR)
LDFLAGS+=-lglog -lpthread

LIB_OBJS=$(LIB_SRCS:.cc=.o)
MAIN_OBJS=$(MAIN_SRCS:.cc=.o)

.PHONY: all clean

all: galaxy test

galaxy: galaxy_main.o $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_OBJS) $(LIB_OBJS) $(LIB_OBJS:.o=.h) $(GTEST_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJS) $(LIB_OBJS) $(GTEST_SRCS) $(LDFLAGS)

clean:
	rm -rf *.o *~ galaxy
