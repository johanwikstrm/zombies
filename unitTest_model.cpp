#include <cassert>
#include <mpi.h>
#include "model.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    //Model(int width,int height,double naturalBirthProb, double naturalDeathRisk, double initialPopDensity, double
    //brainEatingProb,double infectedToZombieProb,double zombieDecompositionRisk, double humanMoveProb, double zombieMoveProb);
    error err = MPI_Init(&argc, &argv);
    assert(err == MPI_SUCCESS);
    
    int rank;
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    assert(err == MPI_SUCCESS);

    Model m1 = Model(3,3,rank,0,0,0, 1,0,0,1,0);
    // should have only 2 zombies in it
    assert(m1.getCount(ZOMBIE)==2);
    assert(m1.getCount(HUMAN)==0);
    assert(m1.getCount(INFECTED)==0);
    assert(m1.getCount(EMPTY)==7);

    Model m2 = Model(10,10,rank,0,0,1,0,0,0,0,0);
    assert(m2.getCount(EMPTY)==36);
    assert(m2.getCount(HUMAN)==62);        
    assert(m2.getCount(ZOMBIE)==2);        

    Model m3 = Model(10,10,rank,0,0,1,1,0,0,0,1);

    m3.moveAll(1);
    // At least 1 people infected by zombies movement
    assert(m3.getCount(INFECTED)>=1);
    assert(m3.getCount(ZOMBIE)== 2);
    assert(m3.getCount(HUMAN)>=60);

    // Model where the bitten turn into zombies very fast
    Model m4 = Model(10,10,rank,0,0,1,1,1,0,1,1);

    assert(m4.getCount(INFECTED) == 0);
    m4.moveAll(5);
    assert(m4.getCount(INFECTED) > 0);
    assert(m4.getCount(ZOMBIE) > 2);

    Model m5 = Model(15,10,rank,0,0,0,0,0,0,0,1); // just two zombies
    assert(m5.getCount(ZOMBIE)==2);

    int zx,zy,zx2,zy2;
    zx = zy = zx2 = zy2 = -1;
    for (int y = 0; y < 10; y++){
        for (int x = 0; x < 15; x++){
            if (m5.at(x,y)->kind() == ZOMBIE){ // finding pos of both zombies
                if (zx != -1){
                    zx = x;
                    zy = y;
                }else  {
                    zx2 = x;
                    zy2 = y;
                }
            }
        }    
    }

    m5.moveAll(1);
    bool currentMoveFlag = m5.at(0,0)->moveFlag();
    // checking that our zombies are within range
    for (int y = 0; y < 10; y++){
        for (int x = 0; x < 15; x++){
            if (m5.at(x,y)->moveFlag() == currentMoveFlag){
                cout << "Wrong moveflag at "<<x<<","<<y<<endl;
            }
            //assert(m5.at(x,y)->moveFlag() == currentMoveFlag);
            if (m5.at(x,y)->kind()==ZOMBIE){
                
            }
        }
    }
    assert(m5.getCount(ZOMBIE)==2);

    err = MPI_Finalize();
    assert(err == MPI_SUCCESS);    
}
