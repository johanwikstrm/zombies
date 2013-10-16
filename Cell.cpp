#include <iostream>
#include "Cell.h"

using namespace std;

Cell::Cell(uint32_t kind, uint32_t sex){
	this->k = kind;
        this->sex = sex;  
	this->mFlag = false;
}

Cell::Cell(const Cell& c){
    k = c.k;
    sex = c.sex;
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

uint32_t Cell::getSex(){
	return sex;
}

void Cell::setSex(uint32_t sex){
	this->sex = sex;
}


bool Cell::getMoveFlag(){
	return mFlag;
}

void Cell::setMoveFlag(bool moveFlag){
	mFlag = moveFlag;
}
