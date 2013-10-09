#ifndef BUFFER_H
#define BUFFER_H value

#include <mpi.h>
#include "Cell.h"
#include "Darray.h"
#include "constants.h"

class Buffer
{
public:
	explicit Buffer(int count);
	explicit Buffer(Darray& array);
	~Buffer();
	void * rawData();
	Darray * toDarray();
	int count();
	// NOTE: uncommitted datatype
	static error datatype(MPI_Datatype *type);
private:
	Cell* cells;
	int cellCount;
	
};

#endif