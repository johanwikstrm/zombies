#ifndef MPIUTILS_H
#define MPIUTILS_H 
#include "constants.h"
#include "Buffer.h"
#include "Darray.h"

// determines your four neighbours in a coordinate system 
// where y grows downwards and x grows to the right
// The universe also wraps around
// neighbours = [up,right,down,left]

void neighbours(int x,int y, int width, int height, int nbours[4]);
int x(int rank);

int y(int rank);

error recvFromNeighbour(int from,Buffer* to,MPI_Datatype dtype);

error sendToNeighbour(int dest, Buffer *data,MPI_Request *request, MPI_Datatype dtype);

error sendToAllNeighbours(Buffer *data[4],int nbours[4],MPI_Request reqs[4]);

#endif