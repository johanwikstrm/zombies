// Scaling run / scaling graph (speed-up graph)
// Graph : x -> number of threads (log 2)
// 	   y -> time (log 2)
// stabilisqtion : pas assez de ressources (pas assez de cores) run out of cores
// parfois, pas assez de travail (pour une faible taille)       run out of works
// limit of strong scaling (12 here)

#include <iostream>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif
using namespace std;
#define SIZE	1000

double **CreateMatrix(int I, int J) {
	double **Matrix = new double *[I];
	for (int i = 0; i < I; i++) {
		Matrix[i] = new double[J];
		for (int j = 0; j < J; j++) Matrix[i][j] = 0.0;
	}
	return(Matrix);
}

int main(int argc, char **argv) {
	double **A = CreateMatrix(SIZE, SIZE);
	double **B = CreateMatrix(SIZE, SIZE);
	double **C = CreateMatrix(SIZE, SIZE);
	for (int t = 1; t <= 64; t*=2) {
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) { 
				A[i][j] = drand48();
				B[i][j] = drand48();
			}
		}
		#ifdef _OPENMP
		double startTime = omp_get_wtime();
		// no copies of A, B, and C
		#pragma omp parallel for default(none) shared(A,B,C) num_threads(t)
		#endif
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) { 
				C[i][j] = 0.0;
				for (int k = 0; k < SIZE; k++) { 
					C[i][j] += A[i][k]*B[k][j];
				}
			} 
		}

		/*
		   for(int i=0; i<SIZE; i++)
		   cout << C[i][i] <<endl;
		   */

		// Maximum nomber of threads (default value) and time
		#ifdef _OPENMP
		cout <<t <<"\t" <<omp_get_wtime() - startTime << endl;
		#endif
	}

}
