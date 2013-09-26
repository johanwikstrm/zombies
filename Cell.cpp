#include "Cell.h"

Cell::Cell(int kind){
	k = kind;
	mFlag = false;
}

Cell * Cell::Spawn(int newKind){
	// TODO: copy any important variables
	c = new Cell(newKind);
	c->mFlag = mFlag;
	return c;
}

Cell::~Cell(){
	
}

int Cell::kind(){
	return k;
}

/*void Cell::setKind(int kind){
	k = kind;
}*/

bool Cell::moveFlag(){
	return mFlag;
}

void Cell::setMoveFlag(bool moveFlag){
	mFlag = moveFlag;
}