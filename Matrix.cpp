using namespace std;
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "Matrix.h"
#include "constants.h"

// Intializing all cells to empty by default
Matrix::Matrix(uint32_t h, uint32_t w):Array(h*w , EMPTY)
{
    height = h;
    width = w;
    counts = new uint32_t[NKINDS];
    for (uint32_t i = 0; i < NKINDS; i++) {
        counts[i] = 0;
    }
    counts[EMPTY] = h*w;
}

Matrix::Matrix(const Matrix& P):Array(P)
{
    width = P.width;
    height = P.height;
    counts = new uint32_t[NKINDS];
    for (int i = 0; i < NKINDS; i++) {
        counts[i] = P.counts[i];
    }
}

Matrix::Matrix():Array()
{
    width = 0;
    height = 0;
    counts = new uint32_t[NKINDS];
    for (uint32_t i = 0; i < NKINDS; i++) {
        counts[i] = 0;
    }
}

Matrix::~Matrix()
{
    delete[] counts;
}

uint32_t Matrix::getHeight() const
{
    return height;
}

uint32_t Matrix::getWidth() const
{
    return width;
}

uint32_t Matrix::getCount(uint32_t kind) const
{
    return counts[kind];
}

void Matrix::print() const
{
    Matrix M = *this;
    cout <<"Matrix : \n";
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            char kind = 'X';
            switch(M(x, y)->getKind()) {
                case ZOMBIE:
                kind = 'Z';
                break;
                case HUMAN:
                kind = 'H';
                break;
                case INFECTED:
                kind = 'I';
                break;
                case EMPTY:
                kind = 'E';
                break;
            }
            cout << kind <<" ";
        }
        cout <<"\n";
    }
}

void Matrix::printMoveFlags() const
{
    Matrix M = *this;
    cout <<"Matrix : \n";
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            char mf;
            if (M(x,y)->getKind() == EMPTY) {
                mf = 'X';
            } else {
                mf = M(x,y)->getMoveFlag()? 'T':'F';
            }
            cout << mf <<" ";
        }
        cout <<"\n";
    }
}


void Matrix::set(uint32_t x, uint32_t y, uint32_t k) 
{
    int previousKind = (*this)(x, y)->getKind();
    counts[previousKind]--;
    counts[k]++;
    Array::set(y*width+x, k);
}

void Matrix::move(uint32_t oldX, uint32_t oldY, uint32_t newX, uint32_t newY)
{
    uint32_t person = (*this)(oldX, oldY)->getKind();
    uint32_t destination = (*this)(newX, newY)->getKind();
    
    // Make sure we move a person (human, infected or zombie, i.e not empty)
    assert(person != EMPTY);
    // Make sure the square the person is trying to move to is empty
    assert(destination == EMPTY);
   
    // The move
    Array::set(oldY*width+oldX, EMPTY);
    Array::set(newY*width+newX, person);
}

// Only a HUMAN or an INFECTED can get INFECTED
void Matrix::getInfected(uint32_t x, uint32_t y) {
    Cell* person = this->operator()(x, y);
    assert(person->getKind() == HUMAN || person->getKind() == INFECTED);
    int oldkind = person->getKind();
    person->setKind(INFECTED);
    counts[oldkind]--;
    counts[INFECTED]++; 
}


Cell* Matrix::operator()(uint32_t x, uint32_t y) const {
    assert(y>=0 && y<height && x>=0 && x<width);
    return  Array::operator()(y*width + x);
}

Cell* Matrix::operator()(Coord c) const {
    return Matrix::operator()(c.getX(), c.getY());
}

Cell*& Matrix::operator()(uint32_t x, uint32_t y) 
{
    assert(y>=0 && y<height && x>=0 && x<width);
    return Array::operator()(y*width + x);
}

Cell*& Matrix::operator()(Coord c) 
{
    return Matrix::operator()(c.getX(), c.getY());
}

bool Matrix::operator==(const Matrix& M)
{
    Matrix N = *this;
    if (N.height == M.height && N.width == M.width) {
        for (uint32_t i = 0; i < height; i++) {
            for (uint32_t j = 0; j < width; j++){
                if (M(i, j)->getKind() != N(i, j)->getKind()) {
                    return false;  
                }
            } 
        }
        // Do not need to check the equality of the values in counts
        return true;
    } else {
        return false;
    }
}

Matrix& Matrix::operator=(const Matrix& P)
{
    Array::operator=(P);
    width = P.width;
    height = P.height;
    memcpy(counts, P.counts, NKINDS*sizeof(int));
    return *this;
}

void Matrix::swap(Matrix& M)
{
    assert(M.width == this->width);
    assert(M.height == this->height);
    Array::swap(M);
    std::swap(this->counts, M.counts); 
}

Array* Matrix::extractColumn(uint32_t col) {
    Array* column = new Array(height);
    for (uint32_t y = 0; y < height; y++) {
       	column->set(y,(*this)(col, y)->getKind()); 
    }
    return column;
} 

Array* Matrix::extractRow(uint32_t r) {
    Array* row = new Array(width);
    for (uint32_t x = 0; x < width; x++) {
        row->set(x,(*this)(x,r)->getKind()); 
    }
    return row;
}

