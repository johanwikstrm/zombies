#include <mpi.h>
#include "constants.h"
#include "Statistic.h"
#include <assert.h>


int main(int argc, char *argv[]){
	error err = MPI_Init(&argc, &argv);
    assert(err == MPI_SUCCESS);
	int rank,size;
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    assert(err== MPI_SUCCESS);
    err = MPI_Comm_size(MPI_COMM_WORLD, &size);
    assert(err== MPI_SUCCESS);
    Statistic stat = Statistic();
    stat.nZombies = 5;
    stat.nEmpty = 4;
    stat.nHumans = 3;
    stat.nInfected = 2;
    stat.mpi_reduce();
    if (rank == ROOT_NODE){
    	assert(stat.nZombies == size * 5);
        assert(stat.nEmpty == size * 4);
        assert(stat.nHumans == size * 3);
        assert(stat.nInfected == size * 2);
    }
    
    
    err = MPI_Finalize();
	assert(err == MPI_SUCCESS);    
	return 0;
}