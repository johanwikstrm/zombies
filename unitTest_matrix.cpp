using namespace std;
#include <iostream>
#include <cassert>
#include "Dmatrix.h"
#include "Darray.h"

int main ()
{
    // Test the constructors
    // Default Constructor
    Dmatrix size0 = Dmatrix();
    assert(size0.getWidth() == 0);
    assert(size0.getHeight() == 0);
    assert(size0.getCount(0) == 0);
    assert(size0.getCount(1) == 0);
    assert(size0.getCount(2) == 0);
    assert(size0.getCount(3) == 0);
    // Constructor with parameters
    // Without init value
    Dmatrix matrix_0 = Dmatrix(2, 2);
    assert(matrix_0.getWidth() == 2);
    assert(matrix_0.getHeight() == 2);
    assert(matrix_0.getCount(0) == 4);
    assert(matrix_0.getCount(1) == 0);
    assert(matrix_0.getCount(2) == 0);
    assert(matrix_0.getCount(3) == 0);
    // With init value
    Dmatrix matrix_1 = Dmatrix(4, 4, 1);
    assert(matrix_1.getWidth() == 4);
    assert(matrix_1.getHeight() == 4);
    assert(matrix_1.getCount(1) == 16);
    assert(matrix_1.getCount(0) == 0);
    assert(matrix_1.getCount(2) == 0);
    assert(matrix_1.getCount(3) == 0);

    // Test set (and count update)
    matrix_1.set(0, 0, 0);
    assert(matrix_1(0,0) == 0);
    assert(matrix_1.getCount(1) == 15);
    assert(matrix_1.getCount(0) == 1);
    assert(matrix_1.getCount(2) == 0);
    assert(matrix_1.getCount(3) == 0);
    matrix_1.set(1, 1, 1);
    assert(matrix_1(1,1) == 1);
    assert(matrix_1.getCount(1) == 15);
    assert(matrix_1.getCount(0) == 1);
    assert(matrix_1.getCount(2) == 0);
    assert(matrix_1.getCount(3) == 0);
    matrix_1.set(1, 1, 2);
    assert(matrix_1(1,1) == 2);
    assert(matrix_1.getCount(1) == 14);
    assert(matrix_1.getCount(0) == 1);
    assert(matrix_1.getCount(2) == 1);
    assert(matrix_1.getCount(3) == 0);

    // Test swap
    Dmatrix matrix_2 = Dmatrix(4,4,2);
    Dmatrix matrix_3 = Dmatrix(4,4,3);
    Dmatrix oldMatrix_2 = matrix_2;
    Dmatrix oldMatrix_3 = matrix_3;
    matrix_2.swap(matrix_3);
    assert(oldMatrix_2 == matrix_3);
    assert(oldMatrix_3 == matrix_2);
    for (int i = 0; i < 4; i++) {
        assert(oldMatrix_2.getCount(i) == matrix_3.getCount(i));
        assert(oldMatrix_3.getCount(i) == matrix_2.getCount(i));
    }

    // Test overloading of =
    matrix_2 = matrix_3;
    assert(matrix_2 == matrix_3);
    for (int i = 0; i < 4; i++) {
        assert(matrix_2.getCount(i) == matrix_3.getCount(i));
    }
    size0 = matrix_3;
    assert(size0 == matrix_3);
    for (int i = 0; i < 4; i++) {
        assert(size0.getCount(i) == matrix_3.getCount(i));
    }

    // Extract colum
    for (int i = 0; i < 4; i++) {
        matrix_2.set(i, 0, i);
    }
    Darray column0 = matrix_2.extractColumn(0);
    for (int i = 0; i < 4; i++) {
        assert(column0(i) == i);
    }

    // Extract row
    for (int j = 0; j < 4; j++) {
        matrix_2.set(0, j, j);
    }
    Darray row0 = matrix_2.extractRow(0);
    for (int j = 0; j < 4; j++) {
        assert(row0(j) == j);
    }
}
