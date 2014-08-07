CXX=clang++
CXXFLAGS=-O3 -std=c++11 -Wall -Wextra

all: um

um: um.o um_main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

.cc.o: um.h
	$(CXX) $(CXXFLAGS) -c $<
