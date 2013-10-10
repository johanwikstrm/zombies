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
//Dmatrix::Dmatrix(uint32_t h, uint32_t w):Darray(h*w , NULL)
Dmatrix::Dmatrix(uint32_t h, uint32_t w):Darray(h*w , EMPTY)
{
    //cout <<"Dmatrix constructor" <<endl;
    height = h;
    width = w;
  //  dummy = new Cell(EMPTY); // is used as a replacer for ALL empty cells
    counts = new uint32_t[numCellKinds];
    for (uint32_t i = 0; i < numCellKinds; i++) {
        counts[i] = 0;
    }
    counts[EMPTY] = h*w;
}

int Dmatrix::kind(Cell* ptr){
    /**
    if (ptr == 0)
    {
        return EMPTY;
    }else{
        return ptr->kind();
    }
    **/

    return ptr->kind();
}

// constructeur par copie
// On choisit que si le vecteur P est propriétaire de ses données, le vecteur créé le sera aussi
// mais si le vecteur P n'est pas propriétaire de ses données, le vecteur créé ne le sera pas non plus
Dmatrix::Dmatrix(const Dmatrix& P):Darray(P)
{
    //cout <<"Dmatrix constructor" <<endl;
    width = P.width;
    height = P.height;
    //dummy = new Cell(EMPTY);
    counts = new uint32_t[numCellKinds];
    for (int i = 0; i < numCellKinds; i++) {
        counts[i] = P.counts[i];
    }
}

Dmatrix::Dmatrix():Darray()
{
    //cout <<"Dmatrix constructor" <<endl;
    width = 0;
    height = 0;
    counts = new uint32_t[numCellKinds];
    for (uint32_t i = 0; i < numCellKinds; i++) {
        counts[i] = 0;
    }
}

Dmatrix::~Dmatrix()
{
    //cout <<"Dmatrix destructor" <<endl;
    //delete dummy;
    delete[] counts;
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
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            char kind = 'X';
            switch(M(x, y)->kind()){
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


void Dmatrix::set(uint32_t x, uint32_t y, int k) 
{
    int previousKind = kind(array[y*width + x]);
    counts[previousKind]--;
    delete array[y*width+x];
    array[y*width + x] = NULL;
    array[y*width + x] = new Cell(k);
    counts[k]++;
}

void Dmatrix::move(uint32_t oldX, uint32_t oldY, uint32_t newX, uint32_t newY)
{
    Dmatrix M = *this;
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

void Dmatrix::getInfected(uint32_t x, uint32_t y) {
    Cell* person = this->operator()(x, y);
    assert(kind(person) == HUMAN || kind(person) == INFECTED);
    this->operator()(x, y)->setKind(INFECTED);
    counts[HUMAN]--;
    counts[INFECTED]++; 
}


Cell* Dmatrix::operator()(uint32_t x, uint32_t y) const {
    assert(y>=0 && y<height && x>=0 && x<width);
    Cell* c = Darray::operator()(y*width + x);
    /**
    if (c == NULL){
        return dummy;
    }else {
        return c;
    }
    */
    return c;
}

Cell* Dmatrix::operator()(Coord c) const {
    assert(c.getY()>=0 && c.getY()<height && c.getX()>=0 && c.getX()<width);
    Cell* cell = Dmatrix::operator()(c.getX(), c.getY());
    return cell;
}



Cell*& Dmatrix::operator()(uint32_t x, uint32_t y) 
{
    assert(y>=0 && y<height && x>=0 && x<width);
    /**
    Cell* c = Darray::operator()(y*width + x);
    if (c == NULL){
        return dummy;
    }else {
        return Darray::operator()(y*width + x);
    }
    **/
    return Darray::operator()(y*width + x);
}

Cell*& Dmatrix::operator()(Coord c) 
{
    assert(c.getY()>=0 && c.getY()<height && c.getX()>=0 && c.getX()<width);
    return Dmatrix::operator()(c.getX(), c.getY());
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

Darray* Dmatrix::extractColumn(uint32_t col) {
    //Dmatrix M = *this;
    Darray* column = new Darray(height);
    for (uint32_t y = 0; y < height; y++) {
        (*column)(y) = new Cell(*(*this)(col,y)); 
    }
    return column;
} 

Darray* Dmatrix::extractRow(uint32_t r) {
    //Dmatrix M = *this;
    Darray* row = new Darray(width);
    for (uint32_t x = 0; x < width; x++) {
        (*row)(x) = new Cell(*(*this)(x,r)); 
    }
    return row;
}

// Returns the number of collisions
int Dmatrix::insertColumnWithCollisions(Darray * toInsert,uint32_t col){
    assert(toInsert->getSize() == height);
    int collisions = 0;
    for (uint32_t y = 0; y < height; y++) {
        int oldKind = (*this)(col,y)->kind();
        int newKind = (*toInsert)(y)->kind();
        if (oldKind != EMPTY && newKind != EMPTY){
            /*cout << "Collision inserting " << kindstr((*toInsert)(y)->kind())
                << " into cell with " << kindstr((*this)(col,y)->kind()) << " in it at "
                << col<<","<<y<<endl;*/
            collisions++;
        }else if (oldKind == EMPTY){
            this->set(col,y,(*toInsert)(y)->kind());    
        }    
    }
    return collisions;  
}

// Returns the number of collisions
int Dmatrix::insertRowWithCollisions(Darray * toInsert,uint32_t row){
    assert(toInsert->getSize() == width);
    int collisions = 0;
    for (uint32_t x = 0; x < width; x++) {
        int oldKind = (*this)(x,row)->kind();
        int newKind = (*toInsert)(x)->kind();
        if (oldKind != EMPTY && newKind != EMPTY){
            /*cout << "Collision inserting " << kindstr((*toInsert)(x)->kind())
                << " into cell with " << kindstr((*this)(x,row)->kind()) << " in it at "
                << x<<","<<row<<endl;*/
            collisions++;
        }else if (oldKind == EMPTY){
            this->set(x,row,(*toInsert)(x)->kind());    
            (*this)(x,row)->setMoveFlag((*toInsert)(x)->moveFlag());
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
Darray** Dmatrix::toSend(int offset){
    assert(width >=4 && height >= 4);// assuming a matrix that is at least 4x4
    assert(offset == 1 || offset == 0);
    Darray **toRet = (Darray**)calloc(4,sizeof(Darray*));
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
int Dmatrix::insertWithCollisions(Darray** toInsert, int offset){
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
