#include <iostream>
#include "Cell.h"

using namespace std;

Cell::Cell(int kind){
	k = kind;
	mFlag = false;
}

Cell * Cell::Spawn(int newKind){
	// TODO: copy any important variables
	Cell *c = new Cell(newKind);
	c->mFlag = mFlag;
	return c;
}

Cell::~Cell(){
	
}

Cell::Cell(const Cell& M){
	cout << "COPY CONSTRUCTOR CALLED!!!!\n";
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