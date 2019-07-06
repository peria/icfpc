CXX=clang++
CXXFLAGS=-std=c++14 -W -Wall -Wno-sign-compare
CXXFLAGS+=-O2 -march=native
#CXXFLAGS+=-O0 -g
CXXFLAGS+=-I.

LDFLAGS=

BUILD_DIR=build

SRCS=action.cc booster.cc game.cc map.cc parser.cc point.cc wrapper.cc
SRCS+=$(wildcard algorithm/*.cc)
OBJS=$(SRCS:%.cc=$(BUILD_DIR)/%.o) $(BUILD_DIR)/solver.o
SOLVERS=$(wildcard solvers/*.cc)

.PHONY: all dirs clean

all: dirs solver

solver: solver_main.cc $(OBJS) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $(filter-out Makefile,$^) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cc %.h Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/solver.o: solver.cc solver.h $(SOLVERS) Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	@mkdir -p $(BUILD_DIR) $(BUILD_DIR)/algorithm

clean:
	rm -rf *.o *~ wrappers $(BUILD_DIR)
