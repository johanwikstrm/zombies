#include <iostream>
#include "Cell.h"

using namespace std;

Cell::Cell(int kind){
	k = kind;
	mFlag = false;
}

Cell::Cell(const Cell& c){
    k = c.k;
    mFlag = c.mFlag;
}

Cell * Cell::Spawn(int newKind){
	// TODO: copy any important variables
	Cell* c = new Cell(newKind);
	c->mFlag = mFlag;
	return c;
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
