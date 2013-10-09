using namespace std;
#include <iostream>
#include <fstream>
#include <ctime>
#include <cassert>
#include <string.h>
#include "Array.h"

Array::Array()
{
    size = 0;
}

Array::Array(uint32_t s, int kind)
{
    size = s;
    if (size == 0)  {
        return;
    }
    array = new Cell*[size];
    for (uint32_t i = 0; i < size; i++) {
        array[i] = new Cell(kind);
    }
}


Array::Array(const Array& P)
{
    size = P.size;
    if (size == 0) {
        return;
    }
    array = new Cell*[size];
    for (uint32_t i = 0; i < size; i++) {
        array[i] = new Cell(*P.array[i]);
    }
}

Array::~Array()
{
    if (size == 0) {
        return;
    }
    for (uint32_t i = 0; i < size; i++)
    {
        delete array[i];
    }
    delete [] array;
}


uint32_t Array::getSize() const 
{
    return size;
}

void Array::print() const
{
    cout <<"Tableau : \n";
    for (uint32_t i = 0; i < size; i++) {
        cout <<array[i] <<" ";
    }
    cout <<"\n";
}

Cell* Array::operator()(uint32_t i) const
{
    assert(i >= 0 && i < size);
    return array[i];
}

Cell*& Array::operator()(uint32_t i) 
{
    assert(i >= 0 && i < size);
    return array[i];
}

Array& Array::operator=(const Array& P)
{
    if (size == 0 && P.size == 0) {
        return *this;
    }
    if (size != 0) {
        for (uint32_t i = 0; i < size; i++) {
            delete array[i];
        }
        delete [] array;
    }
    size = P.size;
    array = new Cell*[size];
    for (uint32_t i = 0; i < size; i++) {
        array[i] = new Cell(*P.array[i]);
    }
    return *this;
}



bool Array::operator==(const Array& P) const
{
    if (size == P.getSize()) {
        for (uint32_t i = 0; i < size; i++) {
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
