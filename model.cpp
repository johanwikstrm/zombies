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
  	tempMatrix = Dmatrix(height,width);
    randomizer = new MTRand(time(0));
    init();
}

Model::~Model(){
	
}

void Model::init(){
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width;x++){
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
   	matrix.set((x+1)%width,y,new Cell(ZOMBIE));
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

void Model::moveZombie(int x,int y){
	if (timeToDecompose())
	{
		//tempMatrix.set(x,y,EMPTY);
	}else if(timeToMoveZombie()){
		int x2,y2;
		getSquareToMoveTo(x,y,&x2,&y2);
	}else{
		//tempMatrix.set(x,y,ZOMBIE);
	}
}

void Model::moveInfected(int x,int y){
    if (this->timeToBecomeZombie())
    {
    	//tempMatrix.set(x,y,ZOMBIE);
    	moveZombie(x,y);
    }else{
    	moveHuman(x,y);
    }
}

void Model::getSquareToMoveTo(int fromX,int fromY,int *toX, int *toY){
	double r = (*randomizer)();
	*toX = fromX;
	*toY = fromY;
	if (r < 0.25) // left
	{
		*toX = (fromX-1+width)%width;
	}else if (r < 0.5) // right
	{
		*toX = (fromX+1)%width;
	}else if (r < 0.75) // up
	{
		*toY = ((fromY-1+height)%height);
	}else// down
	{
		*toY = ((fromY+1)%height);
	}
}

void Model::moveHuman(int x,int y){
	if (this->timeToDie())
	{
		//tempMatrix.set(x,y,EMPTY);
	}else{
		int x2,y2;
		getSquareToMoveTo(x,y,&x2,&y2);
		if (matrix(x2,y2)->kind() != EMPTY) // zombie encounter!!
		{
			if ((*(this->randomizer))() < brainEatingProb)
			{
				//tempMatrix.set(x,y,INFECTED);
			}
		}else if(matrix(x2,y2) == EMPTY)
		{
			//tempMatrix.set(x,y,EMPTY);
			//tempMatrix.set(x2,y2,HUMAN);
		}else{
			//tempMatrix.set(x,y,HUMAN);
		}
	}
}

void Model::move(int x,int y){
	switch(matrix(x,y)->kind()){
        case HUMAN:
            moveHuman(x,y);
        break;
        case EMPTY:
            if(timeToBeBorn()){
                //tempMatrix.set(x,y,HUMAN);
            }
        break;   
        case INFECTED:
            moveInfected(x,y);
        break;
        case ZOMBIE:
        	moveZombie(x,y);
        break;
    }
    
}

void Model::moveAll(int iterations){
	for (int i = 0; i < iterations; i++)
	{
		for (int y = 0; y < height; y++)
	    {
	        for (int x = 0; x < width;x++){
	            move(x,y);
	        }
	    }
		matrix.swap(tempMatrix);    
	}
    
}