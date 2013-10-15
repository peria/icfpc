CXX=g++
CXXFLAGS=-O3 -Wall -std=c++0x
LDFLAGS=-O3 -Wall -std=c++0x

LIB=-lgtest -lgtest_main -lpthread

all: solver expression_test problem_test

test: expression_test problem_test
	./expression_test
	./problem_test


solver: expression.o problem.o solver.o
	$(CXX) $(LDFLAGS) -o $@ $^

expression_test: expression_test.o expression.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIB)

problem_test: problem_test.o problem.o expression.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIB)

.cc.o: Makefile
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *~ .*~ *.o
