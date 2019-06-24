CXX=clang++
CXXFLAGS=-O2 -std=c++14 -W -Wall
CXXFLAGS+=-I.

LDFLAGS=

BUILD_DIR=build

SRCS=solver.cc game.cc map.cc point.cc
OBJS=$(SRCS:%.cc=$(BUILD_DIR)/%.o)

.PHONY: all dirs clean

all: dirs wrappers

wrappers: wrappers_main.cc $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) -c $<

dirs:
	@mkdir -p $(BUILD_DIR)

clean:
	rm -f *.o *~ wrappers
