#include <cassert>
#include <mpi.h>
#include "Model.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    //Model(int width,int height, int rank, double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
    //brainEatingProb,double infectedToZombieProb,double zombieDecompositionRisk, double humanMoveProb, double zombieMoveProb);
     
    int rank = 1;
   
    uint32_t width = 100;
    uint32_t height = 50;
   
    // In Australia : 13.3 births per year per 1000 persons 
    double naturalBirthProb = 0.5;
    //double naturalBirthProb = 13.3*1700/(1000*366*height*width);
    // In Australia : 6.5 deaths per year per 1000 persons
    double naturalDeathRisk = 6/(1000*366);
    // Population density in Northern Territory
    //double initialPopDensity = 0.17;
    double initialPopDensity = 0.9;
    double brainEatingProb = 0;
    double infectedToZombieProb = 0;
    double zombieDecompositionRisk = 0;
    double humanMoveProb = 1;
    double zombieMoveProb = 0.5; 
    
    Model m = Model(width, height, rank, naturalBirthProb, naturalDeathRisk, 
                    initialPopDensity, brainEatingProb, infectedToZombieProb, 
                    zombieDecompositionRisk, humanMoveProb, zombieMoveProb);
    //m.print();
    m.moveAll_omp(10);
    //m.print();
}
