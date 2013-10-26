#include <time.h>
#include <mpi.h>
#include <cassert>
#include <stdlib.h>
#include <iostream>
#include "Model.h"
#include "Statistic.h"

using namespace std;

#define WIDTH 250
#define HEIGHT 250

// natural death rate for humans about once every 60 years
#define NATURAL_DEATH_RISK (6.5/(1000.0*365.25))
// simplest possible birth control mechanism
#define NATURAL_BIRTH_PROB (13.3 / (365.25 * 1000.0))

#define POP_DENSITY 0.1
// Probabiblity of getting your brain eaten when encountering a zombie
#define BRAIN_EATING_PROB 0.7
#define INFECTED_TO_ZOMBIE_PROB (1.0/5.0)
#define ZOMBIE_DECOMPOSITION_RISK (1.0/75.0)
#define HUMAN_MOVE_PROB 0.4
#define ZOMBIE_MOVE_PROB 0.2 

int main(int argc, char *argv[])
{
    error err = MPI_Init(&argc, &argv);
    assert(err == MPI_SUCCESS);
    int rank = 0,size  =0;
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    assert(err == MPI_SUCCESS);
    err = MPI_Comm_size(MPI_COMM_WORLD, &size);
    assert(size == PROC_WIDTH*PROC_HEIGHT);
    if (argc != 7){
        cout << "please give 1 int and 5 doubles as arguments for this executable in the order:"<<endl
            << "iterations" << endl
            << "brainEatingProb" << endl
            << "infectedToZombieProb" << endl
            << "zombieDecompositionRisk" << endl
            << "humanMoveProb" << endl
            << "zombieMoveProb" << endl;
            return 0;
    }

    int nbIterations = atoi(argv[1]);
    double brainEatingProb = atof(argv[2]);
    double infectedToZombieProb = atof(argv[3]);
    double zombieDecompositionRisk = atof(argv[4]);
    double humanMoveProb = atof(argv[5]);
    double zombieMoveProb = atof(argv[6]);

    Model m = Model(WIDTH,HEIGHT,rank,NATURAL_BIRTH_PROB,NATURAL_DEATH_RISK,POP_DENSITY, brainEatingProb,infectedToZombieProb,
            zombieDecompositionRisk,humanMoveProb,zombieMoveProb,true);

    
    // Moving 
    double before;
    if (rank == ROOT_NODE){
        before = MPI_Wtime();
    }
    Statistic** stats = m.moveAll_mpi(nbIterations);
    
    //Statistic** stats = m.moveAll_omp_mpi(nbIterations);
    
    if (rank == ROOT_NODE){
        double seconds = MPI_Wtime()-before;
        cout << "Successfully ran " << nbIterations << " iterations with " << PROC_WIDTH * PROC_HEIGHT
        << " processors and " << NUM_THREADS << " threads per processor"
        <<" in "<<seconds <<" seconds "<<endl;
        printStatsCsv(stats,nbIterations);
        
    }

    err = MPI_Finalize();
    assert(err == MPI_SUCCESS);    
    return 0;
}
