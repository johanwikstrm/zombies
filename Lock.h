#ifndef LOCK_H
#define LOCK_H

#include <pthread.h>
#include <stdint.h>
#include "Matrix.h"


class Lock {
    

    private:
        
        
        uint32_t height;
        // if locks[y] == TRUE, then the column y is currently used by one of the threads
        bool* locks;
        // Mutex variable

    public:
        Lock(uint32_t height);
        ~Lock();
        bool getValue(uint32_t y);
        void lock(uint32_t y);
        void unlock(uint32_t y);
};

#endif
