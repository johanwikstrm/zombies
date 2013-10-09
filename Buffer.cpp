#include <stdlib.h>	
#include "Buffer.h"


Buffer::Buffer(int count){
	cellCount = count;
	cells = (Cell*)calloc(cellCount,sizeof(Cell));
}

Buffer::Buffer(Array& array){
	cellCount = array.getSize();
	cells = (Cell*)calloc(cellCount,sizeof(Cell));
	for (int i = 0; i < cellCount; i++){
		memcpy(cells+i,array(i),sizeof(Cell));
	}
}

Array * Buffer::toArray(){
	Array* a = new Array(cellCount);
	for (int i = 0; i < cellCount; i++){
		// TODO: copy constructor
		Cell *c = (Cell*)malloc(sizeof(Cell));
		memcpy(c,cells+i,sizeof(Cell));
		(*a)(i) = c;
	}
	return a;
}

Buffer::~Buffer(){
	free(cells);
}

void * Buffer::rawData(){
	return cells;
}

int Buffer::count(){
	return cellCount;
}

error Buffer::datatype(MPI_Datatype *type){
	 return MPI_Type_contiguous(sizeof(Cell),MPI_BYTE,type);
}
