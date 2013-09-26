#ifndef MODEL_H
#define MODEL_H
#include "mtrand.h"
#include "constants.h"
#include "Dmatrix.h"


class Model
{
public:
	Model(int width,int height,double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
		brainEatingProb,double infectedToZombieProb,double zombieDecompositionRisk, double humanMoveProb, double zombieMoveProb);
	~Model();
	void moveAll(int iterations=1);
	void printStats();
	int getCount(int kind);
private:
	Dmatrix matrix;
	Dmatrix tempMatrix;
	MTRand *randomizer;
	int width, height;
	double naturalBirthProb, naturalDeathRisk, initialPopDensity, brainEatingProb, infectedToZombieProb;
	double zombieDecompositionRisk,humanMoveProb,zombieMoveProb;
	void move(int x,int y);
	void moveHuman(int x,int y);
	void moveInfected(int x,int y);
	void moveZombie(int x,int y);
	void getSquareToMoveTo(int fromX,int fromY,int *toX, int *toY);

	bool timeToDie();
	bool timeToDecompose();
	bool timeToBeBorn();
	bool timeToBecomeZombie();
	bool timeToMoveHuman();
	bool timeToMoveZombie();
	void init();
};

#endif