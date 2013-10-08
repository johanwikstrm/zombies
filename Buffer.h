#ifndef BUFFER_H
#define BUFFER_H value

#include <mpi.h>
#include "Cell.h"
#include "constants.h"

class Buffer
{
public:
	explicit Buffer(int count);
	~Buffer();
	const void * rawData();
	int count();
	// NOTE: uncommitted datatype
	MPI_Datatype datatype();
private:
	Cell* cells;
	int cellCount;
	
};

#endif