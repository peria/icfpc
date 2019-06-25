CXX=clang++
CXXFLAGS=-O2 -std=c++14 -W -Wall -Wno-sign-compare
CXXFLAGS+=-I.

LDFLAGS=

BUILD_DIR=build

SRCS=action.cc booster.cc game.cc map.cc point.cc solver.cc desc_parser.cc wrapper.cc
OBJS=$(SRCS:%.cc=$(BUILD_DIR)/%.o)

.PHONY: all dirs clean

all: dirs solver

solver: solver_main.cc $(OBJS) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $(filter-out Makefile,$^) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	@mkdir -p $(BUILD_DIR)

clean:
	rm -rf *.o *~ wrappers $(BUILD_DIR)
