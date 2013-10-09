using namespace std;
#include <iostream>
#include <cassert>
#include "Cell.h"
#include "constants.h"

int main(int argc, char const *argv[])
{
	Cell c = Cell(ZOMBIE);
	assert(c.getKind() == ZOMBIE);
	assert(c.getMoveFlag() == false);
	c.setMoveFlag(true);
	assert(c.getMoveFlag() == true);
	return 0;
}
