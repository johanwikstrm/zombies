using namespace std;
#include <iostream>
#include <cassert>
#include <stdlib.h>
#include "Buffer.h"
#include "Matrix.h"
#include "constants.h"
#include "mpiutils.h"


int main(int argc, char *argv[])
{
    Buffer buf = Buffer(10);
    assert(buf.count() == 10);
    
    Array d = Array(100);
    assert(d(55)->getKind() == EMPTY);
    d.set(55,HUMAN);
    assert(d(55)->getKind() == HUMAN);    
    
    Buffer buf2 = Buffer(d);
    
    assert(buf2.count() == 100);
    assert((*buf2.toArray())(55)->getKind() == HUMAN);
    assert((*buf2.toArray())(54)->getKind() == EMPTY);

    Array d2 = Array(2);
    d2.set(0,ZOMBIE);
    Buffer from = Buffer(d2);
    Buffer to = Buffer(d2.getSize());
    assert(from.count() == 2);
    assert(to.count() == 2);
    assert((*from.toArray())(0)->getKind() == ZOMBIE);
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
    if (PROC_HEIGHT*PROC_WIDTH>1){
        if (rank == 0){
        err = sendToNeighbour(1,&from,&request,dtype,123);
        //err = MPI_Isend(from.rawData(), from.count(), dtype, 1, MPI_TAG ,MPI_COMM_WORLD, &request);
        assert(err == MPI_SUCCESS); 
        }else if (rank == 1){
            err = recvFromNeighbour(0,&to,dtype,123);
            //err = MPI_Recv(to.rawData(), to.count(), dtype, 0, MPI_TAG,MPI_COMM_WORLD, &status);
            assert(err == MPI_SUCCESS);
            assert((*to.toArray())(0)->getKind() == ZOMBIE);
            assert((*to.toArray())(1)->getKind() == EMPTY);
        }    
    }
    
    /* this is what all matrices should do
        E E E E      E E Z E
        E H E E      E H E H
        E E E E  ->  E E E E
        E E Z E      Z E Z E
        E E E E      E H E E 
    */

    Matrix myMatr = Matrix(5,4);
    myMatr.set(1,1,HUMAN,NULL);
    myMatr.set(2,3,ZOMBIE,NULL);
    int nbours[4];
    MPI_Request reqs[4];

    neighbours(toX(rank),toY(rank),PROC_WIDTH,PROC_HEIGHT,nbours);
    //neighbours(nbours);
    Array **toSend = myMatr.toSend(1,false);
    assert(toSend[UP]->getSize() == 2);
    assert((*(toSend[UP]))(0)->getKind() == HUMAN);
    assert((*(toSend[DOWN]))(1)->getKind() == ZOMBIE);
    assert(toSend[LEFT]->getSize() == 3);
    Buffer** bufs = (Buffer**)calloc(4,sizeof(Buffer*));
    Buffer** bufs2 = (Buffer**)calloc(4,sizeof(Buffer*));
    for (int i = 0; i < 4; i++){
        bufs[i] = new Buffer(*(toSend[i]));
        bufs2[i] = new Buffer(toSend[i]->getSize());
    }
    assert(bufs[DOWN]->count() == 2);
    assert(bufs[LEFT]->count() == 3);
    assert(bufs2[LEFT]->count() == 3);
    err = sendToAllNeighbours(nbours,bufs,reqs,dtype);
    assert(err == MPI_SUCCESS);
    err = recvFromAllNeighbours(nbours,bufs2,dtype);
    assert(err == MPI_SUCCESS);
    assert((*(bufs2[DOWN]->toArray()))(0)->getKind() == HUMAN);
    assert((*(bufs2[DOWN]->toArray()))(1)->getKind() == EMPTY);
    assert((*(bufs2[UP]->toArray()))(0)->getKind() == EMPTY);
    assert((*(bufs2[UP]->toArray()))(1)->getKind() == ZOMBIE);
    assert((*(bufs2[LEFT]->toArray()))(1)->getKind() == EMPTY);
    assert((*(bufs2[LEFT]->toArray()))(2)->getKind() == ZOMBIE);
    assert((*(bufs2[RIGHT]->toArray()))(2)->getKind() == EMPTY);
    assert((*(bufs2[RIGHT]->toArray()))(0)->getKind() == HUMAN);

    // Full test with matrix
    Matrix matrix = Matrix(6,7);
    matrix.set(0,2,ZOMBIE,NULL);
    matrix.set(1,1,HUMAN,NULL);
    matrix.set(4,4,INFECTED,NULL);
    /*
        E E E E E E E       E E E E I E E     E E E E I E E
        E H E E E E E       E H E E E E H     E E E E E E E
        Z E E E E E E       Z E E E E Z E     Z H E E E Z H
        E E E E E E E  ->   E E E E E E E --> E E E E E E E
        E E E E I E E       E E E E I E E     E E E E I E E
        E E E E E E E       E H E E E E E     E E E E E E E
    */
    assert(matrix.getCount(ZOMBIE) == 1);
    assert(matrix.getCount(HUMAN) == 1);
    assert(matrix.getCount(INFECTED) == 1);
    swapAll(nbours,matrix,false);
    assert(matrix.getCount(ZOMBIE) == 2);
    assert(matrix.getCount(HUMAN) == 3);
    assert(matrix.getCount(INFECTED) == 2);
    // Human moving down
    matrix.set(1,1,EMPTY,NULL);
    matrix.set(1,2,HUMAN,NULL);
    matrix(1,2)->setMoveFlag(true);
    // Infected moving left
    matrix.set(4,4,EMPTY,NULL);
    matrix.set(3,4,INFECTED,NULL);
    matrix(3,4)->setMoveFlag(true);
    // emulating that all of them move
    matrix(5,2)->setMoveFlag(true);
    assert(matrix.getCount(HUMAN) == 3);
    if (rank == 0){
        //matrix.printMoveFlags();
    }
    swapAll(nbours,matrix,true);
    if (rank == 0){
        //matrix.printMoveFlags();
    }
    assert(matrix.getCount(HUMAN) == 2);


    // Trying to get seg fault by using a lot of collisions
    Matrix matrix2 = Matrix(4,4);
    matrix2.set(0,0,HUMAN,NULL);
    matrix2.set(0,1,HUMAN,NULL);
    matrix2.set(0,2,HUMAN,NULL);
    matrix2.set(0,3,HUMAN,NULL);
    matrix2.set(1,0,HUMAN,NULL);
    matrix2.set(1,1,HUMAN,NULL);
    matrix2.set(1,2,HUMAN,NULL);
    matrix2.set(1,3,HUMAN,NULL);
    matrix2.set(2,0,HUMAN,NULL);
    matrix2.set(2,1,HUMAN,NULL);
    matrix2.set(2,2,HUMAN,NULL);
    matrix2.set(2,3,HUMAN,NULL);
    matrix2.set(3,0,HUMAN,NULL);
    matrix2.set(3,1,HUMAN,NULL);
    matrix2.set(3,2,HUMAN,NULL);
    matrix2.set(3,3,HUMAN,NULL);
    /*
        H H H H        H H H H
        H H H H        H H H H
        H H H H        H H H H
        H H H H   ->   H H H H 
    */
    assert(matrix2.getCount(HUMAN) == 16);
    for (int i = 0; i < 100; i++){
        swapAll(nbours,matrix,false);
    }
    assert(matrix2.getCount(HUMAN) == 16);
    // Handling collisions
    Matrix matrix3 = Matrix(5,6);
    
    matrix3.set(1,1,INFECTED,NULL);
    matrix3.set(1,4,ZOMBIE,NULL);
    // Collision handling
    /*
        E E E E E E      E E E E E E
        E I E E E E      E I E E E I
        E E E E E E  ->  E Z E E E Z
        E E E E E E      E E E E E E
        E Z E E E E      E I E E E E
    */
    assert(matrix3.getCount(INFECTED) == 1);
    assert(matrix3.getCount(ZOMBIE) == 1);
    int collisions = swapAll(nbours,matrix3,false);
    
    //cout << "Collisions " << collisions << endl << flush;
    //matrix3.print();
    //assert(collisions == 1);
    //assert(matrix3.getCount(INFECTED) == 3);
    //assert(matrix3.getCount(ZOMBIE) == 2);
    
    Matrix matrix4 = Matrix(100,100);
    if (rank == 0){
        //cout << "looking for deadlocks\n"<<flush;    
    }
    for (int i = 0; i < 100; i++){
        if (rank == 0 && i % 10 == 0 ){
            //cout << "iteration "<<i<<endl<<flush;
        }
        for (int x = 0; x < 100; x++){
            for (int y = 0; y < 100; y++){
                double r = drand48();
                if (r < 0.25){
                    matrix4.set(x,y,HUMAN,NULL);
                }else if(r < 0.5){
                    matrix4.set(x,y,ZOMBIE, NULL);
                }
                else if(r < 0.75){
                    matrix4.set(x,y,INFECTED, NULL);
                }else{
                    matrix4.set(x,y,EMPTY, NULL);
                }
            }
        }
        // might deadlock, who knows
        swapAll(nbours,matrix4,false);
    }


    err = MPI_Finalize();
	assert(err == MPI_SUCCESS);    
}
