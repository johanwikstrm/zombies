SRC_TESTS = $(wildcard unitTest_*.cpp)
## All the executable for the tests
TESTS = $(SRC_TESTS:.cpp=)

OBJETS = Array.o Matrix.o mtrand.o Model.o Cell.o Buffer.o Coord.o Lock.o mpiutils.o Statistic.o

CXX = mpiCC 
#g++
CXXFLAGS = -Wall -g -pedantic -fopenmp

all : $(TESTS) main

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
