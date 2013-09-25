using namespace std;
#include <iostream>
#include <cassert>
#include "Cell.h"
#include "constants.h"

int main(int argc, char const *argv[])
{
	Cell c = Cell(ZOMBIE);
	c.setKind(EMPTY);
	assert(c.kind()==EMPTY);
	assert(c.moveFlag() == false);
	c.setMoveFlag(true);
	assert(c.moveFlag()==true);
	
	return 0;
}