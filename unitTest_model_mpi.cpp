#include <cassert>
#include <mpi.h>
#include "Model.h"
#include <iostream>
#include "constants.h"
#include "Statistic.h"

using namespace std;

int main(int argc, char *argv[]){

    //Model(int width,int height,int procRank,double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
    //brainEatingProb,double infectedToZombieProb,double zombieDecompositionRisk, double humanMoveProb, double zombieMoveProb);
    error err = MPI_Init(&argc, &argv);
    assert(err == MPI_SUCCESS);
    
    int rank;
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    assert(err == MPI_SUCCESS);
    Model m1 = Model(3,3,rank,0,0,0, 1,0,0,1,0,true);
    // should have only 2 zombies in it
    assert(m1.getCount(ZOMBIE)==2);
    assert(m1.getCount(HUMAN)==0);
    assert(m1.getCount(INFECTED)==0);
    assert(m1.getCount(EMPTY)==7);

    Model m2 = Model(10,5,rank,0,0,1,1,0.5,0,0.5,0.5,true);

    Statistic ** stats = m2.moveAll_mpi(1);
    assert(stats[0]->allAboveZero());
    if (rank == ROOT_NODE && stats[0]->sum() != 10*5*4){
        cout << "Expected sum to be " << 10*5*4 <<" got "<<stats[0]->sum()<<endl<<flush;
        m2.print();
        assert(false);
    }
    stats = m2.moveAll_mpi(10);
    assert(stats[0]->allAboveZero());
    // should create a model with a declining population of humans!!
    Model m3 = Model(100,100,rank,0,0.1,0.1,0,0,1,0,0);
    int prev = m3.getCount(HUMAN);
    int accBefore;
    m3.moveAll_mpi(1000);
    if (prev <= m3.getCount(HUMAN)){
        cout<<"prev == "<<prev<<" after == "<<m3.getCount(HUMAN)<<endl<<flush;
    }

    err = MPI_Finalize();
    assert(err == MPI_SUCCESS);    
}
