using namespace std;
#include <iostream>
#include <fstream>
#include <ctime>
#include <cassert>
#include <string.h>
#include "constants.h"
#include "Array.h"

Array::Array()
{
    dummy = new Cell(EMPTY);
    size = 0;
}

Array::Array(uint32_t s, uint32_t kind)
{
    size = s;
    if (size == 0)  {
        return;
    }
    array = new Cell*[size];
    dummy = new Cell(EMPTY);
    for (uint32_t i = 0; i < size; i++) {
        if (kind == EMPTY) {
            array[i] = NULL;
        } else {
            array[i] = new Cell(kind);
        }
    }
}

Array::Array(const Array& P)
{
    size = P.size;
    if (size == 0) {
        return;
    }
    array = new Cell*[size];
    dummy = new Cell(EMPTY);
    for (uint32_t i = 0; i < size; i++) {
        if (P.array[i] != NULL) {
            array[i] = new Cell(*P.array[i]);
        } else {
            array[i] = NULL;
        }
    }
}

Array::~Array()
{
    delete dummy;
    if (size == 0) {
        return;
    }
    for (uint32_t i = 0; i < size; i++) {
        if (array[i] != NULL) {
            delete array[i];
            array[i] = NULL;
        }
    }
    delete [] array;
    array = NULL;
}


uint32_t Array::getSize() const 
{
    return size;
}

Cell* Array::operator()(uint32_t i) const
{
    assert(i >= 0 && i < size);
    if (array[i] == NULL){
        return  dummy;
    }else{
        return array[i];    
    }
}

Cell*& Array::operator()(uint32_t i) 
{
    assert(i >= 0 && i < size);
    if (array[i] == NULL){
        return  dummy;
    }else{
        return array[i];    
    }
}

void Array::set(uint32_t x, uint32_t k) 
{
    int previousKind = (*this)(x)->getKind();
    if (previousKind != EMPTY) {
        delete array[x];
        array[x] = NULL;
    }
    if (k != EMPTY) {
        array[x] = new Cell(k);
    }
}

Array& Array::operator=(const Array& P)
{
    if (size == 0 && P.size == 0) {
        return *this;
    }
    if (size != 0) {
        for (uint32_t i = 0; i < size; i++) {
            if (array[i] != NULL) {
                delete array[i];
                array[i] = NULL;
            }
        }
        delete [] array;
        array = NULL;
    }
    size = P.size;
    array = new Cell*[size];
    for (uint32_t i = 0; i < size; i++) {
        if (P.array[i] != NULL) {
            array[i] = new Cell(*P.array[i]);
        } else {
            array[i] = NULL;
        }
    }
    return *this;
}

bool Array::operator==(const Array& P) const
{
    if (size == P.getSize()) {
        for (uint32_t i = 0; i < size; i++) {
            // TODO : do not compare the pointers !!
            if (array[i] != P(i)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

// The two arrays must have the same length
void Array::swap(Array& P) 
{
    Cell** tmp = P.array;
    P.array = array;
    array = tmp;
}
