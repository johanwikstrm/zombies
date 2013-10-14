#include "Model.h"
#include "Lock.h"
#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <cmath>
#include "mpiutils.h"

using namespace std;

typedef struct inputMoveParallel {
    uint32_t numThread;
    Model* model;
    Lock* locks;
    bool hasMoved;
} inputMoveParallel;

Model::Model(int width,int height,int procRank,double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
        brainEatingProb,double infectedToZombieProb,double zombieDecompositionRisk, double humanMoveProb
        , double zombieMoveProb,bool mpiEnabled){
    this->width = width;
    this->height = height;
    // finding my neighbours
    nbours = new int[4];
    neighbours(toX(procRank),toY(procRank),PROC_WIDTH,PROC_HEIGHT,nbours);
    this->naturalBirthProb = naturalBirthProb;
    this->naturalDeathRisk = naturalDeathRisk;
    this->initialPopDensity = initialPopDensity;
    this->brainEatingProb = brainEatingProb;
    this->infectedToZombieProb = infectedToZombieProb;
    this->zombieDecompositionRisk = zombieDecompositionRisk;
    this->humanMoveProb = humanMoveProb;
    this->zombieMoveProb = zombieMoveProb;

    matrix = Matrix(height, width);
    randomizer = new MTRand*[NUM_THREADS]; 
    for (int i = 0; i < NUM_THREADS; i++) {
        randomizer[i] = new MTRand(time(0) + i + procRank);
    }

    if (mpiEnabled){
        init_mpi();
    } else {
        init();    
    }
}

Model::~Model(){
    delete[] nbours;
    for (int i = 0; i < NUM_THREADS; i++) {
        delete randomizer[i];
    }
    delete[] randomizer;
}

void Model::init_mpi(){
    for (uint32_t y = 1; y < height-1; y++) {
        for (uint32_t x = 1; x < width-1; x++) {
            if ((*randomizer[0])() < initialPopDensity) {
                matrix.set(x,y,HUMAN);
            }
        }
    }
    matrix.set(width/2, height/2, ZOMBIE);
    matrix.set(width/2+1,height/2+1, ZOMBIE);
}

void Model::init(){
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            if ((*randomizer[0])() < initialPopDensity) {
                matrix.set(x,y,HUMAN);
            }
        }
    }
    matrix.set(width/2, height/2, ZOMBIE);
    matrix.set(width/2+1,height/2+1, ZOMBIE);
}

bool Model::timeToDie(uint32_t numThread){
    return (*randomizer[numThread])() < naturalDeathRisk;
}

bool Model::timeToDecompose(uint32_t numThread){
    return (*randomizer[numThread])() < zombieDecompositionRisk;
}

bool Model::timeToBeBorn(uint32_t numThread) {
    return (*randomizer[numThread])() < naturalBirthProb;
}

bool Model::timeToBecomeZombie(uint32_t numThread){
    return (*randomizer[numThread])() < infectedToZombieProb;
}

bool Model::timeToEatBrain(uint32_t numThread){
    return (*randomizer[numThread])() < brainEatingProb;
}

bool Model::timeToMoveHuman(uint32_t numThread){
    return (*randomizer[numThread])() < humanMoveProb;
}

bool Model::timeToMoveZombie(uint32_t numThread){
    return (*randomizer[numThread])() < zombieMoveProb;	
}

int Model::getCount(int kind){
    return matrix.getCount(kind);
}

void Model::printStats(){
    cout << "Stats : "<<endl;
    cout <<"Human\tInfctd\tZombie\tEmpty\tTotal\n";
    uint32_t humans = matrix.getCount(HUMAN);
    uint32_t infected = matrix.getCount(INFECTED);
    uint32_t zombies = matrix.getCount(ZOMBIE);
    uint32_t empty = matrix.getCount(EMPTY);
    uint32_t total= humans + infected + zombies + empty;
    cout <<humans <<"\t" <<infected <<"\t" <<zombies<<"\t" <<empty<<"\t" <<total<<endl;
}

void Model::print(){
    printStats();
    //matrix.print();
}

Coord Model::moveZombie(int x,int y, uint32_t numThread){
    Coord crd = Coord(x, y);

    if (timeToDecompose(numThread)) {
        matrix.set(x, y, EMPTY);

    } else if(timeToMoveZombie(numThread)) {
        Coord crd2 = getSquareToMoveTo(x, y, numThread);
        int destKind = matrix(crd2)->getKind();
        switch(destKind){
            case HUMAN :
            case INFECTED :
                if (timeToEatBrain(numThread)) {
                    // mmmm brains....
                    matrix.getInfected(crd2.getX(), crd2.getY()); 
                }
                break;
            case EMPTY :
                matrix.move(x, y, crd2.getX(), crd2.getY());
                crd = crd2;
                break;
        }
    }
    return crd;
}