/**
// Returns the number of collisions
int Matrix::insertColumnWithCollisions(Array * toInsert,uint32_t col){
    assert(toInsert->getSize() == height);
    int collisions = 0;
    for (uint32_t y = 0; y < height; y++) {
        int oldKind = (*this)(col,y)->getKind();
        int newKind = (*toInsert)(y)->getKind();
        if (oldKind != EMPTY && newKind != EMPTY){
            //cout << "Collision inserting " << kindstr((*toInsert)(y)->getKind())
            //    << " into cell with " << kindstr((*this)(col,y)->getKind()) << " in it at "
            //    << col<<","<<y<<endl;
            collisions++;
        }else if (oldKind == EMPTY){
            this->set(col,y,(*toInsert)(y)->getKind());    
        }    
    }
    return collisions;  
}

// Returns the number of collisions
int Matrix::insertRowWithCollisions(Array * toInsert,uint32_t row){
    assert(toInsert->getSize() == width);
    int collisions = 0;
    for (uint32_t x = 0; x < width; x++) {
        int oldKind = (*this)(x,row)->getKind();
        int newKind = (*toInsert)(x)->getKind();
        if (oldKind != EMPTY && newKind != EMPTY){
            //cout << "Collision inserting " << kindstr((*toInsert)(x)->getKind())
            //    << " into cell with " << kindstr((*this)(x,row)->getKind()) << " in it at "
            //    << x<<","<<row<<endl;
            collisions++;
        }else if (oldKind == EMPTY){
            this->set(x,row,(*toInsert)(x)->getKind());    
            (*this)(x,row)->setMoveFlag((*toInsert)(x)->getMoveFlag());
        }    
    }
    return collisions;  
}
*/

// Returns the number of collisions
int Matrix::insertColumnWithCollisions(Array * toInsert,uint32_t col){
    assert(toInsert->getSize() == height);
    int collisions = 0;
    for (uint32_t y = 0; y < height; y++) {
        int oldKind = (*this)(col,y)->getKind();
        int newKind = (*toInsert)(y)->getKind();
        if (oldKind != EMPTY && newKind != EMPTY){
            /*cout << "Collision inserting " << kindstr((*toInsert)(y)->getKind())
                << " into cell with " << kindstr((*this)(col,y)->getKind()) << " in it at "
                << col<<","<<y<<endl;*/
            collisions++;
        }else if (oldKind == EMPTY){
            this->set(col,y,(*toInsert)(y)->getKind());    
            (*this)(col,y)->setMoveFlag((*toInsert)(y)->getMoveFlag());
        }    
    }
    return collisions;  
}

// Returns the number of collisions
int Matrix::insertRowWithCollisions(Array * toInsert,uint32_t row){
    assert(toInsert->getSize() == width);
    int collisions = 0;
    for (uint32_t x = 0; x < width; x++) {
        int oldKind = (*this)(x,row)->getKind();
        int newKind = (*toInsert)(x)->getKind();
        if (oldKind != EMPTY && newKind != EMPTY){
            /*cout << "Collision inserting " << kindstr((*toInsert)(x)->getKind())
                << " into cell with " << kindstr((*this)(x,row)->getKind()) << " in it at "
                << x<<","<<row<<endl;*/
            collisions++;
        }else if (oldKind == EMPTY){
            this->set(x,row,(*toInsert)(x)->getKind());    
            (*this)(x,row)->setMoveFlag((*toInsert)(x)->getMoveFlag());
        }    
    }
    return collisions;  
}

/*  
    offset=0   offset=1
    X X X X X  E X E X E 
    X E E E X  X X X X X
    X E E E X  X X X X X
    X X X X X  E X E X E
*/
Array** Matrix::toSend(int offset){
    assert(width >=4 && height >= 4);// assuming a matrix that is at least 4x4
    assert(offset == 1 || offset == 0);
    Array **toRet = (Array**)calloc(4,sizeof(Array*));
    toRet[UP] = extractRow(offset);
    toRet[DOWN] = extractRow(height-1-offset);
    toRet[LEFT] = extractColumn(offset);
    toRet[RIGHT] = extractColumn(width-1-offset);
    return toRet;
}

/*  offset=0   offset=1
    X X X X X  E X E X E 
    X E E E X  X X X X X
    X E E E X  X X X X X
    X X X X X  E X E X E
*/
int Matrix::insertWithCollisions(Array** toInsert, int offset){
    assert(offset == 1 || offset == 0);
    assert(width >=4 && height >= 4);// assuming a matrix that is at least 4x4
    assert(toInsert[UP]->getSize() == width);
    assert(toInsert[DOWN]->getSize() == width);
    assert(toInsert[LEFT]->getSize() == height);
    assert(toInsert[RIGHT]->getSize() == height);
    int collisions =0;
    collisions += insertRowWithCollisions(toInsert[UP] , offset);
    collisions += insertRowWithCollisions(toInsert[DOWN] , height-1-offset);
    collisions += insertColumnWithCollisions(toInsert[LEFT] , offset);
    collisions += insertColumnWithCollisions(toInsert[RIGHT] , width-1-offset);
    return collisions;
}
