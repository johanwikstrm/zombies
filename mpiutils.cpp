#include <iostream>
#include "mpiutils.h"

using namespace std;

#define BUFFER_TAG 12345

// determines your four neighbours in a coordinate system 
// where y grows downwards and x grows to the right
// The universe also wraps around
// neighbours = [up,right,down,left]
void neighbours(int x,int y, int width, int height, int nbours[4]){
	nbours[UP] = ((y+height-1)%height)*width+x;
	nbours[RIGHT] = y*width+(x+1)%width;
	nbours[DOWN] = ((y+1)%height)*width+x;
	nbours[LEFT] = y*width + (x-1+width)%width;
}

int x(int rank){
	return rank % PROC_WIDTH;
}

int y(int rank){
	return rank / PROC_WIDTH;
}

error recvFromNeighbour(int from,Buffer* to,MPI_Datatype dtype){
	MPI_Status status;
	return MPI_Recv(to->rawData(), to->count(), dtype, from, BUFFER_TAG ,MPI_COMM_WORLD, &status);
}

error sendToNeighbour(int dest, Buffer *data,MPI_Request *request, MPI_Datatype dtype){
	return MPI_Isend(data->rawData(), data->count(), dtype , dest, BUFFER_TAG ,MPI_COMM_WORLD, request);
}

error sendToAllNeighbours(Buffer *data[4],int nbours[4],MPI_Request reqs[4], MPI_Datatype dtype){
	int direction;
	error err = MPI_SUCCESS;
	for (direction = UP; direction <= LEFT; direction++){
		int e;
		e = sendToNeighbour(nbours[direction],data[direction],&reqs[direction],dtype);
		if (e != MPI_SUCCESS){
			err = e;
		}
	}
	return err;
}