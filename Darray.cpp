using namespace std;
#include <iostream>
#include <fstream>
#include <ctime>
#include <cassert>
#include <string.h>

#include "Darray.h"

// constructeur par défaut
Darray::Darray()
{
    size = 0;
}

// constructeur avec paramètres
Darray::Darray(uint32_t s, Cell* init)
{
    // initilisation de la size du tableau
    size = s;
    // si la size est nulle , pas d'allocation de tableau
    if (size == 0)  {
        return;
    }
    // allocation de la mémoire pour le tableau
    array = new Cell*[size];
    // initilisation des valeurs du tableau
    for (uint32_t i = 0; i < size; i++) {
        array[i] = init;
    }
}

// constructeur par copie
Darray::Darray(const Darray& P)
{
    // initilisation de la size du tableau
    size = P.size;
    if (size == 0) {
        return;
    }
    // allocation de la mémoire pour le tableau
    array = new Cell*[size];
    // copie des valeurs dans le nouveau tableau
    for (uint32_t i = 0; i < size; i++) {
        array[i] = P.array[i];
    }
}

Darray::~Darray()
{
    if (size == 0) {
        return;
    }
    delete [] array;
}

uint32_t Darray::getSize() const 
{
    return size;
}

void Darray::print() const
{
    cout <<"Tableau : \n";
    for (uint32_t i = 0; i < size; i++) {
        cout <<array[i] <<" ";
    }
    cout <<"\n";
}

Cell* Darray::operator()(uint32_t i) const
{
    assert(i >= 0 && i < size);
    return array[i];
}

Cell*& Darray::operator()(uint32_t i) 
{
    assert(i >= 0 && i < size);
    return array[i];
}

Darray& Darray::operator=(const Darray& P)
{
    if (size == 0 && P.size == 0) {
        return *this;
    }

    if (size != P.size) {
        if (size != 0) {
            delete [] array;
        }
        size = P.size;
        array = new Cell*[size];
    } 

    memcpy(array, P.array, size*sizeof(Cell*));
    return *this;
}



bool Darray::operator==(const Darray& P) const
{
    // Egalité de la size des deux vecteurs
    if (size == P.getSize()) {
        // Egalite des valeurs
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
void Darray::swap(Darray& P) 
{
    Cell** tmp = P.array;
    P.array = array;
    array = tmp;
}

ostream& operator<< (ostream& out, const Darray& P) 
{
    for(uint32_t i = 0; i < P.getSize(); i++) {
        out<< P(i) <<endl;
    }
    out<< endl;
    return out;
}
/*
istream& operator>> (istream& in, Darray& P)
{
    // si le fichier contient trop de données, on arrête les affectations une fois que le arrayleau est rempli
    // si le fichier contient moins de données que la size du talbeau, 
    // on affecte seulement les données du fichier (les autres cases du arrayleau restant inchangées)
    for(uint32_t i = 0; i < P.getSize(); i++) {
        in>> P(i);
        if (in.eof()) {
            return in;
        }
    }
    return in;
}
*/