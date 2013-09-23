#include <cassert>
#include "Dmatrix.h"

int main ()
{
    // Test only the initialization
    Dmatrix matrix_0 = Dmatrix(5, 5);
    assert(matrix_0.getCount(0) == 25);
    assert(matrix_0.getCount(1) == 0);

    // Test set (and count update)
    Dmatrix matrix_1 = Dmatrix(4, 4, 1);
    assert(matrix_1.getCount(1) == 16);
    assert(matrix_1.getCount(0) == 0);
    matrix_1.set(0, 0, 0);
    assert(matrix_1.getCount(1) == 15);
    assert(matrix_1.getCount(0) == 1);
    matrix_1.set(1, 1, 1);
    assert(matrix_1.getCount(1) == 15);
    assert(matrix_1.getCount(0) == 1);
    matrix_1.set(2, 2, 3);
    assert(matrix_1.getCount(1) == 14);
    assert(matrix_1.getCount(0) == 1);
    assert(matrix_1.getCount(3) == 1);
}
