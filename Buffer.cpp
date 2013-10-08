#include <stdlib.h>	
#include "Buffer.h"


Buffer::Buffer(int count){
	cellCount = count;
	cells = (Cell*)calloc(count,sizeof(Cell));
	for (int i = 0; i < count; i++){
		cells[i] = Cell(EMPTY);
	}
}

Buffer::~Buffer(){
	free(cells);
}

const void * Buffer::rawData(){
	return cells;
}

int Buffer::count(){
	return cellCount;
}

MPI_Datatype Buffer::datatype(){

}