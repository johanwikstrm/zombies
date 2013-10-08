#include <iostream>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
using namespace std;
//
#define IM		0
#define IP		1
#define JM		2
#define JP		3
//
#define TAG_XP	1
#define TAG_XM	2
//
#define SIZEI	5
#define SIZEJ	4
// #define DBG(S) if (getRank() == 0) std::cerr << S << std::endl;
#define DBG(S) std::cerr << S << std::endl;
#define ASSERT(x) assert(x)
#define ASSERTDBG(x,S) if (!(x)) { DBG(S); ASSERT(x); }

int getSize() {
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	return(size);
}

int getRank() {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	return(rank);
}

int getRank(int i, int j, int I, int J) {
	return(i*J + j);
}

int getRankI(int rank, int I, int J) {
	return(rank/J);
}

int getRankJ(int rank, int I, int J) {
	return(rank % J);
}

int getIntArg(int argc, char *argv[], const char *arg, int defaultValue) {
	for (int i = 1; i < argc - 1; i ++) {
		if (strcmp(argv[i], arg) == 0) {
			DBG("Got argument pair " << argv[i] << " " << argv[i+1] << " returning " << atoi(argv[i+1]))
			return(atoi(argv[i+1]));
		}
	}
	return(defaultValue);
}
int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
	//
	int Pi = getIntArg(argc, argv, "-Pi", 1);
	int Pj = getIntArg(argc, argv, "-Pj", getSize());
	if (Pi*Pj != getSize()) {
		Pi = 1; Pj = getSize();
	}
	int I = getRankI(getRank(), Pi, Pj);
	int J = getRankJ(getRank(), Pi, Pj);
	int Neighbours[4] = { getRank(), getRank(), getRank(), getRank() };
	ASSERTDBG(getRank() == getRank(I, J, Pi, Pj), "getRank(), getRankI(), getRankJ() not working!")
	if (0 < I) Neighbours[IM] = getRank(I - 1, J, Pi, Pj);
	if (I < Pi - 1) Neighbours[IP] = getRank(I + 1, J, Pi, Pj);
	if (0 < J) Neighbours[JM] = getRank(I, J - 1, Pi, Pj);
	if (J < Pj - 1) Neighbours[JP] = getRank(I, J + 1, Pi, Pj);
	MPI_Barrier(MPI_COMM_WORLD);
	for (int r = 0; r < getSize(); r++) {
		if (r == getRank()) {
			cout << getRank() << "["<< I << "," << J << "]:";
			for (int i = 0; i < 4; i++) cout << " " << Neighbours[i];
			cout << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	//
	int *A = new int[SIZEI*SIZEJ];
	for (int i = 0; i < SIZEI; i++) {
		for (int j = 0; j < SIZEJ; j++) { 
			A[i*SIZEJ + j] = 100*(getRank() + 1) + i*SIZEJ + j;
		}
	}
	MPI_Datatype SendRecvI,SendRecvJ;
	MPI_Type_vector(SIZEJ, 1, 1, MPI_INT, &SendRecvI);
	MPI_Type_commit(&SendRecvI);
	MPI_Type_vector(SIZEI,1,SIZEJ,MPI_INT,&SendRecvJ);
	MPI_Type_commit(&SendRecvJ);
	//
	MPI_Status status;
	if (Neighbours[IP] != getRank()) {
		MPI_Send(&A[SIZEJ*(SIZEI-2)], 1, SendRecvI, Neighbours[IP], TAG_XP, MPI_COMM_WORLD);
	}
	if (Neighbours[IM] != getRank()) {
		MPI_Recv(&A[0], 1, SendRecvI, Neighbours[IM], TAG_XP, MPI_COMM_WORLD, &status);
	}
	//
	if (Neighbours[IM] != getRank()) {
		MPI_Send(&A[SIZEJ], 1, SendRecvI, Neighbours[IM], TAG_XP, MPI_COMM_WORLD);
	}
	if (Neighbours[IP] != getRank()) {
		MPI_Recv(&A[SIZEJ*(SIZEI - 1)], 1, SendRecvI, Neighbours[IP], TAG_XP, MPI_COMM_WORLD, &status);
	}
	//
	for (int r = 0; r < getSize(); r++) {
		if (r == getRank()) {
			cout << "Rank: " << getRank() << endl;
			for (int j = SIZEJ - 1; j >= 0; j--) { 
				for (int i = 0; i < SIZEI; i++) {
					cout << A[i*SIZEJ + j] << " ";
				} 
				cout << endl;
			}
			cout << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	//
	MPI_Finalize();
}
