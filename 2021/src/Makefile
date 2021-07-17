CXX=clang++

SRCS=main.cc solver.cc problem.cc solution.cc bonus.cc

CXXFLAGS=-O2 -std=c++17
LDFLAGS=-lglog -lgflags
CXXFLAGS+=-O0 -g

ROOT_DIR:=$(dir $(abspath .))
SRC_DIR=$(ROOT_DIR)/src
THIRD_PARTY_DIR=$(ROOT_DIR)/third_party
BUILD_DIR=$(ROOT_DIR)/out

CXXFLAGS+=-I$(SRC_DIR) -I$(THIRD_PARTY_DIR)/json/include
OBJS=$(SRCS:%.cc=$(BUILD_DIR)/%.o)

SOLVERS_DIR=solvers
SOLVERS_SRCS=$(wildcard $(SOLVERS_DIR)/*.cc)
SOLVERS_OBJS=$(SOLVERS_SRCS:%.cc=$(BUILD_DIR)/%.o)
SOLVERS_BUILD_DIR=$(BUILD_DIR)/$(SOLVERS_DIR)

.PHONY: all
all: dirs solver

.PHONY: dirs
dirs:
	mkdir -p $(BUILD_DIR) $(SOLVERS_BUILD_DIR)

solver: $(OBJS) $(SOLVERS_OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cc
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean
clean:
	$(RM) -rf $(BUILD_DIR)
