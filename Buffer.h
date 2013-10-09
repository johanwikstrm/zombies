#ifndef BUFFER_H
#define BUFFER_H value

#include <mpi.h>
#include "Cell.h"
#include "Array.h"
#include "constants.h"

class Buffer
{
public:
	explicit Buffer(int count);
	explicit Buffer(Array& array);
	~Buffer();
	void * rawData();
	Array * toArray();
	int count();
	// NOTE: uncommitted datatype
	error datatype(MPI_Datatype *type);
private:
	Cell* cells;
	int cellCount;
	
};

#endif
