#ifndef BUFFER_H
#define BUFFER_H

#include <mpi.h>
#include "Cell.h"
#include "Array.h"
#include "constants.h"

class Buffer {

    public:
        explicit Buffer(uint32_t count);
        explicit Buffer(Array& array);
        ~Buffer();
        void * rawData();
        Array * toArray();
        int count();
        // NOTE: uncommitted datatype
        static error datatype(MPI_Datatype *type);

    private:
        Cell* cells;
        uint32_t cellCount;

};

#endif