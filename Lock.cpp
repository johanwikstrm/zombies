#include "Lock.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond =  PTHREAD_COND_INITIALIZER;

using namespace std;

Lock::Lock(uint32_t height) {
    this->height = height;
    locks = new bool[height];
    for (uint32_t i = 0; i < height; i++) locks[i] = false;
}

Lock::~Lock() {
    delete[] locks;
}

bool Lock::getValue(uint32_t y) {
    return locks[y];
}

void Lock::lock(uint32_t y) {
    pthread_mutex_lock(&mutex);
    while (locks[(y-1+height)%height] || locks[y] || locks[(y+1)%height]) {
        pthread_cond_wait(&cond, &mutex);
    }
    // Locking
    locks[(y-1+height)%height] = true; 
    locks[y] = true; 
    locks[(y+1)%height] = true;
    pthread_mutex_unlock(&mutex);
}

void Lock::unlock(uint32_t y) {
    // beginning of the critical section
    pthread_mutex_lock(&mutex);
    locks[(y-1+height)%height] = false; 
    locks[y] = false; 
    locks[(y+1)%height] = false;
    pthread_cond_broadcast(&cond);
    // end of theb critical section
    pthread_mutex_unlock(&mutex);
}
