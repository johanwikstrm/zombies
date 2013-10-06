using namespace std;
#include <iostream>
#include <cassert>
#include "Cell.h"
#include "constants.h"

int main(int argc, char const *argv[])
{
	Cell c = Cell(ZOMBIE);
	Cell *c2;
	c2 = c.Spawn(EMPTY);
	assert(c2->kind()==EMPTY);
	assert(c.moveFlag() == false);
	c.setMoveFlag(true);
	assert(c.moveFlag()==true);
	
	delete c2;
	return 0;
}