#include "Model.h"
#include "Lock.h"
#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <cmath>
#include "mpiutils.h"

using namespace std;

// struct create for the argument of the function 
// executed by pthread_create
typedef struct inputMoveParallel {
    // the number of the thread (between 0 and NUM_THREAD-1)
    uint32_t numThread;
    // the model
    Model* model;
    // the locks (one for each row)
    Lock* locks;
    // the initial state of the moveFlags
    bool hasMoved;
    // the randomzied columns
    // in order to a bias, the columns are not always examined 
    // in the same order
    uint32_t** randomizedColumns;
    // true iff mpi is used
    bool mpi;
} inputMoveParallel;

Model::Model(int width, int height, int procRank, double naturalBirthProb, 
        double naturalDeathRisk, double initialPopDensity, 
        double brainEatingProb, double infectedToZombieProb,
        double zombieDecompositionRisk, double humanMoveProb, 
        double zombieMoveProb, bool mpiEnabled) {
    // The dimension of the mesh
    this->width = width;
    this->height = height;
    // finding my neighbours
    nbours = new int[4];
    rank = procRank;
    neighbours(toX(rank),toY(rank),PROC_WIDTH,PROC_HEIGHT,nbours);
    // the parameters of the model
    this->naturalBirthProb = naturalBirthProb;
    this->naturalDeathRisk = naturalDeathRisk;
    this->initialPopDensity = initialPopDensity;
    this->brainEatingProb = brainEatingProb;
    this->infectedToZombieProb = infectedToZombieProb;
    this->zombieDecompositionRisk = zombieDecompositionRisk;
    this->humanMoveProb = humanMoveProb;
    this->zombieMoveProb = zombieMoveProb;
    // Creation of the mesh
    matrix = Matrix(height, width,mpiEnabled);
    // Creation of the randomizer (one for each thread)
    randomizer = new MersenneTwister*[NUM_THREADS]; 
    // Seed each random number generator
    for (int i = 0; i < NUM_THREADS; i++) {
        randomizer[i] = new MersenneTwister(time(0) + i + procRank);
    }
    // Initialisation of the mesh (insert humans and zombies in the mesh)
    // With MPI we need to consider the ghost cells
    if (mpiEnabled) {
        init_mpi();
    } else {
        init();    
    }
}

Model::~Model(){
    // Free all the memory allocated
    delete[] nbours;
    for (int i = 0; i < NUM_THREADS; i++) {
        delete randomizer[i];
    }
    delete[] randomizer;
}

void Model::init_mpi(){
    // With MPI there are ghost cells
    // the columns 0 and width-1 are not considered
    // the rows 0 and height-1 are not considered
    for (uint32_t y = 1; y < height-1; y++) {
        for (uint32_t x = 1; x < width-1; x++) {
            if (randomizer[0]->rand() < initialPopDensity) {
                // Initially we can set directly the global statistics
                // there is only one thread
                matrix.set(x,y,HUMAN,NULL);
            }
        }
    }
    matrix.set(width/2, height/2, ZOMBIE, NULL);
    matrix.set(width/2+1,height/2+1, ZOMBIE, NULL);
}

void Model::init(){
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            if (randomizer[0]->rand() < initialPopDensity) {
                // Initially we can set directly the global statistics
                // there is only one thread
                matrix.set(x,y,HUMAN, NULL);
            }
        }
    }
    matrix.set(width/2, height/2, ZOMBIE, NULL);
    matrix.set(width/2+1,height/2+1, ZOMBIE, NULL);
}

bool Model::timeToDie(uint32_t* numThread){
    uint32_t n = 0;
    if (numThread != NULL) {
        n = *numThread; 
    }
    return randomizer[n]->rand() < naturalDeathRisk;
}

bool Model::timeToDecompose(uint32_t* numThread){
    uint32_t n = 0;
    if (numThread != NULL) {
        n = *numThread; 
    }
    return randomizer[n]->rand() < zombieDecompositionRisk;
}

bool Model::timeToBeBorn(uint32_t* numThread) {
    uint32_t n = 0;
    if (numThread != NULL) {
        n = *numThread; 
    }
    double empties = matrix.getCount(EMPTY);
    double humans = matrix.getCount(HUMAN);
    return randomizer[n]->rand() < (naturalBirthProb*humans/empties);
}

bool Model::timeToBecomeZombie(uint32_t* numThread){
    uint32_t n = 0;
    if (numThread != NULL) {
        n = *numThread; 
    }
    return randomizer[n]->rand() < infectedToZombieProb;
}

