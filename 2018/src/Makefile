SRC=$(shell ls *.cc solvers/*.cc)
OBJ=$(SRC:%.cc=obj/%.o)

CXX=clang++
CXXFLAGS=-O2 -std=c++17 -W
#CXXFLAGS=-O0 -g -std=c++17 -W -Werror
CXXFLAGS+=-I.
# 3rd party libraries
CXXFLAGS+=-Ithird_party/cmdline
LDFLAGS+=-lglog

.PHONY: all clean format

all: solver Makefile


solver: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

obj/%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

format:
	clang-format -i **/*.cc **/*.h

clean:
	$(RM) -rf $(OBJ) *~ solver
