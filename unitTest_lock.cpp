#include <stdlib.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include "Lock.h"

using namespace std;

#define NUM_THREAD 10

int array[10] = {0};
Lock locks = Lock(10);

void *thread1(void *a) {
    long id = (long) a;
//    printf("Thread n°%li execution \n", id);
    fflush(stdout);
    locks.lock(id);
    assert(locks.getValue(id));    
    assert(locks.getValue((id-1+10)%10));    
    assert(locks.getValue((id+1)%10));    
    array[id] = id;
    array[(id-1+10)%10] = id; 
    array[(id+1)%10] = id;
    usleep(1000 * drand48());
    // Make sure no other thread has changed the valued
    assert(array[id] == id);
    assert(array[(id-1+10)%10] == id); 
    assert(array[(id+1)%10] == id);
    locks.unlock(id);   
//    printf("Thread n°%li execution end\n", id);
    fflush(stdout);
    return (void*)0L;
}


int main(int argc, char *argv[]){

    for (uint32_t y = 0; y < 10; y++) {
        assert(!locks.getValue(y));
    }
    locks.lock(3);
    assert(locks.getValue(2));
    assert(locks.getValue(3));
    assert(locks.getValue(4));
    locks.unlock(3);
    assert(!locks.getValue(2));
    assert(!locks.getValue(3));
    assert(!locks.getValue(4));
    locks.lock(9);
    assert(locks.getValue(8));
    assert(locks.getValue(9));
    assert(locks.getValue(0));
    locks.unlock(9);
    assert(!locks.getValue(8));
    assert(!locks.getValue(9));
    assert(!locks.getValue(0));
    locks.lock(0);
    assert(locks.getValue(9));
    assert(locks.getValue(0));
    assert(locks.getValue(1));
    locks.unlock(0);
    assert(!locks.getValue(9));
    assert(!locks.getValue(0));
    assert(!locks.getValue(1));

    // A thread is waiting
    void *status;
    pthread_t threads[NUM_THREAD];

    for (long i = 0; i < NUM_THREAD; i++) {
  //      printf("Creation thread n° %li \n", i);
        pthread_create(&(threads[i]), NULL, thread1, (void*) i);
    }

    // Wait for the end of every thread
    for (long i = 0; i < NUM_THREAD; i++) {
        pthread_join(threads[i], &status);
        if (status != (void*)0) {
    //        printf("Error in the execution");
            return 1;
        } 
    }

    //printf("All thread have finished \n");

    return 0;

}
