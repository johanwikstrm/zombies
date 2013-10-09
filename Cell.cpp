#include <iostream>
#include "Cell.h"

using namespace std;

Cell::Cell(uint32_t kind){
	k = kind;
	mFlag = false;
}

Cell::Cell(const Cell& c){
    k = c.k;
    mFlag = c.mFlag;
}

Cell::~Cell(){
	
}

uint32_t Cell::getKind(){
	return k;
}

void Cell::setKind(uint32_t kind){
	k = kind;
}

bool Cell::getMoveFlag(){
	return mFlag;
}

void Cell::setMoveFlag(bool moveFlag){
	mFlag = moveFlag;
}
