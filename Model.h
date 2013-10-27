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
        Statistic** moveAll(uint32_t iterations=1);
        Statistic** moveAll_mpi(uint32_t iterations=1);
        void moveAll_multiThreading(uint32_t iterations=1);
        void moveAll_multiThreading_2(uint32_t iterations=1);
        Statistic** moveAll_multiThreading_mpi(uint32_t iterations);
        void print();
        void printStats();
        int getCount(int kind);
        uint32_t getWidth();
        uint32_t getHeight();
        // Used for testing
        Cell* at(int x, int y);

    private:
        
        // The matrix representing the geographical area where the simulation takes place
        Matrix matrix;
        // the dimensions of the area
        uint32_t width, height;
        
        ////// the model parameters
        double naturalBirthProb, naturalDeathRisk, initialPopDensity, brainEatingProb, infectedToZombieProb;
        double zombieDecompositionRisk,humanMoveProb,zombieMoveProb;

        ////// MPI parameters
        // the neighbours of the current processor
        int * nbours;
        // the rank of the current processor
        int rank;

        // a random number generator per thread
        MersenneTwister** randomizer;
       
        void move(int x, int y, bool hasMoved, uint32_t* numThread);
        Coord moveHuman(int x, int y, uint32_t* numThread);
        Coord moveInfected(int x, int y, uint32_t* numThread);
        Coord moveZombie(int x, int y, uint32_t* numThread);
        static void* moveParallel(void* context);
        
        Coord getSquareToMoveTo(int fromX, int fromY, uint32_t* numThread);

        bool timeToDie(uint32_t* numThread);
        bool timeToDecompose(uint32_t* numThread);
        bool timeToBeBorn(uint32_t* numThread);
        bool timeToBecomeZombie(uint32_t* numThread);
        bool timeToMoveHuman(uint32_t* numThread);
        bool timeToMoveZombie(uint32_t* numThread);
        bool timeToEatBrain(uint32_t* numThread);
        void init();
        void init_mpi();
        void initMoveFlags();

        /**
         * @brief Initialise an array with the row numbers
         *        With out any randomization
         *        The order is simply the numbers from 0 to width-1
         *
         * @param   randomized (out parameter)
         */
        void initialiseRandomizedArray(uint32_t** randomized);
       
        /**
         *  @brief  Randomized the input array doing width/2 swaps
         *
         *  @param randomized (in/out parameter)
         */ 
        void randomized(uint32_t** randomized);
        
        /**
         * @brief Compute the numbers of the columns a thread has to deal with
         *        If all the threads cannot deal with the exact same number 
         *        of columns (i.e height % NUM_THREADS != 0), the first threads
         *        will deal with an additional column
         *
         * @param   height
         * @param   numThread
         * @param   firstColumn (out parameter)
         * @param   lastColumn (out parameter)
         */
        static void computeColumnNumbers(uint32_t height, uint32_t numThread,
                                  uint32_t* firstColumn, uint32_t* lastColumn);


            
};

#endif
