CXX=clang++
CXXFLAGS=-O2 -std=c++14 -Wall

all: um

um: um.cc um_main.cc um.h
	$(CXX) $(CXXFLAGS) -o $@ $(filter %.cc, $^)

# unbuildable on recent x64 machines.
um32: um32.cc
	$(CXX) $(CXXFLAGS) -m32 -o $@ $^

.PHONY: clean
clean:
	$(RM) um um32 *.o *~
