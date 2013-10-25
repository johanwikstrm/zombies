#ifndef STATISTIC_H
#define STATISTIC_H
#include "Matrix.h"
#include <string>

using namespace std;

class Statistic
{
    public:
        Statistic();
        Statistic(Matrix& m);
        void mpi_reduce();
        string toCsv();
        int sum();
        bool allAboveZero();
        int nZombies,nEmpty,nHumans,nInfected;
};

void printStatsCsv(Statistic ** stats, int n);

#endif
