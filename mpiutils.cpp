#include <iostream>
#include <cassert>
#include <cstdlib>
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

int toX(int rank){
	return rank % PROC_WIDTH;
}

int toY(int rank){
	return rank / PROC_WIDTH;
}

error recvFromNeighbour(int from,Buffer* to,MPI_Datatype dtype,int tag){
	MPI_Status status;
	return MPI_Recv(to->rawData(), to->count(), dtype, from, tag , 
                        MPI_COMM_WORLD, &status);
}

error sendToNeighbour(int dest, Buffer *data, MPI_Request *request, MPI_Datatype dtype, int tag){
	return MPI_Isend(data->rawData(), data->count(), dtype , dest, tag, MPI_COMM_WORLD, request);
}

error sendToAllNeighbours(int nbours[4], Buffer *data[4], MPI_Request reqs[4], MPI_Datatype dtype){
	int direction;
	error err = MPI_SUCCESS;
	for (direction = UP; direction <= LEFT; direction++){
		int e;
		/*if (nbours[UP] == 0)
		cout << "Sending data to " << dirstr((DIRECTION)direction)
			<< "(rank == " << nbours[direction]
			<< ")data[1] == " << (*(data[direction]->toArray()))(1)->getKind() << endl;
			*/
		// As tag I am sending the direction that the receiver should expect to receive the message from
		// i.e. if I am sending to the RIGHT the receiver should expect this message from the LEFT
		// Without this tag, sometimes we can't separate messages sent from the left and messages sent from the right
		e = sendToNeighbour(nbours[direction], data[direction], 
                                    &reqs[direction], dtype, 
                                    opposite((DIRECTION)direction));
		if (e != MPI_SUCCESS){
			err = e;
		}
	}
	return err;
}

error recvFromAllNeighbours(int nbours[4], Buffer *to[4], MPI_Datatype dtype){
	error err = MPI_SUCCESS;
	for (int direction = UP; direction <= LEFT; direction++){
		error e;
		e = recvFromNeighbour(nbours[direction],to[direction],dtype,direction);
		/*if (nbours[UP] == 0)
		cout << "Received data from " << dirstr((DIRECTION)direction)
			<< "(rank == " << nbours[direction]
			<< ") data[1] == " << (*(to[direction]->toArray()))(1)->getKind() << endl;
			*/
		if (e != MPI_SUCCESS){
			err = e;
		}
	}
	return err;
}

void initBuffers(Matrix& matrix, Buffer** send, Buffer** recv, int offset){
	Array** edges = matrix.toSend(offset);
	assert(edges[UP]->getSize() == matrix.getWidth()-2);
	assert(edges[LEFT]->getSize() == matrix.getHeight()-2);
	for (int i = 0; i < 4; i++){
            // Copy the edges arrays
            send[i] = new Buffer(*(edges[i]));
            // initialise
            recv[i] = new Buffer(edges[i]->getSize());
            delete edges[i];
        }
}

void freeBuffers(Buffer ** send, Buffer ** recv){
	for (int i = 0; i < 4; i++){
		delete send[i];
		delete recv[i];
	}
}

Array ** buffersToArrays(Buffer **received){
	Array ** arrays = (Array**)calloc(4,sizeof(Array*));
	for (int i = 0; i < 4; i++){
		arrays[i] = received[i]->toArray();
	}
	return arrays;
}
/*
	H X X X X X X H 
	X H H H H H H X 
	X H H H H H H X 
	X H H H H H H X 
	X H H H H H H X 
	X H H H H H H X 
	H X X X X X X H 
*/
// NOTE NOTE NOTE, this works because we do async sends and syncronous recvs.
// If we want to do something "smarter" and time saving
// we need to use MPI_TAGs a lot more
int swapAll(int nbours[4], Matrix& matrix){
	error err;
	MPI_Datatype dtype;
	MPI_Request reqs[4];
	err = Buffer::datatype(&dtype);
	assert(err == MPI_SUCCESS);
	err = MPI_Type_commit(&dtype);
        assert(err== MPI_SUCCESS);
	Buffer **to, **from;
	to = (Buffer**)calloc(4,sizeof(Buffer*));
	from = (Buffer**)calloc(4,sizeof(Buffer*));
	// Send my outer ones
	initBuffers(matrix,from,to,0);
	/*cout << "1. "<<
		nbours[UP]<<","<<
		nbours[RIGHT]<<","<<
		nbours[DOWN]<<","<<
		nbours[LEFT]<<" <- "<<
		nbours[LEFT]+1
		<<"\n"<< flush;*/
	err = sendToAllNeighbours(nbours,from,reqs,dtype);
	assert(err == MPI_SUCCESS);
	// (handle collisions?)
	// Insert their outer ones into my inner ones 
	err = recvFromAllNeighbours(nbours, to, dtype);
	assert(err == MPI_SUCCESS);
	assert(to[DOWN]->count() == matrix.getWidth()-2);
	int collisions = matrix.insertWithCollisions(buffersToArrays(to), 1);
	//cout << "Collisons(first): " << collisions << endl << flush;
	// Send my inner ones
	initBuffers(matrix,from,to,1);
	//cout << "Sending to all neighbours 2\n"<< flush;
	err = sendToAllNeighbours(nbours,from,reqs,dtype);
	assert(err == MPI_SUCCESS);
	// Insert their inner ones into my outer ones(collisions should be 0 by now)
	//cout << "recv from all neighbours 2\n"<< flush;
	err = recvFromAllNeighbours(nbours,to,dtype);
	assert(err == MPI_SUCCESS);
	// TODO insertWithoutCollisions
	collisions += matrix.insertWithCollisions(buffersToArrays(to), 0);

	// Giant memory leaks
	freeBuffers(to,from);
	free(to);
	free(from);
	return collisions;
}
