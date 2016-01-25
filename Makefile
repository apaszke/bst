#CXX=clang++
CXX=g++-5
CXXFLAGS+=--std=c++11 -Wall --pedantic -g

.PHONY: all clean

all: clean sample

sample:
	$(CXX) $(CXXFLAGS) sample_bst_usage.cpp -o $@

clean:
	@rm -f sample
	@rm -rf *.dSYM
