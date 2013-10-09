#include <iostream>
#include <cassert>
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

const char * dirstr(DIRECTION d){
	switch(d){
		case UP:
		return "UP";
		break;
		case RIGHT:
		return "RIGHT";
		break;
		case DOWN:
		return "DOWN";
		break;
		case LEFT:
		return "LEFT";
		break;
	}
	return "UNKNOWN DIRECTION";
}

DIRECTION opposite(DIRECTION d){
	assert(d >= 0 && d < 4);
	return (DIRECTION)(((int)d + 2) % 4);
}

int x(int rank){
	return rank % PROC_WIDTH;
}

int y(int rank){
	return rank / PROC_WIDTH;
}

error recvFromNeighbour(int from,Buffer* to,MPI_Datatype dtype,int tag){
	MPI_Status status;
	return MPI_Recv(to->rawData(), to->count(), dtype, from, tag ,MPI_COMM_WORLD, &status);
}

error sendToNeighbour(int dest, Buffer *data,MPI_Request *request, MPI_Datatype dtype, int tag){
	return MPI_Isend(data->rawData(), data->count(), dtype , dest, tag,MPI_COMM_WORLD, request);
}

error sendToAllNeighbours(int nbours[4],Buffer *data[4],MPI_Request reqs[4], MPI_Datatype dtype){
	int direction;
	error err = MPI_SUCCESS;
	for (direction = UP; direction <= LEFT; direction++){
		int e;
		/*if (nbours[UP] == 0)
		cout << "Sending data to " << dirstr((DIRECTION)direction)
			<< "(rank == " << nbours[direction]
			<< ")data[1] == " << (*(data[direction]->toDarray()))(1)->kind() << endl;
			*/
		// As tag I am sending the direction that the receiver should expect to receive the message from
		// i.e. if I am sending to the RIGHT the receiver should expect this message from the LEFT
		// Without this tag, sometimes we can't separate messages sent from the left and messages sent from the right
		e = sendToNeighbour(nbours[direction],data[direction],&reqs[direction],dtype,opposite((DIRECTION)direction));
		if (e != MPI_SUCCESS){
			err = e;
		}
	}
	return err;
}

error recvFromAllNeighbours(int nbours[4],Buffer *to[4], MPI_Datatype dtype){
	error err = MPI_SUCCESS;
	for (int direction = UP; direction <= LEFT; direction++){
		error e;
		e = recvFromNeighbour(nbours[direction],to[direction],dtype,direction);
		/*if (nbours[UP] == 0)
		cout << "Received data from " << dirstr((DIRECTION)direction)
			<< "(rank == " << nbours[direction]
			<< ") data[1] == " << (*(to[direction]->toDarray()))(1)->kind() << endl;
			*/
		if (e != MPI_SUCCESS){
			err = e;
		}
	}
	return err;
}