bool Model::timeToEatBrain(uint32_t* numThread){
    uint32_t n = 0;
    if (numThread != NULL) {
        n = *numThread; 
    }
    return randomizer[n]->rand() < brainEatingProb;
}

bool Model::timeToMoveHuman(uint32_t* numThread){
    uint32_t n = 0;
    if (numThread != NULL) {
        n = *numThread; 
    }
    return randomizer[n]->rand() < humanMoveProb;
}

bool Model::timeToMoveZombie(uint32_t* numThread){
    uint32_t n = 0;
    if (numThread != NULL) {
        n = *numThread; 
    }
    return randomizer[n]->rand() < zombieMoveProb;	
}

int Model::getCount(int kind){
    return matrix.getCount(kind);
}

void Model::printStats(){
    //cout << "Stats : "<<endl;
    //cout <<"Human\tInfctd\tZombie\tEmpty\tTotal\n";
    uint32_t humans = matrix.getCount(HUMAN);
    uint32_t infected = matrix.getCount(INFECTED);
    uint32_t zombies = matrix.getCount(ZOMBIE);
    uint32_t empty = matrix.getCount(EMPTY);
    uint32_t total= humans + infected + zombies + empty;
    cout <<humans <<"\t" <<infected <<"\t" <<zombies<<"\t" <<empty<<"\t" <<total<<endl;
}

void Model::print(){
    printStats();
    matrix.print();
}

Coord Model::moveZombie(int x,int y, uint32_t* numThread){
    Coord crd = Coord(x, y);
    // Decomposition of the zombie
    if (timeToDecompose(numThread)) {
        matrix.set(x, y, EMPTY, numThread);
        // The zombie moves 
    } else if(timeToMoveZombie(numThread)) {
        Coord crd2 = getSquareToMoveTo(x, y, numThread);
        int destKind = matrix(crd2)->getKind();
        switch(destKind){
            // If the square where the zombie wants to move
            // is inhabited by an infected
            // nothing happens
            case INFECTED :
                break;
                // If the square where the zombie wants to move
                // is inhabited by a human,
                // the human will get infected
            case HUMAN :
                if (timeToEatBrain(numThread)) {
                    // mmmm brains....
                    matrix.getInfected(crd2.getX(), crd2.getY(), numThread); 
                }
                break;
                // If the square where the zombie wants to move
                // is empty, the zombie moves
            case EMPTY :
                matrix.move(x, y, crd2.getX(), crd2.getY());
                crd = crd2;
                break;
        }
    }
    return crd;
}

Coord Model::moveInfected(int x,int y, uint32_t* numThread){
    Coord crd;
    // The incubation time is finished
    // The infected becomes a zombie
    // and move as a zombie
    if (timeToBecomeZombie(numThread)) {
        matrix.set(x, y, ZOMBIE, numThread);
        crd = moveZombie(x, y, numThread);
        // The infected behave as humans during the incubation period
        // The may die like humans, and move like them
    } else {
        crd = moveHuman(x, y, numThread);
    }
    return crd;
}

Coord Model::moveHuman(int x,int y, uint32_t* numThread){
    Coord crd = Coord(x, y);
    // Death of the human
    if (timeToDie(numThread)) {	
        matrix.set(x, y, EMPTY, numThread);
        // The human moves
    } else if(timeToMoveHuman(numThread)) {
        Coord crd2 = getSquareToMoveTo(x, y, numThread);
        if (matrix(crd2)->getKind() == ZOMBIE && timeToEatBrain(numThread)) { 
            // zombie encounter!!
            // brain eaten, infected, doesn't move;
            matrix.getInfected(x, y, numThread); 
        } else if(matrix(crd2)->getKind() == EMPTY){
            matrix.move(x, y, crd2.getX(), crd2.getY());
            crd = crd2;
        }
    }
    return crd;
}

Coord Model::getSquareToMoveTo(int fromX,int fromY, uint32_t* numThread){
    uint32_t n = 0;
    if (numThread != NULL) {
        n = *numThread; 
    }
    double r = randomizer[n]->rand();
    // all the directions have the same probability (0.25)
    Coord crd = Coord(fromX, fromY);
    if (r < 0.25) {
        // move left
        crd.setX((fromX-1+width)%width);
    } else if (r < 0.5) {
        // move right
        crd.setX((fromX+1)%width);
    } else if (r < 0.75) {
        // move up
        crd.setY((fromY-1+height)%height);
    } else {
        // move down
        crd.setY((fromY+1)%height);
    }
    return crd;
}

