SRC=$(shell ls *.cc)
OBJ=$(SRC:%.cc=obj/%.o)

CXX=clang++
CXXFLAGS=-O2 -std=c++17 -W
CXXFLAGS+=-I.
# 3rd party libraries
CXXFLAGS+=-Ithird_party/elog
CXXFLAGS+=-Ithird_party/cmdline

.PHONY: all clean

all: solver Makefile


solver: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

obj/%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) -rf $(OBJ) *~ solver
