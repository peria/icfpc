CXX=clang++
CPP=clang++
CXXFLAGS=-O2 -std=c++20 -Werror
LDFLAGS=-lglog -lgflags -lfmt
CXXFLAGS+=-O0 -g

SRCS=problem.cc solution.cc
SRCS+=empty_solver.cc
HEADERS=$(wildcard *.h)
SRCS+=main.cc

ROOT_DIR:=$(dir $(abspath .))
DATA_DIR=$(ROOT_DIR)/data
PROBLEMS_DIR=$(abspath $(DATA_DIR)/problems)
SOLUTIONS_DIR=$(abspath $(DATA_DIR)/solutions)
SRC_DIR=$(ROOT_DIR)/src
THIRD_PARTY_DIR=$(ROOT_DIR)/third_party
BUILD_DIR=$(abspath $(ROOT_DIR)/out)

CXXFLAGS+=-DDEFAULT_PROBLEMS_DIR=\"$(PROBLEMS_DIR)\"
CXXFLAGS+=-DDEFAULT_SOLUTIONS_DIR=\"$(SOLUTIONS_DIR)\"
CXXFLAGS+=-I$(SRC_DIR) -I$(THIRD_PARTY_DIR)/json/include
OBJS=$(SRCS:%.cc=$(BUILD_DIR)/%.o)

DEPEND=depend.inc

.PHONY: all
all: dirs solver $(DEPEND)

.PHONY: dirs
dirs:
	mkdir -p $(BUILD_DIR)

solver: $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cc
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean
clean:
	$(RM) -rf $(BUILD_DIR)

$(DEPEND): $(SRCS) $(HEADERS) Makefile
	-@ $(RM) $(DEPEND)
	-@ for i in $(filter %.cc,$^); \
		do \
			$(CPP) $(CXXFLAGS) -M $$i >>$@; \
		done

-include $(DEPEND)
