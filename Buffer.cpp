#include <stdlib.h>	
#include "Buffer.h"


Buffer::Buffer(uint32_t count){
	cellCount = count;
	cells = (Cell*)calloc(cellCount,sizeof(Cell));
}

Buffer::Buffer(Array& array){
	cellCount = array.getSize();
	cells = (Cell*)calloc(cellCount,sizeof(Cell));
	for (uint32_t i = 0; i < cellCount; i++){
		memcpy(cells+i,array(i),sizeof(Cell));
	}
}

Array * Buffer::toArray(){
	Array* a = new Array(cellCount);
	Cell *c = (Cell*)malloc(sizeof(Cell));
	for (uint32_t i = 0; i < cellCount; i++){
		// TODO: copy constructor
		memcpy(c,cells+i,sizeof(Cell));
		a->set(i,c->getKind(),c->getSex());
		a->operator()(i)->setMoveFlag(c->getMoveFlag());
	}
	free(c);
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
