using namespace std;
#include <iostream>
#include <cassert>
#include "stdlib.h"
#include "Buffer.h"
#include "Dmatrix.h"
#include "constants.h"
#include "mpiutils.h"


int main(int argc, char *argv[])
{
    Buffer buf = Buffer(10);
    assert(buf.count() == 10);

    Darray d = Darray(100);
    d(55) = new Cell(HUMAN);
    Buffer buf2 = Buffer(d);
    assert(buf2.count() == 100);
    assert((*buf2.toDarray())(55)->kind() == HUMAN);
    assert((*buf2.toDarray())(54)->kind() == EMPTY);

    Darray d2 = Darray(2);
    d2(0) = new Cell(ZOMBIE);
    Buffer from = Buffer(d2);
    Buffer to = Buffer(d2.getSize());
    assert(from.count() == 2);
    assert(to.count() == 2);
    assert((*from.toDarray())(0)->kind() == ZOMBIE);
    error err = MPI_Init(&argc, &argv);
    assert(err == MPI_SUCCESS);
    
    int rank;
    MPI_Request request;
    MPI_Datatype dtype;
    err = from.datatype(&dtype);
    assert(err == MPI_SUCCESS);
    err = MPI_Type_commit(&dtype);
    assert(err== MPI_SUCCESS);
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    assert(err == MPI_SUCCESS);
    if (rank == 0){
        err = sendToNeighbour(1,&from,&request,dtype,123);
    	//err = MPI_Isend(from.rawData(), from.count(), dtype, 1, MPI_TAG ,MPI_COMM_WORLD, &request);
    	assert(err == MPI_SUCCESS);	
    }else if (rank == 1){
		err = recvFromNeighbour(0,&to,dtype,123);
        //err = MPI_Recv(to.rawData(), to.count(), dtype, 0, MPI_TAG,MPI_COMM_WORLD, &status);
		assert(err == MPI_SUCCESS);
		assert((*to.toDarray())(0)->kind() == ZOMBIE);
		assert((*to.toDarray())(1)->kind() == EMPTY);
    }

    /* this is what all matrices should do
        E E E E      E E Z E
        E H E E      E H E H
        E E E E  ->  E E E E
        E E Z E      Z E Z E
        E E E E      E H E E 
    */

    Dmatrix myMatr = Dmatrix(5,4);
    myMatr.set(1,1,HUMAN);
    myMatr.set(2,3,ZOMBIE);
    int nbours[4];
    MPI_Request reqs[4];

    neighbours(toX(rank),toY(rank),PROC_WIDTH,PROC_HEIGHT,nbours);

    //neighbours(nbours);
    Darray **toSend = myMatr.toSend(1);
    assert(toSend[UP]->getSize() == 4);
    assert((*(toSend[UP]))(1)->kind() == HUMAN);
    assert((*(toSend[UP]))(0)->kind() == EMPTY);
    assert((*(toSend[DOWN]))(2)->kind() == ZOMBIE);
    assert(toSend[LEFT]->getSize() == 5);
    Buffer** bufs = (Buffer**)calloc(4,sizeof(Buffer*));
    Buffer** bufs2 = (Buffer**)calloc(4,sizeof(Buffer*));
    for (int i = 0; i < 4; i++){
        bufs[i] = new Buffer(*(toSend[i]));
        bufs2[i] = new Buffer(toSend[i]->getSize());
    }
    assert(bufs[DOWN]->count() == 4);
    assert(bufs[LEFT]->count() == 5);
    assert(bufs2[LEFT]->count() == 5);
    err = sendToAllNeighbours(nbours,bufs,reqs,dtype);
    assert(err == MPI_SUCCESS);
    err = recvFromAllNeighbours(nbours,bufs2,dtype);
    assert(err == MPI_SUCCESS);
    assert((*(bufs2[DOWN]->toDarray()))(1)->kind() == HUMAN);
    assert((*(bufs2[DOWN]->toDarray()))(0)->kind() == EMPTY);
    assert((*(bufs2[UP]->toDarray()))(1)->kind() == EMPTY);
    assert((*(bufs2[UP]->toDarray()))(2)->kind() == ZOMBIE);
    assert((*(bufs2[LEFT]->toDarray()))(1)->kind() == EMPTY);
    assert((*(bufs2[LEFT]->toDarray()))(3)->kind() == ZOMBIE);
    assert((*(bufs2[RIGHT]->toDarray()))(2)->kind() == EMPTY);
    assert((*(bufs2[RIGHT]->toDarray()))(1)->kind() == HUMAN);

    // Full test with matrix
    Dmatrix matrix = Dmatrix(6,7);
    matrix.set(0,2,ZOMBIE);
    matrix.set(1,1,HUMAN);
    matrix.set(4,4,INFECTED);
    /*
        E E E E E E E       E E E E I E E
        E H E E E E E       E H E E E E H
        Z E E E E E E       Z E E E E Z E
        E E E E E E E  ->   E E E E E E E
        E E E E I E E       E E E E I E E
        E E E E E E E       E H E E E E E
    */
    assert(matrix.getCount(ZOMBIE) == 1);
    assert(matrix.getCount(HUMAN) == 1);
    assert(matrix.getCount(INFECTED) == 1);
    swapAll(nbours,matrix);
    assert(matrix.getCount(ZOMBIE) == 2);
    assert(matrix.getCount(HUMAN) == 3);
    assert(matrix.getCount(INFECTED) == 2);



    err = MPI_Finalize();
	assert(err == MPI_SUCCESS);    
}
