using namespace std;
#include <iostream>
#include <cassert>
#include "Buffer.h"

int main ()
{
    Buffer buf = Buffer(10);
    assert(buf.count() == 10);
}
