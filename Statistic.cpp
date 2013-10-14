#include <mpi.h>
#include <assert.h>
#include "Statistic.h"
#include "constants.h"
#include <iostream>
#include <sstream>


using namespace std;

Statistic::Statistic(){
	nZombies = nEmpty = nHumans = nInfected = 0;
}

Statistic::Statistic(Matrix& m){
	nZombies = m.getCount(ZOMBIE);
	nHumans = m.getCount(HUMAN);
	nEmpty = m.getCount(EMPTY);
	nInfected = m.getCount(INFECTED);
}

void Statistic::mpi_reduce(){
	int gZombies,gHumans,gEmpty,gInfected;
	gZombies = gHumans = gEmpty = gInfected = 0;
	error err = MPI_Reduce(&nZombies, &gZombies, 1, MPI_INT, MPI_SUM, ROOT_NODE,MPI_COMM_WORLD);
	assert(err == MPI_SUCCESS);
	err = MPI_Reduce(&nHumans, &gHumans, 1, MPI_INT, MPI_SUM, ROOT_NODE,MPI_COMM_WORLD);
	assert(err == MPI_SUCCESS);
	err = MPI_Reduce(&nEmpty, &gEmpty, 1, MPI_INT, MPI_SUM, ROOT_NODE,MPI_COMM_WORLD);
	assert(err == MPI_SUCCESS);
	err = MPI_Reduce(&nInfected, &gInfected, 1, MPI_INT, MPI_SUM, ROOT_NODE,MPI_COMM_WORLD);
	assert(err == MPI_SUCCESS);
	nZombies = gZombies;
}

string Statistic::toCsv(){
	stringstream t;
	t << nEmpty<< ' ' << nHumans <<' '<< nZombies<<' ' << nInfected << endl;
	return t.str();
}

void printStatsCsv(Statistic ** stats, int n){
	cout <<"EMPTY HUMANS ZOMBIES INFECTED\n";
	for (int i = 0; i < n; i++){
		cout << stats[i]->toCsv();
	}
}
