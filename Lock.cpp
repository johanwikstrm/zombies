#include "Lock.h"

using namespace std;

Lock::Lock(uint32_t height) {
    this->height = height;
    locks = new bool[height];
    for (int i = 0; i < height; i++) locks[i] = false;
}

Lock::~Lock() {
    delete[] locks;
}

void Lock::lock(uint32_t y) {
    for (bool locked = false; locked == false; /*NOP*/) {
#pragma omp critical (LockRegion)
        {
            locked = !locks[(y-1+height)%height] && !locks[y] && !locks[(y+1)%height];
            if (locked) {
                locks[(y-1+height)%height] = true; 
                locks[y] = true; 
                locks[(y+1)%height] = true;
            }
        }
    }
}

void Lock::unlock(uint32_t y) {
#pragma omp critical (LockRegion)
    {
        locks[(y-1+height)%height] = false; 
        locks[y] = false; 
        locks[(y+1)%height] = false;
    }
}

