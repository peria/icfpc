CXX=clang++
CXXFLAGS=-O2 -std=c++14 -Wall

TMP_FILE=/tmp/command.txt

all: um

um: um.cc um_main.cc um.h
	$(CXX) $(CXXFLAGS) -o $@ $(filter %.cc, $^)

dump: um codex.umz description.key
	cp description.key $(TMP_FILE)
	echo "p" >> $(TMP_FILE)
	./um codex.umz < $(TMP_FILE) | tail -c +196 > um.um

# unbuildable on recent x64 machines.
um32: um32.cc
	$(CXX) $(CXXFLAGS) -m32 -o $@ $^

.PHONY: clean
clean:
	$(RM) -f um um32 *.o *~ *.um