Coord Model::moveInfected(int x,int y, uint32_t numThread){
    Coord crd;
    if (timeToBecomeZombie(numThread)) {
        matrix.set(x, y, ZOMBIE);
        crd = moveZombie(x, y, numThread);
    } else {
        crd = moveHuman(x, y, numThread);
    }
    return crd;
}

Coord Model::getSquareToMoveTo(int fromX,int fromY, uint32_t numThread){
    
    double r = (*(randomizer[numThread]))();

    Coord crd = Coord(fromX, fromY);
    if (r < 0.25) // left
    {
        crd.setX((fromX-1+width)%width);
    }else if (r < 0.5) // right
    {
        crd.setX((fromX+1)%width);
    }else if (r < 0.75) // up
    {
        crd.setY((fromY-1+height)%height);
    }else// down
    {
        crd.setY((fromY+1)%height);
    }
    return crd;
}

Coord Model::moveHuman(int x,int y, uint32_t numThread){
    Coord crd = Coord(x, y);
    if (timeToDie(numThread)) {	
        matrix.set(x, y, EMPTY);
    } else if(timeToMoveHuman(numThread)) {
        Coord crd2 = getSquareToMoveTo(x, y, numThread);
        if (matrix(crd2)->getKind() == ZOMBIE && timeToEatBrain(numThread)){ // zombie encounter!!
            // brain eaten, infected, doesn't move;
            matrix.getInfected(x, y); 
        }else if(matrix(crd2)->getKind() == EMPTY){
            matrix.move(x, y, crd2.getX(), crd2.getY());
            crd = crd2;
        }
    }
    return crd;
}

void Model::move(int x,int y, bool hasMoved, uint32_t numThread){

    int kind = matrix(x,y)->getKind();
    if (kind == EMPTY) {
        if (timeToBeBorn(numThread)) {
            matrix.set(x, y, HUMAN);
        }
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

        if (x != 0 && x != (width-1)) {
            assert(abs((int)crd.getX() - (int)x) <=1);
        }
        if (y != 0 && y != (height-1)) {
            assert(abs((int)crd.getY() - (int)y) <= 1);
        }

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
    initMoveFlags();
    Statistic **stats;
    stats = (Statistic**)calloc(iterations,sizeof(Statistic*));
    for (uint32_t i = 0; i < iterations; i++){
        bool hasMoved = (i % 2) == 1;
        for (uint32_t y = 0; y < height; y++){
            for (uint32_t x = 0; x < width; x++){
                move(x, y, hasMoved, 0);
            }
        }
        stats[i] = new Statistic(matrix);
    }
    return stats;
}

Statistic** Model::moveAll_mpi(uint32_t iterations){
    initMoveFlags();
    Statistic **stats;
    stats = (Statistic**)calloc(iterations,sizeof(Statistic*));
    for (uint32_t i = 0; i < iterations; i++){
        bool hasMoved = (i % 2) == 1;
        for (uint32_t y = 1; y < height-1; y++){
            for (uint32_t x = 1; x < width-1; x++){
                move(x, y, hasMoved, 0);
            }
        }
        stats[i] = new Statistic(matrix);
        stats[i]->mpi_reduce();
        swapAll(nbours,matrix);

    }
    return stats;
}

uint32_t Model::getWidth() {
    return width;
}

uint32_t Model::getHeight() {
    return height;
}

void* Model::moveParallel(void* context) {
    inputMoveParallel* input = (inputMoveParallel*) context;
    uint32_t numThread = input->numThread;
    Model* model = input->model;
    Lock* locks = input->locks;
    bool hasMoved = input->hasMoved;
    uint32_t numColumns = model->getHeight() / NUM_THREADS;
    uint32_t firstColumn = numThread*numColumns;
    uint32_t lastColumn = (numThread+1)*numColumns;
    for (uint32_t y = firstColumn; y < lastColumn; y++){
        locks->lock(y);
        assert(locks->getValue(y));
        for (uint32_t x = 0; x < model->getWidth(); x++){
            model->move(x, y, hasMoved, numThread);
        }
        locks->unlock(y);
    }
    return (void*)0L;
}


void Model::moveAll_omp(uint32_t iterations) {
    initMoveFlags();
    Lock locks = Lock(height);
    for (uint32_t i = 0; i < iterations; i++) {
        bool hasMoved = (i % 2) == 1;
        void *status;
        pthread_t threads[NUM_THREADS];
        for (uint32_t n = 0; n < NUM_THREADS; n++) {
            inputMoveParallel input = {n, this, &locks, hasMoved};
            pthread_create(&(threads[n]), NULL, Model::moveParallel, (void*) (&input));
        } 
        // Wait for the end of every thread
        for (uint32_t n = 0; n < NUM_THREADS; n++) {
            pthread_join(threads[n], &status);
            if (status != (void*)0) {
                printf("Error in the execution");
            } 
        }
    }
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
