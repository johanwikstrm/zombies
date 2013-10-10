#ifndef CONSTANTS_H
#define CONSTANTS_H

// NOTE: for the MPI stuff to work PROC_WIDTH*PROC_HEIGHT 
// must be equal to the number of MPI processes
#define PROC_WIDTH 2
#define PROC_HEIGHT 2

#define NKINDS 4

#define EMPTY 0
#define HUMAN 1
#define INFECTED 2
#define ZOMBIE 3

char * kindstr(int kind);

typedef int error;

enum direction{UP,RIGHT,DOWN,LEFT};
typedef enum direction DIRECTION;

#endif
