#ifndef MODEL_H
#define MODEL_H
#include "mtrand.h"
#include "constants.h"
#include "Matrix.h"
#include "Coord.h"



class Model
{
    public:
        Model(int width,int height,int procRank,double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
                brainEatingProb,double infectedToZombieProb,double zombieDecompositionRisk, double humanMoveProb, double zombieMoveProb);
        ~Model();
        void moveAll(uint32_t iterations=1);
        void moveAll_mpi(uint32_t iterations=1);
        void moveAll_omp(uint32_t iterations=1);
        void print();
        void printStats();
        int getCount(int kind);
        // Used for testing
        Cell * at(int x, int y);

    private:
        Matrix matrix;
        uint32_t width, height;
        int *nbours;
        MTRand *randomizer;
        double naturalBirthProb, naturalDeathRisk, initialPopDensity, brainEatingProb, infectedToZombieProb;
        double zombieDecompositionRisk,humanMoveProb,zombieMoveProb;
        void move(int x,int y, bool hasMoved);
        Coord moveHuman(int x,int y);
        Coord moveInfected(int x,int y);
        Coord moveZombie(int x,int y);
        Coord getSquareToMoveTo(int fromX,int fromY);

        bool timeToDie();
        bool timeToDecompose();
        bool timeToBeBorn();
        bool timeToBecomeZombie();
        bool timeToMoveHuman();
        bool timeToMoveZombie();
        bool timeToEatBrain();
        void init();
        void init_mpi();
        void initMoveFlags();
};

#endif
