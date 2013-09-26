#include <cassert>
#include "model.h"
#include <iostream>

using namespace std;

int main ()
{
  //Model(int width,int height,double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
  //    brainEatingProb,double infectedToZombieProb,double zombieDecompositionRisk, double humanMoveProb, double zombieMoveProb);
    Model m1 = Model(3,3,0,0,0, 1,0,0,1,0);
    // should have only 2 zombies in it
    assert(m1.getCount(ZOMBIE)==2);
    assert(m1.getCount(HUMAN)==0);
    assert(m1.getCount(INFECTED)==0);
    assert(m1.getCount(EMPTY)==7);

    Model m2 = Model(10,10,0,0,1,0,0,0,0,0);
    assert(m2.getCount(EMPTY)==0);
    assert(m2.getCount(HUMAN)==98);        
    assert(m2.getCount(ZOMBIE)==2);        

    Model m3 = Model(10,10,0,0,1,1,0,0,0,1);
    
    m3.moveAll(1);
    
    // Exactly two people infected by zombies movement
    assert(m3.getCount(INFECTED)==2);
    assert(m3.getCount(ZOMBIE)== 2);
    assert(m3.getCount(HUMAN)==96);
    
    // Model where the bitten turn into zombies very fast
    Model m4 = Model(10,10,0,0,1,1,1,0,1,1);

    assert(m4.getCount(INFECTED) == 0);
    m4.moveAll(5);
    assert(m4.getCount(INFECTED) > 0);
    assert(m4.getCount(ZOMBIE) > 2);
}
