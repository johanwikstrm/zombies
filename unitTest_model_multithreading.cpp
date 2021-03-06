#include <cassert>
#include <mpi.h>
#include "Model.h"
#include <iostream>
#include "MersenneTwister.h"
#include <pthread.h>

using namespace std;


int main(int argc, char *argv[]){
    //Model(int width,int height, int rank, double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
    //brainEatingProb,double infectedToZombieProb,double zombieDecompositionRisk, double humanMoveProb, double zombieMoveProb);
     
    int rank = 1;
   
    uint32_t width = 1000;
    uint32_t height = 1500;
   
    double naturalBirthProb = 0.2;
    double naturalDeathRisk = 6/(1000*366);
    double initialPopDensity = 0.017;
    double brainEatingProb = 0;
    double infectedToZombieProb = 0.8;
    double zombieDecompositionRisk = 0.2;
    double humanMoveProb = 0.8;
    double zombieMoveProb = 0.5; 
    
    Model m = Model(width, height, rank, naturalBirthProb, naturalDeathRisk, 
                    initialPopDensity, brainEatingProb, infectedToZombieProb, 
                    zombieDecompositionRisk, humanMoveProb, zombieMoveProb);
    m.print();
    m.moveAll_multiThreading(100);
    m.print();
}
