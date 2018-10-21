CXX=clang++
CXXFLAGS+=-O2 -Wall -std=c++17

LDFLAGS+=-lglog -lgflags
GTEST_DIR=../third_party/googletest/googletest

all: dna2rna dna2rna_test

test: dna2rna_test
	./dna2rna_test

dna2rna: dna2rna.o dna2rna_main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

dna2rna_test: dna2rna.o dna2rna_test.cc
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) \
	$(GTEST_DIR)/src/gtest_main.cc $(GTEST_DIR)/src/gtest-all.cc \
	-I$(GTEST_DIR)/include -I$(GTEST_DIR) -lpthread

clean:
	rm -f *.o *~ dna2rna dna2rna_test
