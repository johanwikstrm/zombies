#include "Model.h"
#include "Lock.h"
#include <iostream>
#include <omp.h>
#include <assert.h>
#include "mpiutils.h"

using namespace std;

Model::Model(int width,int height,int procRank,double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
        brainEatingProb,double infectedToZombieProb,double zombieDecompositionRisk, double humanMoveProb, double zombieMoveProb){
    this->width = width;
    this->height = height;
    // finding my neighbours
    nbours = new int[4];
    neighbours(toX(procRank),toY(procRank),PROC_WIDTH,PROC_HEIGHT,nbours);
    this->naturalBirthProb = naturalBirthProb;
    this->naturalDeathRisk = naturalDeathRisk;
    this->initialPopDensity = initialPopDensity;
    this->brainEatingProb = brainEatingProb;
    this ->infectedToZombieProb = infectedToZombieProb;
    this->zombieDecompositionRisk = zombieDecompositionRisk;
    this->humanMoveProb = humanMoveProb;
    this->zombieMoveProb = zombieMoveProb;
    
    matrix = Matrix(height, width);
    randomizer = new MTRand(time(0));
    init();
}

Model::~Model(){
    delete[] nbours;
    delete randomizer;
}

void Model::init(){
    for (int y = 1; y < height-1; y++)
    {
        for (int x = 1; x < width-1; x++){
            if ((*randomizer)() < initialPopDensity)
            {
                matrix.set(x,y,HUMAN);
            }
        }
    }
    matrix.set(width/2, height/2, ZOMBIE);
    matrix.set(width/2+1,height/2+1, ZOMBIE);
}

bool Model::timeToDie(){
    return (*randomizer)() < naturalDeathRisk;
}

bool Model::timeToDecompose(){
    return (*randomizer)() < zombieDecompositionRisk;
}

// weighs the result on the number of people in the matrix and 
// the number of empty spaces
bool Model::timeToBeBorn(){
    return (*randomizer)() < naturalBirthProb*matrix.getCount(HUMAN)/matrix.getCount(EMPTY);
}

bool Model::timeToBecomeZombie(){
    return (*randomizer)() < infectedToZombieProb;
}

bool Model::timeToEatBrain(){
    return (*randomizer)() < brainEatingProb;
}

bool Model::timeToMoveHuman(){
    return (*randomizer)() < humanMoveProb;
}
bool Model::timeToMoveZombie(){
    return (*randomizer)() < zombieMoveProb;	
}

int Model::getCount(int kind){
    return matrix.getCount(kind);
}

void Model::printStats(){
    cout << Model::matrix.getCount(HUMAN)<< "\t"<<Model::matrix.getCount(INFECTED) << "\t"<<Model::matrix.getCount(ZOMBIE)<< "\t"<<Model::matrix.getCount(EMPTY)<<"\n";
}

void Model::print(){
    cout << "Stats:\nHuman   Infctd  Zombie  Empty\n";
    printStats();
    matrix.print();
    matrix.printMoveFlags();
}

Coord Model::moveZombie(int x,int y){
    Coord crd = Coord(x, y);

    if (timeToDecompose()) {
        matrix.set(x, y, EMPTY);

    }else if(timeToMoveZombie()) {
        Coord crd2 = getSquareToMoveTo(x,y);
        int destKind = matrix(crd2)->getKind();
        switch(destKind){
            case HUMAN :
            case INFECTED :
                if (timeToEatBrain()) {
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

Coord Model::moveInfected(int x,int y){
    Coord crd;
    if (timeToBecomeZombie()) {
        matrix.set(x, y, ZOMBIE);
        crd = moveZombie(x, y);
    } else {
        crd = moveHuman(x, y);
    }
    return crd;
}

Coord Model::getSquareToMoveTo(int fromX,int fromY){
    double r = (*randomizer)();
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

Coord Model::moveHuman(int x,int y){
    Coord crd = Coord(x, y);
    if (timeToDie()) {	
        matrix.set(x, y, EMPTY);
    } else if(timeToMoveHuman()) {
        Coord crd2 = getSquareToMoveTo(x,y);
        if (matrix(crd2)->getKind() == ZOMBIE && timeToEatBrain()){ // zombie encounter!!
            // brain eaten, infected, doesn't move;
            matrix.getInfected(x, y); 
        }else if(matrix(crd2)->getKind() == EMPTY){
            matrix.move(x, y, crd2.getX(), crd2.getY());
            crd = crd2;
        }
    }
    return crd;
}

void Model::move(int x,int y, bool hasMoved){

    int kind = matrix(x,y)->getKind();
    if (kind ==EMPTY) {
        if(timeToBeBorn()){
            matrix.set(x, y, HUMAN);
        }
    } else if (matrix(x,y)->getMoveFlag() == hasMoved) {
        Coord crd = Coord(x, y);
        switch(kind){
            case HUMAN :
                crd = moveHuman(x, y);
                break;
            case INFECTED :
                crd = moveInfected(x, y);
                break;
            case ZOMBIE :
                crd = moveZombie(x, y);
                break;
        }
        // The square (x, y) has been considered 
        matrix(x,y)->setMoveFlag(!hasMoved);
        // If the person in (x,y) has moved, updata the move Flag of the destination
        matrix(crd.getX(), crd.getY())->setMoveFlag(!hasMoved);
    }
}

void Model::moveAll(int iterations){
    for (int i = 0; i < iterations; i++){
        // TODO : init flags
        // really bad assuming that all Cells have moveFlag set to false in beginning
        bool hasMoved = (i % 2) == 1;
        for (int y = 1; y < height-1; y++){
            for (int x = 1; x < width-1; x++){
                move(x, y, hasMoved);
            }
        }
        //swapAll(nbours,matrix);
    }
}

void Model::moveAll_omp(int iterations){
    Lock locks = Lock(height);
    double startTime = omp_get_wtime();
    for (int i = 0; i < iterations; i++){
        // TODO : init flags
        // really bad assuming that all Cells have moveFlag set to false in beginning
        bool hasMoved = (i % 2) == 1;
#pragma omp parallel for shared(locks)
        for (int y = 0; y < height; y++){
            // Lock the 3 columns
            locks.lock(y);
            for (int x = 0; x < width; x++){
                move(x, y, hasMoved);
            }
            locks.unlock(y);
        }
        //print(); 
    }
    cout <<omp_get_max_threads() <<"\t" <<omp_get_wtime()-startTime <<endl;
}



Cell * Model::at(int x, int y){
    return matrix(x,y);
}
