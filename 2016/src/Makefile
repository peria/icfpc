CXX=clang++
CC=clang
CXXFLAGS=-std=c++11 -O3 -I${HOME}/lib/include
LDFLAGS=-L${HOME}/lib/lib -lglog -lgmpxx -lgmp

BASE=base.o vertex.o polygon.o silhouette.o skeleton.o problem.o

all: solution_main

solution_main: solution_main.o solution.o $(BASE)
	$(CXX) $^ -o $@ $(LDFLAGS)

.cc.o:
