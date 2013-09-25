#include "Cell.h"

Cell::Cell(int kind){
	k = kind;
	mFlag = false;
}

Cell::~Cell(){
	
}

int Cell::kind(){
	return k;
}

void Cell::setKind(int kind){
	k = kind;
}

bool Cell::moveFlag(){
	return mFlag;
}

void Cell::setMoveFlag(bool moveFlag){
	mFlag = moveFlag;
}