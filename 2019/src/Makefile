CXX=clang++
CXXFLAGS=-O2 -std=c++14 -W -Wall
CXXFLAGS+=-I. -Ithird_party/tanakh

LDFLAGS=

BUILD_DIR=build

SRCS=booster.cc game.cc map.cc point.cc solver.cc desc_parser.cc
OBJS=$(SRCS:%.cc=$(BUILD_DIR)/%.o)

.PHONY: all dirs clean

all: dirs wrappers

wrappers: wrappers_main.cc $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	@mkdir -p $(BUILD_DIR)

clean:
	rm -rf *.o *~ wrappers $(BUILD_DIR)
