#ifndef LOCK_H
#define LOCK_H

#include <stdint.h>

class Lock {
    
    private:
        uint32_t height;
        // if locks[y] == TRUE, then the column y is currently used by one of the threads
        bool* locks;

    public:
        Lock(uint32_t height);
        ~Lock();
        void lock(uint32_t y);
        void unlock(uint32_t y);
};

#endif
