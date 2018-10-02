SRC=$(filter-out %_test.cc, $(shell ls *.cc solvers/*.cc))
OBJ=$(SRC:%.cc=obj/%.o)
TEST_SRC=$(shell ls *_test.cc)
TESTS=$(TEST_SRC:%.cc=%)

CXX=clang++
CXXFLAGS=-O2 -std=c++17 -W
#CXXFLAGS=-O0 -g -std=c++17 -W -Werror
CXXFLAGS+=-I.
# 3rd party libraries
CXXFLAGS+=-Ithird_party/cmdline
LDFLAGS+=-lglog

GTEST_DIR=third_party/googletest/googletest

.PHONY: all clean format

all: $(TESTS) solver Makefile

solver: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

obj/%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

%_test: %_test.cc $(filter-out %main.o, $(OBJ))
	$(CXX) $(CXXFLAGS) -o $@ \
	$^ $(GTEST_DIR)/src/gtest_main.cc $(GTEST_DIR)/src/gtest-all.cc \
	-I$(GTEST_DIR)/include -I$(GTEST_DIR) $(LDFLAGS) -lpthread


format:
	clang-format -i **/*.cc **/*.h

clean:
	$(RM) -rf $(OBJ) *~ solver
