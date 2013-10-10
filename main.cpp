#include <time.h>
#include <mpi.h>
#include <cassert>
#include <stdlib.h>
#include <iostream>
#include "Model.h"

using namespace std;

#define WIDTH 10   
#define HEIGHT 10

// natural death rate for humans about once every 60 years
#define NATURAL_DEATH_RISK (1.0/(60.0*365.0))
// simplest possible birth control mechanism
#define NATURAL_BIRTH_PROB (1.0/(60.0*365.0))

#define POP_DENSITY 0.3
// Probabiblity of getting your brain eaten when encountering a zombie
#define BRAIN_EATING_PROB 1
#define INFECTED_TO_ZOMBIE_PROB (1.0/10.0)
#define ZOMBIE_DECOMPOSITION_RISK 0.1
#define HUMAN_MOVE_PROB 0.7
#define ZOMBIE_MOVE_PROB 0.5

#define ITERATIONS 10


int main(int argc, char *argv[])
{
	error err = MPI_Init(&argc, &argv);
    assert(err == MPI_SUCCESS);
    int rank;
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    assert(err == MPI_SUCCESS);
    Model m = Model(WIDTH,HEIGHT,rank,NATURAL_BIRTH_PROB,NATURAL_DEATH_RISK,POP_DENSITY, BRAIN_EATING_PROB,INFECTED_TO_ZOMBIE_PROB,
        ZOMBIE_DECOMPOSITION_RISK,HUMAN_MOVE_PROB,ZOMBIE_MOVE_PROB);

    //m.moveAll(ITERATIONS);
    m.moveAll(ITERATIONS);
    //m.moveAll_omp(ITERATIONS);

    err = MPI_Finalize();
	assert(err == MPI_SUCCESS);    
    return 0;
}