void Model::move(int x,int y, bool hasMoved, uint32_t* numThread) {
    int kind = matrix(x,y)->getKind();
    // If the cell is empty
    // a human may be born
    if (kind == EMPTY) {
        if (timeToBeBorn(numThread)) {
            matrix.set(x, y, HUMAN, numThread);
        }
        // Otherwise, the person in the cell moves
    } else if (matrix(x,y)->getMoveFlag() == hasMoved) {
        Coord crd = Coord(x, y);
        switch(kind){
            case HUMAN :
                crd = moveHuman(x, y, numThread);
                break;
            case INFECTED :
                crd = moveInfected(x, y, numThread);
                break;
            case ZOMBIE :
                crd = moveZombie(x, y, numThread);
                break;
        }
        // Update moveFlags for the non-empty cells only
        // Very important when multi-threading (because of the dummy)
        if (matrix(x,y)->getKind() != EMPTY) {
            // The square (x, y) has been considered 
            matrix(x,y)->setMoveFlag(!hasMoved);
        }
        if (matrix(crd.getX(), crd.getY())->getKind() != EMPTY) {
            // If the person in (x,y) has moved, updata the move Flag of the destination
            matrix(crd.getX(), crd.getY())->setMoveFlag(!hasMoved);
        }
    }
}

Statistic** Model::moveAll(uint32_t iterations){
    // Initialisation of the moveFlags
    initMoveFlags();
    Statistic **stats;
    stats = (Statistic**)calloc(iterations,sizeof(Statistic*));
    for (uint32_t i = 0; i < iterations; i++) {
        // For each iteration each person in the cell tries to move
        bool hasMoved = (i % 2) == 1;
        for (uint32_t y = 0; y < height; y++) {
            for (uint32_t x = 0; x < width; x++) {
                move(x, y, hasMoved, NULL);
            }
        }
        stats[i] = new Statistic(matrix);
    }
    return stats;
}

void* Model::moveParallel(void* context) {
    inputMoveParallel* input = (inputMoveParallel*) context;
    uint32_t numThread = input->numThread;
    Model* model = input->model;
    Lock* locks = input->locks;
    bool hasMoved = input->hasMoved;
    //uint32_t** randomizedColumnsNumbers = input->randomizedColumns;
    bool mpi = input->mpi;

    uint32_t height = model->getHeight();
    uint32_t width = model->getWidth();
    if (mpi) {
        height = height - 2; 
        width = width - 2; 
    }
    // compute the number of the columns the thread
    // has to deal with (from firstColumn to lastColumn)
    uint32_t numColumns = height / NUM_THREADS;
    uint32_t firstColumn = numThread*numColumns;
    uint32_t lastColumn = (numThread+1)*numColumns;
    uint32_t firstRow = 0;
    uint32_t lastRow = width;
    if (mpi) {
        firstColumn ++;
        lastColumn ++;
        firstRow = 1;
        lastRow = width+1;
    }
    // The thread executes the movements for its columns
    for (uint32_t y = firstColumn; y < lastColumn; y++){
        // locking : the considered column (y) and its two
        // nearest neighbours
        locks->lock(y);
        for (uint32_t x = firstRow; x < lastRow; x++){
            //model->move((*randomizedRowsNumbers)[x][numThread], 
            //            y, hasMoved, numThread);
            model->move(x, y, hasMoved, &numThread);
        }
        // unlocking the three columns
        locks->unlock(y);
    }
    return (void*)0L;
}

