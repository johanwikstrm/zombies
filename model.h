#ifndef MODEL_H
#define MODEL_H
#include "mtrand.h"
#include "Dmatrix.h"


#define ITERATIONS 10000

#define NOTHING 0
#define HUMAN 1
#define INFECTED 2
#define ZOMBIE 3

class Model
{
public:
	Model(int width,int height,double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
		brainEatingProb,double infectedToZombieProb);
	~Model();
	void moveAll();
	void printStats();
	int getCount(int kind);
private:
	Dmatrix matrix;
	Dmatrix swap;
	MTRand *randomizer;
	int width, height;
	double naturalBirthProb, naturalDeathRisk, initialPopDensity, brainEatingProb, infectedToZombieProb;
	void move(int x,int y);
	void moveHuman(int x,int y);
	void moveInfected(int x,int y);
	void moveZombie(int x,int y);

	bool timeToDie();
	bool timeToBeBorn();
	bool timeToBecomeZombie();
	void init();
};

#endif