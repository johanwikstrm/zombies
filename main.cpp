#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "model.h"

using namespace std;

#define WIDTH 100   
#define HEIGHT 100

// natural death rate for humans about once every 60 years
#define NATURAL_DEATH_RISK (1.0/(60.0*365.0))
// simplest possible birth control mechanism
#define NATURAL_BIRTH_PROB (1.0/(60.0*365.0))

#define POP_DENSITY 0.1
// Probabiblity of getting your brain eaten when encountering a zombie
#define BRAIN_EATING_PROB 1
#define INFECTED_TO_ZOMBIE_PROB (1.0/10.0)


int main(int argc, char const *argv[])
{
    Model m = Model(WIDTH,HEIGHT,NATURAL_BIRTH_PROB,NATURAL_DEATH_RISK,POP_DENSITY, BRAIN_EATING_PROB,INFECTED_TO_ZOMBIE_PROB);
    for (int i = 0; i < ITERATIONS; i++) {
        m.moveAll();
        m.printStats();
    }
    return 0;
}