void Model::moveAll_multiThreading(uint32_t iterations) {
    initMoveFlags();
    // Create an object Lock in order to have a locker for each column
    // of the matrix
    Lock locks = Lock(height);
    // Create an array for the randmized column numbers
    uint32_t* randomizedColumnNumbers = (uint32_t*)calloc(width, sizeof(uint32_t));
    for (uint32_t i = 0; i < width; i++) {
        randomizedColumnNumbers[i] = i;
    }
    for (uint32_t i = 0; i < iterations; i++) {
        // For each iteration we change the order the columns are examined
        // TODO
        /**
        uint32_t row1;
        uint32_t row2;
        for (uint32_t n = 0; n < width/2; n++) {
            row1 = randomizer[0]->randInt(width-1);
            row2 = randomizer[0]->randInt(width-1);
            uint32_t tmp = randomizedColumnNumbers[row1];
            randomizedColumnNumbers[row1] = randomizedColumnNumbers[row2]; 
            randomizedColumnNumbers[row2] = tmp;
        } 
    */
        bool hasMoved = (i % 2) == 1;
        void *status;
        inputMoveParallel* inputs[NUM_THREADS];
        pthread_t threads[NUM_THREADS];
        for (uint32_t n = 0; n < NUM_THREADS; n++) {
            inputs[n] = (inputMoveParallel*)malloc(sizeof(inputMoveParallel));
            inputs[n]->numThread = n;
            inputs[n]->model = this;
            inputs[n]->locks = &locks;
            inputs[n]->hasMoved = hasMoved;
            //inputs[n]->randomizedColumns = &randomizedColumnNumbers;
            inputs[n]->mpi = false;
            pthread_create(&(threads[n]), NULL, Model::moveParallel, (void*)inputs[n]);
        } 
        // Wait for the end of every thread
        for (uint32_t n = 0; n < NUM_THREADS; n++) {
            pthread_join(threads[n], &status);
            free(inputs[n]);
            if (status != (void*)0) {
                printf("Error in the execution");
            }
        }
        // The statistics are computed locally
        // Need to reduce the results of each thread
        matrix.computeGlobalStatistics();
        printStats();
    }
}

Statistic** Model::moveAll_mpi(uint32_t iterations){
    initMoveFlags();
    Statistic **stats;
    stats = (Statistic**)calloc(iterations,sizeof(Statistic*));
    for (uint32_t i = 0; i < iterations; i++){
        // Not cool
        neighbours(toX(rank),toY(rank),PROC_WIDTH,PROC_HEIGHT,nbours);
        bool hasMoved = (i % 2) == 1;
        // All valid cells should have moveflag hasMoved by now
        // some ghost cells may have the wrong moveflag, so they should not be exchanged 
        // with the other processes, this is to prevent duplication
        if (nbours[RIGHT] == 1){
            cout <<" Iteration: "<<i<<endl<<flush;
        }
        swapAll(nbours, matrix,hasMoved, i); // must be done before the first iteration
        for (uint32_t y = 1; y < height-1; y++){
            for (uint32_t x = 1; x < width-1; x++){
                move(x, y, hasMoved, NULL);
            }
        }
        stats[i] = new Statistic(matrix);
        stats[i]->mpi_reduce();
    }
    return stats;
}

Statistic** Model::moveAll_multiThreading_mpi(uint32_t iterations){
    Statistic **stats;
    stats = (Statistic**)calloc(iterations,sizeof(Statistic*));
    initMoveFlags();
    Lock locks = Lock(height);
    for (uint32_t i = 0; i < iterations; i++) {
        bool hasMoved = (i % 2) == 1;
        void *status;
        // All valid cells should have moveflag==hasMoved by now
        // some ghost cells may have moveflag!=hasMoved
        // in that case they should not be sent to the other processes 
        // to avoid duplication of humans, zombies and infected
        int collisions = swapAll(nbours,matrix,hasMoved,i);

        cout<<"collisions: "<<collisions<<endl<<flush;   

        //cout << "Iteration " << i <<endl << flush;
        inputMoveParallel* inputs[NUM_THREADS];
        pthread_t threads[NUM_THREADS];
        for (uint32_t n = 0; n < NUM_THREADS; n++) {
            inputs[n] = (inputMoveParallel*)malloc(sizeof(inputMoveParallel));
            inputs[n]->numThread = n;
            inputs[n]->model = this;
            inputs[n]->locks = &locks;
            inputs[n]->hasMoved = hasMoved;
            inputs[n]->mpi = true;
            pthread_create(&(threads[n]), NULL, Model::moveParallel, (void*)inputs[n]);
        } 
        // Wait for the end of every thread
        for (uint32_t n = 0; n < NUM_THREADS; n++) {
            pthread_join(threads[n], &status);
            free(inputs[n]);
            if (status != (void*)0) {
                printf("Error in the execution");
            } 
        }
        stats[i] = new Statistic(matrix);
        stats[i]->mpi_reduce();
    }
    return stats ;  
}

uint32_t Model::getWidth() {
    return width;
}

uint32_t Model::getHeight() {
    return height;
}

void Model::initMoveFlags() {
    for (uint32_t x = 0; x < width; x++) {
        for(uint32_t y = 0; y < height; y++) {
            matrix(x,y)->setMoveFlag(false);
        }
    }    
}

Cell * Model::at(int x, int y){
    return matrix(x,y);
}
