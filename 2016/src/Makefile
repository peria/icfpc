CXX=clang++
INCLUDES=-I. -Ithird_party/
CXXFLAGS=-O2 -std=c++14
CXXFLAGS+=-O0 -g
LDFLAGS=/usr/local/lib/libgmpxx.a -lgmp

SRCS=main.cc problem.cc polygon.cc
OBJS=$(SRCS:.cc=.o)
TARGETS=solver

all: $(TARGETS)

solver: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm -f *~ *.o $(TARGETS)

