SRC_TESTS = $(wildcard unitTest_*.cpp)
## All the executable for the tests
TESTS = $(SRC_TESTS:.cpp=)

OBJETS = Darray.o Dmatrix.o mtrand.o model.o Cell.o Coord.o

CXX = g++
CXXFLAGS = -Wall -g -pedantic 

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
