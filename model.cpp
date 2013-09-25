#include "model.h"
#include <iostream>

using namespace std;

Model::Model(int width,int height,double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
		brainEatingProb,double infectedToZombieProb){
	this->width = width;
	this->height = height;
	this->naturalBirthProb = naturalBirthProb;
	this->naturalDeathRisk = naturalDeathRisk;
	this->initialPopDensity = initialPopDensity;
	this->brainEatingProb = brainEatingProb;
	this ->infectedToZombieProb = infectedToZombieProb;

	matrix = Dmatrix(height,width);
  	swap = Dmatrix(height,width);
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
                this->matrix.set(x,y,HUMAN);
            }	
        }
    }
    for (int i = 0; i < 2; i++)
    {
    	int x = (int)((*randomizer)()*width);
    	int y = (int)((*randomizer)()*height);
    	this->matrix.set(x,y,ZOMBIE);
    }
    
}

bool Model::timeToDie(){
    return (*randomizer)() < naturalDeathRisk;
}

// weighs the result on the number of people in the matrix and 
// the number of empty spaces
bool Model::timeToBeBorn(){
    return (*randomizer)() < naturalBirthProb*matrix.getCount(HUMAN)/matrix.getCount(NOTHING);
}

bool Model::timeToBecomeZombie(){
    return (*randomizer)() < infectedToZombieProb;
}

int Model::getCount(int kind){
	return matrix.getCount(kind);
}

void Model::printStats(){
    cout << Model::matrix.getCount(HUMAN)<< "\t"<<Model::matrix.getCount(INFECTED) << "\t"<<Model::matrix.getCount(ZOMBIE)<< "\t"<<Model::matrix.getCount(NOTHING)<<"\n";
}

void Model::moveZombie(int x,int y){

}

void Model::moveInfected(int x,int y){
    if (this->timeToBecomeZombie())
    {
     	  	
    }
}

void Model::moveHuman(int x,int y){
	if (this->timeToDie())
	{
		swap.set(x,y,NOTHING);
	}else{
		double r = (*randomizer)();
		int x2,y2;
		x2 = x;
		y2 = y;
		if (r < 0.2) // left
		{
			x2 = (x-1+width)%width;
		}else if (r < 0.4) // right
		{
			x2 = (x+1)%width;
		}else if (r < 0.6) // up
		{
			y2 = ((y-1+height)%height);
		}else if (r < 0.8) //down
		{
			y2 = ((y+1)%height);
		}
		if (matrix(x2,y2) == ZOMBIE) // zombie encounter!!
		{
			if ((*(this->randomizer))() < brainEatingProb)
			{
				swap.set(x,y,INFECTED);
			}
		}else if(matrix(x2,y2) == NOTHING)
		{
			swap.set(x,y,NOTHING);
			swap.set(x2,y2,HUMAN);
		}// else do nothing
	}
}

void Model::move(int x,int y){
	switch(matrix(x,y)){
        case HUMAN:
            moveHuman(x,y);
        break;
        case NOTHING:
            if(timeToBeBorn()){
                swap.set(x,y,HUMAN);
            }
        break;   
        case INFECTED:
            moveInfected(x,y);
        case ZOMBIE:
        	moveZombie(x,y);
        break;
    }
    matrix.swap(swap);
}

void Model::moveAll(){
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width;x++){
            
        }
    }
}