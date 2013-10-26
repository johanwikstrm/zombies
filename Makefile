SRC_TESTS = $(wildcard unitTest_*.cpp)
## All the executable for the tests
TESTS = $(SRC_TESTS:.cpp=)

OBJETS = Array.o Matrix.o Model.o Cell.o Buffer.o Coord.o Lock.o mpiutils.o constants.o Statistic.o

CXX = mpic++ 
#g++
CXXFLAGS = -g -Wall -fopenmp -pedantic

all : $(TESTS) main

unitTest_buffer : $(OBJETS) unitTest_buffer.o
	$(CXX) $(CXXFLAGS) -o $@ $^

unitTest_% : $(OBJETS) unitTest_%.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main : $(OBJETS) main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

check :
	make
	./unitTests.sh

checkMemory :
	make
	./checkMemory.sh

clean :
	rm -f main
	rm -f *.o
	rm -f $(TESTS)
