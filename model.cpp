#include "model.h"
#include <iostream>

using namespace std;

Model::Model(int width,int height,double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
        brainEatingProb,double infectedToZombieProb,double zombieDecompositionRisk, double humanMoveProb, double zombieMoveProb){
    this->width = width;
    this->height = height;
    this->naturalBirthProb = naturalBirthProb;
    this->naturalDeathRisk = naturalDeathRisk;
    this->initialPopDensity = initialPopDensity;
    this->brainEatingProb = brainEatingProb;
    this ->infectedToZombieProb = infectedToZombieProb;
    this->zombieDecompositionRisk = zombieDecompositionRisk;
    this->humanMoveProb = humanMoveProb;
    this->zombieMoveProb = zombieMoveProb;

    matrix = Dmatrix(height,width);
    randomizer = new MTRand(time(0));
    init();
}

Model::~Model(){

}

void Model::init(){
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++){
            //if (distr(twister) < initialPopDensity)
            if ((*randomizer)() < initialPopDensity)
            {
                matrix.set(x,y,new Cell(HUMAN));
            }
        }
    }
    int x = (int)((*randomizer)()*width);
    int y = (int)((*randomizer)()*height);
    matrix.set(x,y,new Cell(ZOMBIE));
    matrix.set((x+2)%width,y,new Cell(ZOMBIE));
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
}

Coord Model::moveZombie(int x,int y){
    Coord crd;
    crd.x = x;
    crd.y = y;
    if (timeToDecompose())
    {
        matrix.set(x,y,EMPTY);
    }else if(timeToMoveZombie()){
        Coord crd2 = getSquareToMoveTo(x,y);
        if (matrix(crd2.x,crd2.y)->kind() == HUMAN && timeToEatBrain())
        {// mmmm brains....
        	matrix.set(crd2.x,crd2.y,new Cell(INFECTED));
        }else if(matrix(crd2.x,crd2.y)->kind() == EMPTY){
        	Cell * zombie = matrix(x,y);
        	matrix.print();
            matrix.set(x,y,EMPTY);
            cout << "Moving zombie " << zombie <<" kind: "<< zombie->kind()<< "from" <<crd.x<<","<<crd.y
                <<" to " << crd2.x<<" , "<<crd2.y<<endl;
         	matrix.set(crd2.x,crd2.y,zombie);
            matrix.print();
        	crd = crd2;
        }
    }
    return crd;
}

Coord Model::moveInfected(int x,int y){
	Coord crd;
    // TODO update moveFlag
    if (this->timeToBecomeZombie())
    {
    	// TODO: Cell infected-zombie copy
        matrix.set(x,y,new Cell(ZOMBIE));
        moveZombie(x,y);
    }else{
        moveHuman(x,y);
    }
    return crd;
}

Coord Model::getSquareToMoveTo(int fromX,int fromY){
    double r = (*randomizer)();
    Coord crd;
    crd.x = fromX;
    crd.y = fromY;
    if (r < 0.25) // left
    {
        crd.x = (fromX-1+width)%width;
    }else if (r < 0.5) // right
    {
        crd.x = (fromX+1)%width;
    }else if (r < 0.75) // up
    {
        crd.y = ((fromY-1+height)%height);
    }else// down
    {
        crd.y = ((fromY+1)%height);
    }
    return crd;
}

Coord Model::moveHuman(int x,int y){
    Coord crd;
    crd.x = x;
    crd.y = y;
    if (this->timeToDie())
    {	
        matrix.set(x,y,EMPTY);
    }else if(timeToMoveHuman()){
        Coord crd2 = getSquareToMoveTo(x,y);
        if (matrix(crd2.x,crd2.y)->kind() == ZOMBIE && timeToEatBrain()) // zombie encounter!!
        {// brain eaten, infected, doesn't move;
            matrix.set(x,y,matrix(x,y)->Spawn(INFECTED));
        }else if(matrix(crd2.x,crd2.y)->kind() == EMPTY){
        	Cell * human = matrix(x,y);
        	matrix.set(x,y,EMPTY);
        	matrix.set(crd2.x,crd2.y,human);
        	crd = crd2;
        }
    }
    return crd;
}

void Model::move(int x,int y, bool hasMoved){
    // TODO check hasMoved
	int kind = matrix(x,y)->kind();
	if (kind == EMPTY)
	{
		if(timeToBeBorn()){
            Cell *newBorn = new Cell(HUMAN);
            newBorn->setMoveFlag(!hasMoved);
        }
	}else if(matrix(x,y)->moveFlag() == hasMoved)
	{
		Coord crd;
		switch(kind){
        case HUMAN:
            crd = moveHuman(x, y);
            break;
        case EMPTY:
            
            break;   
        case INFECTED:
            crd = moveInfected(x, y);
            break;
        case ZOMBIE:
            crd = moveZombie(x, y);
            break;
    	}

    	if (matrix(crd.x,crd.y)->kind() != EMPTY) // Might have died/decomposed
    	{
    		matrix(crd.x,crd.y)->setMoveFlag(!hasMoved);	
    	}
	}
}

void Model::moveAll(int iterations){
    for (int i = 0; i < iterations; i++)
    {
    	// TODO: really bad assuming that all Cells have moveFlag set to false in beginning
        bool hasMoved = (i % 2) == 1;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width;x++){
                move(x, y, hasMoved);
            }
        }
    }
}
