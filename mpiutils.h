#ifndef MPIUTILS_H
#define MPIUTILS_H 
#include "constants.h"
#include "Buffer.h"
#include "Array.h"

// determines your four neighbours in a coordinate system 
// where y grows downwards and x grows to the right
// The universe also wraps around
// neighbours = [up,right,down,left]

const char * dirstr(DIRECTION d);


void neighbours(int x,int y, int width, int height, int nbours[4]);
int x(int rank);

int y(int rank);

error recvFromNeighbour(int from,Buffer* to,MPI_Datatype dtype,int tag);

error sendToNeighbour(int dest, Buffer *data,MPI_Request *request, MPI_Datatype dtype, int tag);

error sendToAllNeighbours(int nbours[4],Buffer *data[4],MPI_Request reqs[4], MPI_Datatype dtype);

error recvFromAllNeighbours(int nbours[4],Buffer *data[4], MPI_Datatype dtype);

#endif
