#ifndef CONSTANTS_H
#define CONSTANTS_H

// NOTE: for the MPI stuff to work PROC_WIDTH*PROC_HEIGHT 
// must be equal to the number of MPI processes
#define PROC_WIDTH 2
#define PROC_HEIGHT 2

// Number of OMP threads
#define NUM_THREADS 3

#define NKINDS 4

#define EMPTY 0
#define HUMAN 1
#define INFECTED 2
#define ZOMBIE 3

// The node used as MPI root node throughout the application.
#define ROOT_NODE 0

typedef int error;

enum direction{UP,RIGHT,DOWN,LEFT};
typedef enum direction DIRECTION;

#endif
