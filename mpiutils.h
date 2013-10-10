#ifndef MPIUTILS_H
#define MPIUTILS_H 
#include "constants.h"
#include "Buffer.h"
#include "Array.h"
#include "Matrix.h"

// determines your four neighbours in a coordinate system 
// where y grows downwards and x grows to the right
// The universe also wraps around
// neighbours = [up,right,down,left]

const char * dirstr(DIRECTION d);


void neighbours(int x,int y, int width, int height, int nbours[4]);
int toX(int rank);

int toY(int rank);

error recvFromNeighbour(int from,Buffer* to,MPI_Datatype dtype,int tag);

error sendToNeighbour(int dest, Buffer *data,MPI_Request *request, MPI_Datatype dtype, int tag);

error sendToAllNeighbours(int nbours[4],Buffer *data[4],MPI_Request reqs[4], MPI_Datatype dtype);

error recvFromAllNeighbours(int nbours[4],Buffer *data[4], MPI_Datatype dtype);

// Swaps all the necessary rows and columns of matrix
// with all neighbours, in total 8 rows and columns
// returns the number of collisions
int swapAll(int nbours[4],Matrix& matrix);

#endif