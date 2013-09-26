using namespace std;
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "Dmatrix.h"
#include "constants.h"


// constructeur avec paramètres
// Intializing all cells to empty by default
Dmatrix::Dmatrix(uint32_t h, uint32_t w):Darray(h*w , EMPTY)
{
    height = h;
    width = w;
    dummy = new Cell(EMPTY); // is used as a replacer for ALL empty cells
    counts = (uint32_t*)calloc(numCellKinds, sizeof(uint32_t));
    counts[EMPTY] = h*w;
}

int Dmatrix::kind(Cell* ptr){
    if (ptr == 0)
    {
        return EMPTY;
    }else{
        return ptr->kind();
    }
}

// constructeur par copie
// On choisit que si le vecteur P est propriétaire de ses données, le vecteur créé le sera aussi
// mais si le vecteur P n'est pas propriétaire de ses données, le vecteur créé ne le sera pas non plus
Dmatrix::Dmatrix(const Dmatrix& P):Darray(P)
{
    width = P.width;
    height = P.height;
    dummy = new Cell(EMPTY);
    counts = (uint32_t*)calloc(numCellKinds, sizeof(uint32_t));
    for (int i = 0; i < numCellKinds; i++) {
        counts[i] = P.counts[i];
    }
}

Dmatrix::Dmatrix():Darray()
{
    width = 0;
    height = 0;
    counts = (uint32_t*)calloc(numCellKinds, sizeof(uint32_t));
}

Dmatrix::~Dmatrix()
{
    free(counts);
}


///////////////////////////////////////////////////////
////////// Les méthodes de la classe Dmatrix ////////// 
///////////////////////////////////////////////////////

uint32_t Dmatrix::getHeight() const
{
    return height;
}

uint32_t Dmatrix::getWidth() const
{
    return width;
}

uint32_t Dmatrix::getCount(int kind) const
{
    return counts[kind];
}

void Dmatrix::print() const
{
    // Création d'un alias sur this
    Dmatrix M = *this;
    // Affichage de M
    cout <<"Matrix : \n";
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            char kind = 'E';
            switch(M(i, j)->kind()){
                case ZOMBIE:
                kind = 'Z';
                break;
                case HUMAN:
                kind = 'H';
                break;
                case INFECTED:
                kind = 'I';
                break;
            }
            cout << kind <<" ";
        }
        cout <<"\n";
    }
}


void Dmatrix::set(uint32_t i, uint32_t j, Cell* newValue) 
{
    Dmatrix M = *this;
    Cell* previousValue = M(i, j);
    counts[kind(previousValue)] --;
    array[i*height+j]=newValue;
    counts[kind(newValue)] ++;
}

Cell* Dmatrix::operator()(uint32_t i, uint32_t j) const
{
    assert(i>=0 && i<height && j>=0 && j<width);
    Cell * c = Darray::operator()(i*height + j);
    if (c== EMPTY){
        return dummy;
    }else {
        return c;
    }
}

Cell*& Dmatrix::operator()(uint32_t i, uint32_t j) 
{
    assert(i>=0 && i<height && j>=0 && j<width);
    Cell * c = Darray::operator()(i*height + j);
    if (c== EMPTY){
        return dummy;
    }else {
        return c;
    }
}

bool Dmatrix::operator==(const Dmatrix& M)
{
    // Construction d'un alias sur this
    Dmatrix N = *this;
    // Vérifie que les deux matrices ont la même taille
    if (N.height == M.height && N.width == M.width) {
        // Vérifie l'égalite des valeurs
        for (uint32_t i = 0; i < height; i++) {
            for (uint32_t j = 0; j < width; j++){
                // TODO: deep equals
                if (M(i, j)->kind() != N(i, j)->kind()) {
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

Dmatrix& Dmatrix::operator=(const Dmatrix& P)
{
    Darray::operator=(P);
    width = P.width;
    height = P.height;
    memcpy(counts, P.counts, numCellKinds*sizeof(int));
    return *this;
}

void Dmatrix::swap(Dmatrix& M)
{
    assert(M.width == this->width);
    assert(M.height == this->height);
    Darray::swap(M);
    std::swap(this->counts, M.counts); 
}

Darray Dmatrix::extractColumn(uint32_t col) {
    Dmatrix M = *this;
    Darray column = Darray(height);
    for (uint32_t i = 0; i < width; i++) {
        column(i) = M(i, col); 
    }
    return column;
} 

Darray Dmatrix::extractRow(uint32_t r) {
    Dmatrix M = *this;
    Darray row = Darray(width);
    for (uint32_t j = 0; j < height; j++) {
        row(j) = M(r, j); 
    }
    return row;
} 
