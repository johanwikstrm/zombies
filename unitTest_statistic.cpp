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
    stat.mpi_reduce();
    if (rank == ROOT_NODE){
    	assert(stat.nZombies == size * 5);
    }
    
    
    err = MPI_Finalize();
	assert(err == MPI_SUCCESS);    
	return 0;
}