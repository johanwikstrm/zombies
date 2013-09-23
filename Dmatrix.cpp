using namespace std;
#include <cassert>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include "Dmatrix.h"


// constructeur avec paramètres
Dmatrix::Dmatrix(uint32_t h, uint32_t w, int init):Darray(h*w, init)
{
    height = h;
    width = w;
    counts = (uint32_t*)calloc(4, sizeof(uint32_t));
    counts[init] = h*w;
}

// constructeur par copie
// On choisit que si le vecteur P est propriétaire de ses données, le vecteur créé le sera aussi
// mais si le vecteur P n'est pas propriétaire de ses données, le vecteur créé ne le sera pas non plus
Dmatrix::Dmatrix(const Dmatrix& P):Darray(P)
{
    width = P.width;
    height = P.height;
    counts = (uint32_t*)calloc(4, sizeof(uint32_t));
    for (int i = 0; i <3; i++) {
        counts[i] = P.counts[i];
    }
}

Dmatrix::~Dmatrix()
{
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
            cout <<M(i, j) <<" ";
        }
        cout <<"\n";
    }
}


void Dmatrix::set(uint32_t i, uint32_t j, int newValue) 
{
    Dmatrix M = *this;
    int previousValue = M(i, j);
    counts[previousValue] --;
    M(i,j) = newValue;    
    counts[newValue] ++;
}

int Dmatrix::operator()(uint32_t i, uint32_t j) const
{
    assert(i>=0 && i<height && j>=0 && j<width);
    return Darray::operator()(i*height + j);
}

int& Dmatrix::operator()(uint32_t i, uint32_t j) 
{
    assert(i>=0 && i<height && j>=0 && j<width);
    return Darray::operator()(i*height + j);
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
                if (M(i, j) != N(i, j)) {
                    return false;
                }
            } 
        }
        return true;
    } else {
        return false;
    }
}
