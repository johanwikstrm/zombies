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
int nZombies,nEmpty,nHumans,nInfected;
private:
};

void printStatsCsv(Statistic ** stats, int n);

#endif