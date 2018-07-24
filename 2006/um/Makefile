CXX=clang++
CXXFLAGS=-O3 -std=c++11 -Wall -Wextra

all: um fast_um

um: um.o um_main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

fast_um: fast_um.cc
	$(CXX) $(CXXFLAGS) -m32 -o $@ $^

.cc.o: um.h
	$(CXX) $(CXXFLAGS) -c $<
