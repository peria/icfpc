CXXFLAGS+=-O3 -Wall -std=c++0x
LDFLAGS+=-lglog -lgflags
CC=g++

all: dna2rna_main dna2rna_test

test: dna2rna_test
	./dna2rna_test

dna2rna_main: dna2rna.o dna2rna_main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

dna2rna_test: dna2rna.o dna2rna_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -lgtest_main -lgtest -lpthread

.o.cc:

clean:
	rm -f *.o *~ dna2rna_main dna2rna_test
