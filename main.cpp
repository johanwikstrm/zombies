#include <time.h>
#include "mtrand.h"
#include <stdlib.h>
#include <iostream>
#include "Dmatrix.h"

#define WIDTH 10
#define HEIGHT 10
#define ITERATIONS 10000

#define NOTHING 0
#define HUMAN 1
#define INFECTED 2
#define ZOMBIE 3

// natural death rate for humans about once every 60 years
#define NATURAL_DEATH_RISK (1.0/(60.0*365.0))
// simplest possible birth control mechanism
#define NATURAL_BIRTH_PROB (1.0/(60.0*365.0))

#define POP_DENSITY 0.01
// Probabiblity of getting your brain eaten when encountering a zombie
#define BRAIN_EATING_PROB 1
#define INFECTED_TO_ZOMBIE_PROB (1.0/10.0)


using namespace std;

MTRand* randomizer;

void init(Dmatrix &matrix){
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH;x++){
            //if (distr(twister) < POP_DENSITY)
            if (drand48() < POP_DENSITY)
            {
                matrix.set(x,y,HUMAN);
            }	
        }
    }
    // TODO: make random
    matrix.set(WIDTH/2,HEIGHT/2,ZOMBIE);
    matrix.set(WIDTH/2,HEIGHT/2+1,ZOMBIE);
}
bool timeToDie(){
    return (*randomizer)() < NATURAL_DEATH_RISK;
}

// weighs the result on the number of people in the matrix and 
// the number of empty spaces
bool timeToBeBorn(Dmatrix &matrix){
    return (*randomizer)() < NATURAL_BIRTH_PROB*matrix.getCount(HUMAN)/matrix.getCount(NOTHING);
}

bool timeToBecomeZombie(){
    return (*randomizer)() < INFECTED_TO_ZOMBIE_PROB;
}

void move(Dmatrix &from, Dmatrix &to){
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH;x++){
            switch(from(x,y)){
                case HUMAN:
                    if (timeToDie())
                    {
                        to.set(x,y,NOTHING);
                    }else{
                    double r = drand48();
                    int x2,y2;
                    x2 = x;
                    y2 = y;
                    if (r < 0.2) // left
                    {
                        x2 = (x-1+WIDTH)%WIDTH;
                    }else if (r < 0.4) // right
                    {
                        x2 = (x+1)%WIDTH;
                    }else if (r < 0.6) // up
                    {
                        y2 = ((y-1+HEIGHT)%HEIGHT);
                    }else if (r < 0.8) //down
                    {
                        y2 = ((y+1)%HEIGHT);
                    }
                    if (from(x2,y2) == ZOMBIE) // zombie encounter!!
                    {
                        if ((*randomizer)() < BRAIN_EATING_PROB)
                        {
                            to.set(x,y,INFECTED);
                        }
                    }else if(from(x2,y2) == NOTHING)
                    {
                        to.set(x,y,NOTHING);
                        to.set(x2,y2,HUMAN);
                    }
                }
                break;
                case NOTHING:
                    if(timeToBeBorn(from)){
                        to.set(x,y,HUMAN);
                    }  
                break;   
                case INFECTED:
                    if (timeToBecomeZombie())
                    {
                            
                    }
                break;
            }
        }
    }
}

void printStats(Dmatrix &matrix){
    cout << matrix.getCount(HUMAN)<< "\t"<<matrix.getCount(INFECTED) << "\t"<<matrix.getCount(ZOMBIE)<< "\t"<<matrix.getCount(NOTHING) <<endl;
}


int main(int argc, char const *argv[])
{
    Dmatrix matrix = Dmatrix(HEIGHT,WIDTH);
    Dmatrix matrix2 = Dmatrix(HEIGHT,WIDTH);
    randomizer = new MTRand(time(0));
    //init(matrix,distr,twister);
    init(matrix);

    for (int i = 0; i < ITERATIONS; i++) {
        move(matrix,matrix2);
        printStats(matrix2);
    }

    return 0;
}
