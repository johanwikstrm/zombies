#include <cassert>
#include <mpi.h>
#include "Model.h"
#include <iostream>
#include "constants.h"
#include "Statistic.h"

using namespace std;

int main(int argc, char *argv[]){

    // //Model(int width,int height,int procRank,double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
    // //brainEatingProb,double infectedToZombieProb,double zombieDecompositionRisk, double humanMoveProb, double zombieMoveProb);
     error err = MPI_Init(&argc, &argv);
     assert(err == MPI_SUCCESS);
    
     int rank;
     err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     assert(err == MPI_SUCCESS);
    // Model m1 = Model(5,5,rank,0,0,0,1,0,0,1,0,true);
    // // should have only 2 zombies in it
    // assert(m1.getCount(ZOMBIE)==2);
    // assert(m1.getCount(HUMAN)==0);
    // assert(m1.getCount(INFECTED)==0);
    // assert(m1.getCount(EMPTY)==7);

    // Model m2 = Model(10,5,rank,0,0,1,1,0.5,0,0.5,0.5,true);

    // Statistic ** stats = m2.moveAll_mpi(10);
    // if (rank == ROOT_NODE && stats[0]->sum() != 8*3*PROC_WIDTH*PROC_HEIGHT){
    //     assert(stats[0]->allAboveZero());
    //     cout << "Expected sum to be " << 10*5*4 <<" got "<<stats[0]->sum()<<endl<<flush;
    //     m2.print();
    //     assert(false);
    // }
    
    // should create a model with a declining population of humans!!
    Model m3 = Model(100,100,rank,0.0,0.0,0.1,0.7,0.2,1,0.5,0,true);
    int prev = m3.getCount(HUMAN);
    int iter = 2000;
    m3.moveAll_mpi(iter);
    int after1000 = m3.getCount(HUMAN);
    cout<<"prev == "<<prev<<" after "<<iter<<" == "<<after1000<<endl<<flush;
    if (prev < m3.getCount(HUMAN)){
        cout << "Impossible that the population increases without birth or death rate"<<endl<<flush;
        assert(false);
    }
    err = MPI_Finalize();
    assert(err == MPI_SUCCESS);    
}
