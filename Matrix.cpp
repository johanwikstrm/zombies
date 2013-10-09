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
    dummy = new Cell(EMPTY); // is used as a replacer for ALL empty cells
    counts = new uint32_t[numCellKinds];
    for (uint32_t i = 0; i < numCellKinds; i++) {
        counts[i] = 0;
    }
    counts[EMPTY] = h*w;
}

uint32_t Matrix::kind(Cell* ptr){
    if (ptr == NULL) {
        return EMPTY;
    }else {
        return ptr->getKind();
    }
}

Matrix::Matrix(const Matrix& P):Array(P)
{
    width = P.width;
    height = P.height;
    dummy = new Cell(EMPTY);
    counts = new uint32_t[numCellKinds];
    for (int i = 0; i < numCellKinds; i++) {
        counts[i] = P.counts[i];
    }
}

Matrix::Matrix():Array()
{
    width = 0;
    height = 0;
    dummy = new Cell(EMPTY);
    counts = new uint32_t[numCellKinds];
    for (uint32_t i = 0; i < numCellKinds; i++) {
        counts[i] = 0;
    }
}

Matrix::~Matrix()
{
    delete dummy;
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


void Matrix::set(uint32_t x, uint32_t y, uint32_t k) 
{
    int previousKind = kind(array[y*width + x]);
    counts[previousKind]--;
    if (array[y*width+x] != NULL) {
        delete array[y*width+x];
        array[y*width + x] = NULL;
    }
    if (k != EMPTY) {
        array[y*width + x] = new Cell(k);
    }
    counts[k]++;
}

// TODO : unitTest !!!!
void Matrix::move(uint32_t oldX, uint32_t oldY, uint32_t newX, uint32_t newY)
{
    Matrix M = *this;
    Cell* person = M(oldX, oldY);
    Cell* destination = M(newX, newY);
    // Make sure we move a person (human, infected or zombie, i.e not empty)
    assert(kind(person) != EMPTY);
    // Make sure the square the person is trying to move to is empty
    assert(kind(destination) == EMPTY);
    // The move
    Cell* tmp = person;
    person = destination;
    destination = tmp;
}

// TODO unitTest
void Matrix::getInfected(uint32_t x, uint32_t y) {
    Cell* person = this->operator()(x, y);
    assert(kind(person) == HUMAN || kind(person) == INFECTED);
    this->operator()(x, y)->setKind(INFECTED);
    counts[HUMAN]--;
    counts[INFECTED]++; 
}


Cell* Matrix::operator()(uint32_t x, uint32_t y) const {
    assert(y>=0 && y<height && x>=0 && x<width);
    Cell* c = Array::operator()(y*width + x);
    if (c == NULL) {
        return dummy;
    } else {
        return c;
    }
}

Cell* Matrix::operator()(Coord c) const {
    assert(c.getY()>=0 && c.getY()<height && c.getX()>=0 && c.getX()<width);
    Cell* cell = Matrix::operator()(c.getX(), c.getY());
    return cell;
}

Cell*& Matrix::operator()(uint32_t x, uint32_t y) 
{
    assert(y>=0 && y<height && x>=0 && x<width);
    Cell* c = Array::operator()(y*width + x);
    if (c == NULL) {
        return dummy;
    } else {
        return Array::operator()(y*width + x);
    }
}

Cell*& Matrix::operator()(Coord c) 
{
    assert(c.getY()>=0 && c.getY()<height && c.getX()>=0 && c.getX()<width);
    return Matrix::operator()(c.getX(), c.getY());
}

bool Matrix::operator==(const Matrix& M)
{
    Matrix N = *this;
    if (N.height == M.height && N.width == M.width) {
        for (uint32_t i = 0; i < height; i++) {
            for (uint32_t j = 0; j < width; j++){
                // TODO: deep equals
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
    memcpy(counts, P.counts, numCellKinds*sizeof(int));
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
        if ((*this)(col, y) != NULL) {
            (*column)(y) = new Cell(*(*this)(col, y)); 
        } else {
            (*column)(y) = NULL;
        }
    }
    return column;
} 

Array* Matrix::extractRow(uint32_t r) {
    Array* row = new Array(width);
    for (uint32_t x = 0; x < width; x++) {
        if ((*this)(x, r) != NULL) {
            (*row)(x) = new Cell(*(*this)(x, r)); 
        } else {
            (*row)(x) = NULL;
        }
    }
    return row;
} 

Array** Matrix::toSend(){
    assert(width >=4 && height >= 4);// assuming a'matrix that is at least 4x4
    Array **toRet = (Array**)calloc(4,sizeof(Array*));
    toRet[UP] = extractRow(1);
    toRet[DOWN] = extractRow(height-2);
    toRet[LEFT] = extractColumn(1);
    toRet[RIGHT] = extractColumn(width-2);
    return toRet;
}

void Matrix::insert(Array** toInsert){

}
