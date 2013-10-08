#include <iostream>
#include <stdlib.h>
#include <omp.h>
using namespace std;

#define STEPS	100
#define SIZE	1000
#define MOVE	0.4

bool **CreateMesh(int I, int J) {
	bool **Mesh = new bool *[I];
	for (int i = 0; i < I; i++) {
		Mesh[i] = new bool[J];
		for (int j = 0; j < J; j++) Mesh[i][j] = false;
	}
	return(Mesh);
}

// spin lock
// atomic - one process at a time section
void lock(int i, bool *locks) {
	for (bool locked = false; locked == false; /*NOP*/) {
#pragma omp critical (LockRegion)
		{
			locked = !locks[i-1] && !locks[i] && !locks[i+1];
			if (locked) {
				locks[i-1] = true; locks[i] = true; locks[i+1] = true;
			}
		}
	}
}

void unlock(int i, bool *locks) {
#pragma omp critical (LockRegion)
{
	locks[i-1] = false; locks[i] = false; locks[i+1] = false;
}
}


// parallelize by rows
// probleme a la frontiere -> locking
// locking must be expensive -> ROW LOCKING
// a lock variable for each row
// il faut aue chaque threads ait au mois 5 lignes (pout eviter overlapping)

int main(int argc, char **argv) {
	srand48(8767134);
	// LOCK = true -> the row is used
	bool *locks = new bool[SIZE + 2];
	for (int i = 0; i < SIZE + 2; i++) locks[i] = false;
	bool **MeshA = CreateMesh(SIZE + 2, SIZE + 2);
	bool **MeshB = CreateMesh(SIZE + 2, SIZE + 2);
	for (int i = 1; i <= SIZE; i++) {
		for (int j = 1; j <= SIZE; j++) {
			if (drand48() < 0.05) MeshA[i][j] = true;
		}
	}
	// cpu time
	double startTime = omp_get_wtime();
	// time loop
	for (int n = 0; n < STEPS; n++) {
		// 3 variables : MeshA, MeshB, tableau de locks
		// Parallelization ()
#pragma omp parallel for shared(MeshA, MeshB, locks)
		for (int i = 1; i <= SIZE; i++) {
			// Lock the 3 rows
			lock(i, locks);
			for (int j = 1; j <= SIZE; j++) if (MeshA[i][j] == true) { 
				MeshA[i][j] = false;
				double move = drand48();
				if (move < 1.0*MOVE) {
					MeshB[i-1][j] = true;
				} else if (move < 2.0*MOVE) {
					MeshB[i+1][j] = true;
				} else if (move < 3.0*MOVE) {
					MeshB[i][j-1] = true;
				} else if (move < 4.0*MOVE) {
					MeshB[i][j+1] = true;
				} else {
					MeshB[i][j] = true;
				}
			} 
			// we have finished working on the row
			unlock(i, locks);
		}
		swap(MeshA, MeshB);
	}
	cout <<omp_get_max_threads() <<"\t" <<omp_get_wtime()-startTime <<endl;
}
