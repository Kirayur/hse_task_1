CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17
HEADER = second_version_header.hpp

all: tests pi

tests: test_second_version.o
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Running tests..."
	@./tests

pi: pi_first_version.o
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Calculating Pi..."
	@./pi

test_second_version.o: test_second_version.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $<

pi_first_version.o: pi_first_version.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $<

rand12.o: rand12.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o tests pi

.PHONY: all tests pi clean
