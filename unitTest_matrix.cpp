using namespace std;
#include <iostream>
#include <cassert>
#include "Matrix.h"
#include "constants.h"

int main ()
{
    // Test the constructors
    // Default Constructor
    Matrix size0 = Matrix();
    assert(size0.getWidth() == 0);
    assert(size0.getHeight() == 0);
    assert(size0.getCount(0) == 0);
    assert(size0.getCount(1) == 0);
    assert(size0.getCount(2) == 0);
    assert(size0.getCount(3) == 0);

    // Constructor with parameters
    // Without init value
    Matrix matrix_0 = Matrix(2, 2);
    assert(matrix_0.getWidth() == 2);
    assert(matrix_0.getHeight() == 2);
    assert(matrix_0.getCount(0) == 4);
    assert(matrix_0.getCount(1) == 0);
    assert(matrix_0.getCount(2) == 0);
    assert(matrix_0.getCount(3) == 0);

    
    // Test set (and count update)
    Matrix matrix_1 = Matrix(4, 4);
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            matrix_1.set(i,j, 1);
        }
    }

    // Test set (and count update)
    matrix_1.set(0, 0, 0);
    assert(matrix_1(0,0)->getKind() == 0);
    assert(matrix_1.getCount(1) == 15);
    assert(matrix_1.getCount(0) == 1);
    matrix_1.set(1, 1, 0);
    assert(matrix_1.getCount(1) == 14);
    assert(matrix_1.getCount(0) == 2);
    assert(matrix_1.getCount(2) == 0);
    assert(matrix_1.getCount(3) == 0);
    matrix_1.set(1, 1, 2);
    assert(matrix_1(1,1)->getKind() == 2);
    assert(matrix_1.getCount(1) == 14);
    assert(matrix_1.getCount(0) == 1);
    assert(matrix_1.getCount(2) == 1);
    assert(matrix_1.getCount(3) == 0);

    // Test swap
    Matrix matrix_2 = Matrix(4,4);
    Matrix matrix_3 = Matrix(4,4);
    matrix_2.set(1,1, 2);
    Matrix oldMatrix_2 = matrix_2;
    Matrix oldMatrix_3 = matrix_3;
    matrix_2.swap(matrix_3);
    assert(oldMatrix_2 == matrix_3);
    assert(oldMatrix_3 == matrix_2);
    for (uint32_t i = 0; i < 4; i++) {
        assert(oldMatrix_2.getCount(i) == matrix_3.getCount(i));
        assert(oldMatrix_3.getCount(i) == matrix_2.getCount(i));
    }

    // Test overloading of =
    matrix_2 = matrix_3;
    assert(matrix_2 == matrix_3);
    for (uint32_t i = 0; i < 4; i++) {
        assert(matrix_2.getCount(i) == matrix_3.getCount(i));
    }
    
    size0 = matrix_3;
    assert(size0 == matrix_3);
    for (uint32_t i = 0; i < 4; i++) {
        assert(size0.getCount(i) == matrix_3.getCount(i));
    }

    // Extract column
    for (uint32_t i = 0; i < 4; i++) {
        matrix_2.set(0, i, i);
    }
    Array* column0 = matrix_2.extractColumn(0,0);
    for (uint32_t i = 0; i < 4; i++) {
        assert((*column0)(i)->getKind() == i);
    }
    delete column0;
    // Extract row
    for (uint32_t j = 0; j < 4; j++) {
        matrix_2.set(j,0, j);
    }
    Array* row0 = matrix_2.extractRow(0,0);
    for (uint32_t j = 0; j < 4; j++) {
        assert((*row0)(j)->getKind() == j);
    }
    delete row0;
    
    // testing insertion of columns
    Matrix matrix_4 = Matrix(4,5);
    matrix_4.set(1,2,INFECTED);
/*  
    E E E E E 
    E E E E E 
    E I E E E 
    E E E E E 
*/
    // no collision
    Array * insCol = new Array(4);
    insCol->set(1,ZOMBIE);
    assert(matrix_4.insertColumnWithCollisions(insCol,1,0) == 0);
/*  
    E E E E E 
    E Z E E E 
    E I E E E 
    E E E E E 
*/
    assert(matrix_4(1,1)->getKind() == ZOMBIE);
    assert(matrix_4(1,2)->getKind() == INFECTED);
    insCol->set(1,HUMAN);
    assert(matrix_4.insertColumnWithCollisions(insCol,1,0) == 1);
/*  
    E E E E E 
    E ? E E E 
    E I E E E 
    E E E E E 
*/
    delete insCol;
    insCol = new Array(5);
    insCol->set(1,INFECTED);
    assert(matrix_4.insertRowWithCollisions(insCol,3,0) == 0);
/*  
    E E E E E 
    E ? E E E 
    E I E E E 
    E I E E E 
*/
    assert(matrix_4(1,3)->getKind() == INFECTED);
    assert(matrix_4.insertRowWithCollisions(insCol,2,0) == 1);
/*  
    E E E E E 
    E ? E E E 
    E ? E E E 
    E I E E E 
*/

    delete insCol;

}
