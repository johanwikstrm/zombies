#include <cassert>
#include "model.h"

int main ()
{
  //Model m1 = Model(WIDTH,HEIGHT,NATURAL_BIRTH_PROB,NATURAL_DEATH_RISK,POP_DENSITY, BRAIN_EATING_PROB,INFECTED_TO_ZOMBIE_PROB);
    Model m1 = Model(2,2,0,0,0, 1,0);
    // should have only 2 zombies in it
    assert(m1.getCount(ZOMBIE)==2);
    assert(m1.getCount(HUMAN)==0);
    assert(m1.getCount(INFECTED)==0);
    assert(m1.getCount(NOTHING)==2);

    Model m2 = Model(10,10,0,0,0, 1,0);


}
