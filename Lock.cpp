#include "Lock.h"

using namespace std;

Lock::Lock(uint32_t height) {
    this->height = height;
    locks = new bool[height];
    for (uint32_t i = 0; i < height; i++) locks[i] = false;
    pthread_mutex_init(&mutex, NULL);

}

Lock::~Lock() {
    delete[] locks;
}

bool Lock::getValue(uint32_t y) {
    return locks[y];
}

/**
  void Lock::lock(uint32_t y) {
  for (bool locked = false; locked == false; ) {
#pragma omp critical (LockRegion)

cout <<"Try to lock column "<<y <<endl <<flush;
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
*/

void Lock::lock(uint32_t y) {
    for (bool locked = false; locked == false; ) {
        // beginning of the critical section
//#pragma omp critical (LockRegion)
        {
        pthread_mutex_lock(&mutex);
        locked = !locks[(y-1+height)%height] && !locks[y] && !locks[(y+1)%height];
        if (locked) {
            // Locking
            locks[(y-1+height)%height] = true; 
            locks[y] = true; 
            locks[(y+1)%height] = true;
        }
        // end of the critical section
        pthread_mutex_unlock(&mutex);
        }
    }
}

/**
  void Lock::unlock(uint32_t y) {
#pragma omp critical (LockRegion)
{
locks[(y-1+height)%height] = false; 
locks[y] = false; 
locks[(y+1)%height] = false;
}
}
*/

void Lock::unlock(uint32_t y) {
    // beginning of the critical section
//#pragma omp critical (LockRegion)
    {
    pthread_mutex_lock(&mutex);
    locks[(y-1+height)%height] = false; 
    locks[y] = false; 
    locks[(y+1)%height] = false;
    // end of the critical section
    pthread_mutex_unlock(&mutex);
    }
}

/**
void Lock::print() {
    for (uint32_t y = 0; y < height; y++) {
        if (locks[y]) {
            cout <<"1 "<<fflush;
        } else {
            cout <<"0 "<<fflush;
        }
    }
    cout <<endl <<fflush;
}
*/
