#ifndef MODEL_H
#define MODEL_H
#include "MersenneTwister.h"
#include "constants.h"
#include "Matrix.h"
#include "Coord.h"
#include "Statistic.h"

class Model
{
    public:
        Model(int width,int height,int procRank,double naturalBirthProb, double naturalDeathRisk, double initialPopDensity
                , double brainEatingProb,double infectedToZombieProb,double zombieDecompositionRisk, double humanMoveProb
                , double zombieMoveProb, bool mpiEnabled = false);
        ~Model();
        Statistic ** moveAll(uint32_t iterations=1);
        Statistic ** moveAll_mpi(uint32_t iterations=1);
        void moveAll_multiThreading(uint32_t iterations=1);
        Statistic ** moveAll_multiThreading_mpi(uint32_t iterations);
        void print();
        void printStats();
        int getCount(int kind);
        uint32_t getWidth();
        uint32_t getHeight();
        // Used for testing
        Cell * at(int x, int y);

    private:
        Matrix matrix;
        uint32_t width, height;
        int * nbours;
        int rank;

        // a generator for each thread
        MersenneTwister** randomizer;
        
        static void* moveParallel(void* context);
        double naturalBirthProb, naturalDeathRisk, initialPopDensity, brainEatingProb, infectedToZombieProb;
        double zombieDecompositionRisk,humanMoveProb,zombieMoveProb;
        void move(int x, int y, bool hasMoved, uint32_t numThread);
        Coord moveHuman(int x, int y, uint32_t numThread);
        Coord moveInfected(int x, int y, uint32_t numThread);
        Coord moveZombie(int x, int y, uint32_t numThread);
        Coord getSquareToMoveTo(int fromX, int fromY, uint32_t numThread);

        bool timeToDie(uint32_t numThread);
        bool timeToDecompose(uint32_t numThread);
        bool timeToBeBorn(uint32_t numThread);
        bool timeToBecomeZombie(uint32_t numThread);
        bool timeToMoveHuman(uint32_t numThread);
        bool timeToMoveZombie(uint32_t numThread);
        bool timeToEatBrain(uint32_t numThread);
        void init();
        void init_mpi();
        void initMoveFlags();
};

#endif
